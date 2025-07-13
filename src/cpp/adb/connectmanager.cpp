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
{
    m_deviceCheckTimer = new QTimer();
    m_deviceCheckTimer->setInterval(OtherSettingsHandler::instance()->deviceRefreshInterval());
    qWarning() << "设备刷新间隔:" << m_deviceCheckTimer->interval() << "ms";
    connect(m_deviceCheckTimer, &QTimer::timeout, this, &ConnectManager::refreshDevice);
    
    // 当设备刷新时间设置改变时，更新定时器间隔
    connect(OtherSettingsHandler::instance(), &OtherSettingsHandler::deviceRefreshIntervalChanged, this, [this](int interval) {
        m_deviceCheckTimer->setInterval(interval);
    });
}

void ConnectManager::startCheckDevice()
{
    m_deviceCheckTimer->start();
}

void ConnectManager::startADBServer(std::function<void()> callback)
{
    asyncOperator([this, callback](){
        setadbServerStarting(true);
        if (!ADBTOOL->startService()) {
            NotificationController::instance()->send("ADB服务启动失败", "请检查ADB服务是否已启动", NotificationController::Error);
        }
        setadbServerStarting(false);
        if (callback) {
            callback();
        }
    });
}

void ConnectManager::killADBServer()
{
    asyncOperator([this](){
        setadbServerStarting(true);
        ADBTOOL->killService();
        setadbServerStarting(false);
    });
}

void ConnectManager::stopCheckDevice()
{
    m_deviceCheckTimer->stop();
    qInfo() << "等待ADB执行结束";
    while(ADBTOOL->isRunning());
    qInfo() << "ADB执行结束";
}

QVector<QSharedPointer<Device>> ConnectManager::devices(ConnectStatus type) const
{
    qWarning() << "获取设备列表";
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
    if (ipPort.isEmpty()) {
        NotificationController::instance()->send("配对失败", "ip和端口不能为空");
        return;
    }
    if (pairCode.isEmpty()) {
        NotificationController::instance()->send("配对失败", "请输入配对码");
        return;
    }

    NotificationController::instance()->send("配对中", "请耐心等待", NotificationController::Info);

    asyncOperator([ipPort, pairCode, this](){
        auto retStr = ADBTOOL->executeCommand(ADBTools::ADB, {"pair", ipPort}, pairCode).simplified();
        if (retStr.contains("Success")) {
            NotificationController::instance()->send("配对成功", "请进行下一步");
        } else {
            NotificationController::instance()->send("配对失败", "配对失败，请检查信息是否填写正确", NotificationController::Error);
        }
    });
}

void ConnectManager::requestConnectDevice(const QString &ipPort)
{
    if (ipPort.isEmpty()) {
        NotificationController::instance()->send("连接失败", "ip和端口不能为空", NotificationController::Warning);
        return;
    }

    NotificationController::instance()->send("连接中", "请耐心等待", NotificationController::Info);

    asyncOperator([ipPort, this](){
        auto retStr = ADBTOOL->executeCommand(ADBTools::ADB, {"connect", ipPort}).simplified();
        NotificationController::instance()->send("返回信息", retStr);
        if (!retStr.contains("connected")) {
            NotificationController::instance()->send("连接失败", "请检查信息是否填写正确", NotificationController::Error);
        }
    });
}

