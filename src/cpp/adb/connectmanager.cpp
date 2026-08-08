#include "connectmanager.h"
#include <QtQml>
#include <QTimer>
#include "../utils/notificationcontroller.h"
#include "../settingPageTool/othersettingshandler.h"
#include "src/cpp/adb/adbtools.h"
#include "src/cpp/adb/device.h"
#include "src/cpp/adb/fastbootdevice.h"
#include "src/cpp/utils/utils.hpp"

namespace ADT {

ConnectManager::ConnectManager(QObject *parent)
    : QObject(parent)
    , m_cutADBDevice(nullptr)
    , m_cutFastbootDevice(nullptr)
    , m_deviceCheckTimer(nullptr)
    , m_enableADBCheck(true)
    , m_enableFastbootCheck(true)
    , m_refreshInProgress(false)
    , m_wirelessOperationRunning(false)
    , m_adbStateMessage("等待扫描")
{
    m_deviceCheckTimer = new QTimer(this);
    m_deviceCheckTimer->setInterval(OtherSettingsHandler::instance()->deviceRefreshInterval());
    connect(m_deviceCheckTimer, &QTimer::timeout, this, &ConnectManager::refreshDevice);
    
    // 当设备刷新时间设置改变时，更新定时器间隔
    connect(OtherSettingsHandler::instance(), &OtherSettingsHandler::deviceRefreshIntervalChanged, this, [this](int interval) {
        m_deviceCheckTimer->setInterval(interval);
    });
}

void ConnectManager::startCheckDevice()
{
    m_deviceCheckTimer->start();
    refreshDevice();
}

void ConnectManager::startADBServer(std::function<void()> callback)
{
    setadbServerStarting(true);
    asyncOperator([this, callback]() {
        const bool started = ADBTOOL->startService();
        QMetaObject::invokeMethod(this, [this, callback, started]() {
            if (!started) {
                NotificationController::instance()->send("ADB服务启动失败", "请检查ADB服务是否已启动", NotificationController::Error);
            }
            setadbServerStarting(false);
            if (callback) {
                callback();
            }
        }, Qt::QueuedConnection);
    });
}

void ConnectManager::killADBServer()
{
    setadbServerStarting(true);
    asyncOperator([this]() {
        ADBTOOL->killService();
        QMetaObject::invokeMethod(this, [this]() { setadbServerStarting(false); }, Qt::QueuedConnection);
    });
}

void ConnectManager::restartADBServer()
{
    stopCheckDevice();
    setadbServerStarting(true);
    asyncOperator([this]() {
        ADBTOOL->killService();
        const bool started = ADBTOOL->startService();
        QMetaObject::invokeMethod(this, [this, started]() {
            if (!started) {
                NotificationController::instance()->send("ADB服务启动失败", "请检查ADB服务是否已启动", NotificationController::Error);
            }
            setadbServerStarting(false);
            startCheckDevice();
        }, Qt::QueuedConnection);
    });
}

void ConnectManager::stopCheckDevice()
{
    m_deviceCheckTimer->stop();
}

QVector<QSharedPointer<Device>> ConnectManager::devices(ConnectStatus type) const
{
    if (type == C_ADB) {
        QVector<QSharedPointer<Device>> devices;
        for (const auto &device : m_adbDeviceList) {
            devices.push_back(device);
        }
        return devices;
    } else if (type == C_Fastboot) {
        QVector<QSharedPointer<Device>> devices;
        for (const auto &device : m_fastbootDeviceList) {
            devices.push_back(device);
        }
        return devices;
    }
    return {};
}

QSharedPointer<ADBDevice> ConnectManager::selectedADBDevice() const
{
    for (const auto &device : m_adbDeviceList) {
        if (device.get() == cutADBDevice()) {
            return device;
        }
    }
    return {};
}

void ConnectManager::requestSetCutADBDevice(const QString &deviceCode)
{
    for (const auto &device : m_adbDeviceList) {
        if (device->code() == deviceCode) {
            setcutADBDevice(device.get());
            break;
        }
    }
}

void ConnectManager::requestSetCutFastbootDevice(const QString &deviceCode)
{
    for (const auto &device : m_fastbootDeviceList) {
        if (device->code() == deviceCode) {
            setcutFastbootDevice(device.get());
            break;
        }
    }
}

void ConnectManager::requestPairDevice(const QString &ipPort, const QString &pairCode)
{
    if (wirelessOperationRunning()) {
        NotificationController::instance()->send("操作进行中", "请等待当前无线任务完成", NotificationController::Warning);
        return;
    }
    if (ipPort.isEmpty() || pairCode.isEmpty()) {
        NotificationController::instance()->send("配对失败", "请输入配对地址和配对码", NotificationController::Warning);
        return;
    }

    setwirelessOperationRunning(true);
    NotificationController::instance()->send("配对中", ipPort, NotificationController::Info);
    asyncOperator([ipPort, pairCode, this](){
        const auto result = ADBTOOL->executeCommand(ADBTools::ADB, {"pair", ipPort}, pairCode).simplified();
        NotificationController::instance()->send(result.contains("Success") ? "配对成功" : "配对失败", result,
            result.contains("Success") ? NotificationController::Info : NotificationController::Error);
        QMetaObject::invokeMethod(this, [this]() { setwirelessOperationRunning(false); }, Qt::QueuedConnection);
    });
}

void ConnectManager::requestConnectDevice(const QString &ipPort)
{
    if (wirelessOperationRunning()) {
        NotificationController::instance()->send("操作进行中", "请等待当前无线任务完成", NotificationController::Warning);
        return;
    }
    if (ipPort.isEmpty()) {
        NotificationController::instance()->send("连接失败", "请输入设备地址和端口", NotificationController::Warning);
        return;
    }

    setwirelessOperationRunning(true);
    NotificationController::instance()->send("连接中", ipPort, NotificationController::Info);
    asyncOperator([ipPort, this](){
        const auto result = ADBTOOL->executeCommand(ADBTools::ADB, {"connect", ipPort}).simplified();
        const bool connected = result.contains("connected");
        NotificationController::instance()->send(connected ? "连接成功" : "连接失败", result,
            connected ? NotificationController::Info : NotificationController::Error);
        QMetaObject::invokeMethod(this, [this, connected]() {
            setwirelessOperationRunning(false);
            if (connected) startCheckDevice();
        }, Qt::QueuedConnection);
    });
}

void ConnectManager::refreshDevice()
{
    if (refreshInProgress()) {
        return;
    }

    const bool checkADB = enableADBCheck();
    const bool checkFastboot = enableFastbootCheck();
    if (!checkADB && !checkFastboot) {
        emit deviceRefreshFinish();
        return;
    }

    setrefreshInProgress(true);
    asyncOperator([this, checkADB, checkFastboot]() {
        QString adbState = checkADB ? QString() : QStringLiteral("ADB 检查已关闭");
        const QVector<QString> adbDevices = checkADB ? getDeviceList(C_ADB, &adbState) : QVector<QString>{};
        const QVector<QString> fastbootDevices = checkFastboot ? getDeviceList(C_Fastboot) : QVector<QString>{};
        QMetaObject::invokeMethod(this, [this, adbDevices, fastbootDevices, adbState]() {
            setadbStateMessage(adbState);
            updateDevices(adbDevices, fastbootDevices);
        }, Qt::QueuedConnection);
    });
}

void ConnectManager::updateDevices(const QVector<QString> &adbDevices, const QVector<QString> &fastbootDevices)
{
    if (enableADBCheck()) {
        for (const QString &deviceCode : adbDevices) {
            if (!deviceCode.isEmpty() && !hasDevice(deviceCode, C_ADB)) {
                const auto device = addDevice(deviceCode, C_ADB);
                emit deviceConnected(device);
                NotificationController::instance()->send("发现设备通过adb连接", device->code() + "已连接");

                if (!cutADBDevice()) {
                    const auto adbDevice = device.dynamicCast<ADBDevice>();
                    if (adbDevice) {
                        setcutADBDevice(adbDevice.get());
                    }
                }
            }
        }

        for (int i = m_adbDeviceList.size() - 1; i >= 0; --i) {
            bool deviceFound = false;
            for (const QString &deviceCode : adbDevices) {
                if (m_adbDeviceList[i]->code() == deviceCode) {
                    deviceFound = true;
                    break;
                }
            }
            if (deviceFound) {
                continue;
            }

            const auto disconnectingDevice = m_adbDeviceList[i];
            emit deviceDisconnected(disconnectingDevice);
            NotificationController::instance()->send("adb设备已断开", disconnectingDevice->code() + "已断开");

            if (cutADBDevice() == disconnectingDevice.get()) {
                setcutADBDevice(nullptr);
                for (const auto &device : m_adbDeviceList) {
                    if (device.get() != disconnectingDevice.get()) {
                        setcutADBDevice(device.get());
                        break;
                    }
                }
            }
            m_adbDeviceList.remove(i);
        }
    }

    if (enableFastbootCheck()) {
        for (const QString &deviceCode : fastbootDevices) {
            if (!deviceCode.isEmpty() && !hasDevice(deviceCode, C_Fastboot)) {
                const auto device = addDevice(deviceCode, C_Fastboot);
                emit deviceConnected(device);
                NotificationController::instance()->send("发现设备通过fastboot连接", device->code() + "已连接");

                if (!cutFastbootDevice()) {
                    const auto fastbootDevice = device.dynamicCast<FastbootDevice>();
                    if (fastbootDevice) {
                        setcutFastbootDevice(fastbootDevice.get());
                    }
                }
            }
        }

        for (int i = m_fastbootDeviceList.size() - 1; i >= 0; --i) {
            bool deviceFound = false;
            for (const QString &deviceCode : fastbootDevices) {
                if (m_fastbootDeviceList[i]->code() == deviceCode) {
                    deviceFound = true;
                    break;
                }
            }
            if (deviceFound) {
                continue;
            }

            const auto disconnectingDevice = m_fastbootDeviceList[i];
            emit deviceDisconnected(disconnectingDevice);
            NotificationController::instance()->send("fastboot设备已断开", disconnectingDevice->code() + "已断开");

            if (cutFastbootDevice() == disconnectingDevice.get()) {
                setcutFastbootDevice(nullptr);
                for (const auto &device : m_fastbootDeviceList) {
                    if (device.get() != disconnectingDevice.get()) {
                        setcutFastbootDevice(device.get());
                        break;
                    }
                }
            }
            m_fastbootDeviceList.remove(i);
        }
    }

    setrefreshInProgress(false);
    emit deviceRefreshFinish();
}

QVector<QString> ConnectManager::getDeviceList(ConnectStatus type, QString *statusMessage)
{
    QVector<QString> deviceList;

    if (type == C_ADB) {
        const auto result = ADBTOOL->executeCommandDetailed(ADBTools::ADB, {"devices"});
        if (!result.isSuccess()) {
            if (statusMessage) *statusMessage = "ADB 不可用，请查看错误记录";
            return deviceList;
        }

        QString issue;
        for (QString lineInfo : result.output.split('\n')) {
            lineInfo = lineInfo.simplified();
            const QStringList parts = lineInfo.split(' ');
            if (parts.size() != 2) continue;
            if (parts.last() == "device") deviceList.push_back(parts.first());
            else if (parts.last() == "unauthorized") issue = "设备等待 USB 调试授权";
            else if (parts.last() == "offline") issue = "设备离线，请重新连接";
        }
        if (statusMessage) {
            *statusMessage = !deviceList.isEmpty() ? QString("%1 台设备已连接").arg(deviceList.size())
                                                   : (issue.isEmpty() ? QStringLiteral("未发现设备") : issue);
        }
    } else if (type == C_Fastboot) {
        const auto output = ADBTOOL->executeCommand(ADBTools::FASTBOOT, {"devices"});
        for (QString lineInfo : output.split('\n')) {
            lineInfo = lineInfo.simplified();
            if (const QStringList parts = lineInfo.split('\t'); parts.size() == 2 && parts.last() == "fastboot") {
                deviceList.push_back(parts.first());
            }
        }
    }

    return deviceList;
}

bool ConnectManager::hasDevice(const QString &deviceCode, ConnectStatus type)
{
    if (type == C_ADB) {
        for (const auto &device : m_adbDeviceList) {
            if (device->code() == deviceCode) {
                return true;
            }
        }
    } else if (type == C_Fastboot) {
        for (const auto &device : m_fastbootDeviceList) {
            if (device->code() == deviceCode) {
                return true;
            }
        }
    }
    return false;
}

QSharedPointer<Device> ConnectManager::addDevice(const QString &deviceCode, ConnectStatus type)
{
    QSharedPointer<Device> device;
    if (type == C_ADB) {
        device = QSharedPointer<ADBDevice>::create(deviceCode);
        m_adbDeviceList.append(device.staticCast<ADBDevice>());
    } else if (type == C_Fastboot) {
        device = QSharedPointer<FastbootDevice>::create(deviceCode);
        m_fastbootDeviceList.append(device.staticCast<FastbootDevice>());
    }
    
    device->setconnectStatus(type);
    device->setisConnected(true);
    return device;
}

} // namespace ADT