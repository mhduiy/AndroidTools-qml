#include "fastbootdevicemanager.h"
#include <QDebug>
#include "../utils/utils.hpp"
#include "src/cpp/utils/notificationcontroller.h"

namespace ADT {

DeviceCheckTool::DeviceCheckTool(QObject *parent)
    : QObject (parent)
    , m_checkTimer(new QTimer())
    , m_adbTool (ADBTools::instance())
{
    m_checkTimer->setInterval(1500);
    m_checkTimer->setSingleShot(true);
    connect(m_checkTimer, &QTimer::timeout, this, &DeviceCheckTool::checkDevice);
}

void DeviceCheckTool::startCheck()
{
    m_checkTimer->start();
}

void DeviceCheckTool::stopCheck()
{
    m_checkTimer->stop();
}

QStringList DeviceCheckTool::getDevices()
{
    return m_devices;
}

void DeviceCheckTool::checkDevice()
{
    m_devices.clear();
    QStringList args;
    args << "devices";

    QStringList retInfos = m_adbTool->executeCommand(ADBTools::FASTBOOT, args).split('\n');
    for (QString &lineInfo : retInfos) {
        lineInfo = lineInfo.simplified();
        if (const QStringList &deviceInfoList = lineInfo.split(' '); deviceInfoList.last() == "fastboot") {
            m_devices.append(deviceInfoList.first());
            // 暂时仅支持单设备
            break;
        }
    }
    emit checkFinish();
}

FastBootDeviceManager::FastBootDeviceManager(QObject *parent)
    : QObject(parent)
    , m_deviceCheckThread(new QThread(this))
    , m_deviceCheckTool(new DeviceCheckTool())
{
    m_deviceCheckTool->moveToThread(m_deviceCheckThread);
    m_deviceCheckThread->start();
    m_deviceCheckTool->startCheck();

    connect(m_deviceCheckTool, &DeviceCheckTool::checkFinish, this, &FastBootDeviceManager::handleDeviceChanged);
}

FastBootDeviceManager::~FastBootDeviceManager()
{
    qInfo() << "FastBootDeviceManager Thread exiting";
    m_deviceCheckThread->quit();
    m_deviceCheckThread->wait();
    qInfo() << "FastBootDeviceManager Thread exited";
}

QString FastBootDeviceManager::currentDeviceCode()
{
    return m_currentDeviceCode;
}

void FastBootDeviceManager::handleDeviceChanged()
{
    auto deviceInfos = m_deviceCheckTool->getDevices();
    if (deviceInfos.size() > 0 && !deviceInfos.first().isEmpty()) {
        m_currentDeviceCode = deviceInfos.first();
        emit currentDeviceChanged(m_currentDeviceCode);
    }
}

void FastBootDeviceManager::updateDevices()
{
    m_deviceCheckTool->startCheck();
}

void FastBootDeviceManager::rebootToSystem(const QString &deviceCode)
{
    runFastboot(deviceCode, {"reboot"});
}

void FastBootDeviceManager::powerOff(const QString &deviceCode)
{
    runFastboot(deviceCode, {"poweroff"});
}

void FastBootDeviceManager::runFastboot(const QString &deviceCode, const QStringList &args)
{
    if (m_currentDeviceCode.isEmpty()) {
        NotificationController::instance()->send("执行失败", "当前无设备连接", NotificationController::Warning, 3000);
        return;
    }
    const QString tarDevice = deviceCode.isEmpty() ? m_currentDeviceCode : deviceCode;

    QStringList fullArgs;
    fullArgs << "-s" << tarDevice << args;

    asyncOperator([fullArgs](){
        ADBTools::instance()->executeCommand(ADBTools::FASTBOOT, fullArgs);
    });
}

} // namespace ADT