void ConnectManager::refreshDevice()
{
    qInfo() << "开始检查设备";
    if (enableADBCheck()) {
        QVector<QString> adbDevices = getDeviceList(C_ADB);
        // 检查是否有新设备连接(ADB)
        for (QString &deviceCode : adbDevices) {
            if (!deviceCode.isEmpty() && !hasDevice(deviceCode, C_ADB)) {
                auto &&device = addDevice(deviceCode, C_ADB);
                emit deviceConnected(device);
                NotificationController::instance()->send("发现设备通过adb连接", QString(device->code() + "已连接"));
                
                // 如果是第一个ADB设备，设置为当前设备
                if (!cutADBDevice()) {
                    auto adbDevice = device.dynamicCast<ADBDevice>();
                    if (adbDevice) {
                        setcutADBDevice(adbDevice.get());
                    }
                }
            }
        }

        // 检查是否有设备断开连接(ADB)
        for (int i = m_adbDeviceList.size() - 1; i >= 0; i--) {
            // 检查是否有设备断开连接
            bool deviceFound = false;
            for (const auto &deviceCode : adbDevices) {
                if (m_adbDeviceList[i]->code() == deviceCode) {
                    deviceFound = true;
                    break;
                }
            }
            if (!deviceFound) {
                auto disconnectingDevice = m_adbDeviceList[i];
                emit deviceDisconnected(disconnectingDevice);
                NotificationController::instance()->send("adb设备已断开", QString(disconnectingDevice->code() + "已断开"));
                
                // 如果断开的是当前设备，需要更新cutADBDevice
                if (cutADBDevice() == disconnectingDevice.get()) {
                    if (m_adbDeviceList.count() > 1) {
                        // 有其他设备，切换到第一个不是当前设备的设备
                        for (const auto &device : m_adbDeviceList) {
                            if (device.get() != disconnectingDevice.get()) {
                                setcutADBDevice(device.get());
                                break;
                            }
                        }
                    } else {
                        setcutADBDevice(nullptr);
                    }
                }
                
                m_adbDeviceList.remove(i);
                continue;
            }
        }
    }
    
    if (enableFastbootCheck()) {
        QVector<QString> fastBootDevices = getDeviceList(C_Fastboot);
        for (QString &deviceCode : fastBootDevices) {
            if (!deviceCode.isEmpty() && !hasDevice(deviceCode, C_Fastboot)) {
                auto &&device = addDevice(deviceCode, C_Fastboot);
                emit deviceConnected(device);
                NotificationController::instance()->send("发现设备通过fastboot连接", QString(device->code() + "已连接"));
                
                // 如果是第一个Fastboot设备，设置为当前设备
                if (!cutFastbootDevice()) {
                    auto fastbootDevice = device.dynamicCast<FastbootDevice>();
                    if (fastbootDevice) {
                        setcutFastbootDevice(fastbootDevice.get());
                    }
                }
            }
        }

        // 检查是否有设备断开连接(Fastboot)
        for (int i = m_fastbootDeviceList.size() - 1; i >= 0; i--) {
            // 检查是否有设备断开连接
            bool deviceFound = false;
            for (const auto &deviceCode : fastBootDevices) {
                if (m_fastbootDeviceList[i]->code() == deviceCode) {
                    deviceFound = true;
                    break;
                }
            }
            if (!deviceFound) {
                auto disconnectingDevice = m_fastbootDeviceList[i];
                NotificationController::instance()->send("fastboot设备已断开", QString(disconnectingDevice->code() + "已断开"));
                emit deviceDisconnected(disconnectingDevice);
                
                // 如果断开的是当前设备，需要更新cutFastbootDevice
                if (cutFastbootDevice() == disconnectingDevice.get()) {
                    if (m_fastbootDeviceList.count() > 1) {
                        // 有其他设备，切换到第一个不是当前设备的设备
                        for (const auto &device : m_fastbootDeviceList) {
                            if (device.get() != disconnectingDevice.get()) {
                                setcutFastbootDevice(device.get());
                                break;
                            }
                        }
                    } else {
                        setcutFastbootDevice(nullptr);
                    }
                }
                
                m_fastbootDeviceList.remove(i);
                continue;
            }
        }
    }

    emit deviceRefreshFinish();
}

QVector<QString> ConnectManager::getDeviceList(ConnectStatus type)
{
    QVector<QString> deviceList;
    QStringList retStrList;
    
    if (type == C_ADB) {
        retStrList = ADBTOOL->executeCommand(ADBTools::ADB, {"devices"}).split('\n');
        for (QString &lineInfo : retStrList) {
            lineInfo = lineInfo.simplified();
            if (QStringList &&blockInfo = lineInfo.split(' '); blockInfo.size() == 2 && blockInfo.last() == "device") {
                QString deviceCode = blockInfo.first().simplified();
                deviceList.push_back(deviceCode);
            }
        }
    } else if (type == C_Fastboot) {
        retStrList = ADBTOOL->executeCommand(ADBTools::FASTBOOT, {"devices"}).split('\n');
        for (QString &lineInfo : retStrList) {
            lineInfo = lineInfo.simplified();
            if (QStringList &&blockInfo = lineInfo.split('\t'); blockInfo.size() == 2 && blockInfo.last() == "fastboot") {
                QString deviceCode = blockInfo.first().simplified();
                deviceList.push_back(deviceCode);
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
        device = QSharedPointer<ADBDevice>::create();
        device->setcode(deviceCode);
        m_adbDeviceList.append(device.staticCast<ADBDevice>());
    } else if (type == C_Fastboot) {
        device = QSharedPointer<FastbootDevice>::create();
        device->setcode(deviceCode);
        m_fastbootDeviceList.append(device.staticCast<FastbootDevice>());
    }
    
    device->setconnectStatus(type);
    device->setisConnected(true);
    return device;
}

} // namespace ADT