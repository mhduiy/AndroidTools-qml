#include "fastbootdevicemanager.h"
#include <QDebug>
#include "../utils/utils.hpp"

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
    qWarning() << retInfos;
    for (QString &lineInfo : retInfos) {
        lineInfo = lineInfo.simplified();
        if (const QStringList &deviceInfoList = lineInfo.split(' '); deviceInfoList.last() == "device") {
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

void FastBootDeviceManager::setCurrentDevice(const QString &deviceCode)
{
    Q_UNUSED(deviceCode)
}

void FastBootDeviceManager::setCurrentDevice(int deviceIndex)
{
    Q_UNUSED(deviceIndex)
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
    qWarning() << "刷新设备1";
    m_deviceCheckTool->startCheck();
}

void FastBootDeviceManager::rebootToFastBoot(const QString &deviceCode)
{
    QString tarDevice = deviceCode;
    if (deviceCode.isEmpty()) {
        tarDevice = m_currentDeviceCode;
    }

    QStringList args;
    args << "reboot" << "fastboot";

    auto operatorFunc = [&tarDevice, &args](){
        ADBTools::instance()->executeCommand(ADBTools::FASTBOOT, args);
    };

    asyncOperator(operatorFunc);
}

void FastBootDeviceManager::rebootToSystem(const QString &deviceCode)
{
    QString tarDevice = deviceCode;
    if (deviceCode.isEmpty()) {
        tarDevice = m_currentDeviceCode;
    }

    QStringList args;
    args << "reboot";

    auto operatorFunc = [&tarDevice, &args](){
        ADBTools::instance()->executeCommand(ADBTools::FASTBOOT, args);
    };

    asyncOperator(operatorFunc);
}

void FastBootDeviceManager::rebootToRecovery(const QString &deviceCode)
{
    QString tarDevice = deviceCode;
    if (deviceCode.isEmpty()) {
        tarDevice = m_currentDeviceCode;
    }

    QStringList args;
    args << "reboot" << "recovery";

    auto operatorFunc = [&tarDevice, &args](){
        ADBTools::instance()->executeCommand(ADBTools::FASTBOOT, args);
    };

    asyncOperator(operatorFunc);
}

void FastBootDeviceManager::powerOff(const QString &deviceCode)
{
    QString tarDevice = deviceCode;
    if (deviceCode.isEmpty()) {
        tarDevice = m_currentDeviceCode;
    }

    QStringList args;
    args << "poweroff";

    auto operatorFunc = [&tarDevice, &args](){
        ADBTools::instance()->executeCommand(ADBTools::FASTBOOT, args);
    };

    asyncOperator(operatorFunc);
}
