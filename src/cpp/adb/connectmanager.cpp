#include "connectmanager.h"
#include <QtQml>
#include <QTimer>
#include "../utils/notificationcontroller.h"

ConnectManager::ConnectManager(QObject *parent) : QObject(parent)
{
    m_deviceListviewModel = new DeviceListviewModel();
    qmlRegisterSingletonInstance("DeviceListviewModel", 1, 0, "DeviceListviewModel", m_deviceListviewModel);

    m_deviceCheckTimer = new QTimer();
    m_deviceCheckTimer->setInterval(1500);
    connect(m_deviceCheckTimer, &QTimer::timeout, this, &ConnectManager::refreshDevice);

    connect(this, &ConnectManager::deviceConnected, [](QString code){
        qInfo() << code << " connected";
        NotificationController::instance()->send("发现设备", QString(code + "已连接"));
    });
    connect(this, &ConnectManager::deviceDisconnected, [](QString code){
        qInfo() << code << " disconnected";
        NotificationController::instance()->send("设备断开", QString(code + "已断开"), NotificationController::Warning);
    });

    // TODO 可能导致异常
    connect(m_deviceListviewModel, &DeviceListviewModel::currentItemChanged, this, &ConnectManager::setCurrentDeviceCode, Qt::DirectConnection);
}

QString ConnectManager::currentDeviceCode()
{
    return m_currentDeviceCode;
}

bool ConnectManager::setCurrentDeviceCode(const QString &code)
{
    if (code.isEmpty()) {
        m_currentDeviceCode = "";
        emit currentDeviceChanged(code);
        qInfo() << "set current device: " << "Empty";
        return true;
    }
    if (m_deviceCodeSet.contains(code) && m_currentDeviceCode != code) {
        m_currentDeviceCode = code;
        emit currentDeviceChanged(code);
        qInfo() << "set current device: " << code;
        return true;
    }
    qInfo() << "set current failed: " << code;
    return false;
}

void ConnectManager::startCheckDevice()
{
    m_deviceCheckTimer->start();
}

void ConnectManager::stopCheckDevice()
{
    m_deviceCheckTimer->stop();
    qInfo() << "等待ADB执行结束";
    while(ADBTools::instance()->isRunning());
    qInfo() << "ADB执行结束";
}

DeviceBaceInfo ConnectManager::getDeviceBaceInfo(const QString &code)
{
    return m_deviceBaceInfoMap.value(code.isEmpty() ? m_currentDeviceCode : code);
}

DeviceBatteryInfo ConnectManager::getDeviceBatteryInfo(const QString &code)
{
    return m_deviceBatteryInfoMap.value(code.isEmpty() ? m_currentDeviceCode : code);
}

DeviceCutActivityInfo ConnectManager::getDeviceCutActivityInfo(const QString &code)
{
    return m_deviceCutActivityInfoMap.value(code.isEmpty() ? m_currentDeviceCode : code);
}

void ConnectManager::refreshDevice()
{
    QVector<QString> devices = m_adbInterface->getDeviceCodes();

    // 检查是否有新设备连接
    for (QString &deviceCode : devices) {
        if (!deviceCode.isEmpty() && !m_deviceCodeSet.contains(deviceCode)) {
            m_deviceCodeSet.push_back(deviceCode);
            emit deviceConnected(deviceCode);
            if (deviceCode == m_currentDeviceCode) {
                emit currentDeviceChanged("");
            }
        }
    }

    m_deviceBatteryInfoMap.clear();
    m_deviceCutActivityInfoMap.clear();
    m_deviceBaceInfoMap.clear();

    for (int i = m_deviceCodeSet.size() - 1; i >= 0; i--) {
        // 检查是否有设备断开连接
        if (!devices.contains(m_deviceCodeSet[i])) {
            m_deviceListviewModel->removeRow(m_deviceCodeSet[i]);
            emit deviceDisconnected(m_deviceCodeSet[i]);
            m_deviceCodeSet.remove(i);
            if (m_deviceCodeSet.count()) {
                setCurrentDeviceCode(m_deviceCodeSet.first());
            } else {
                setCurrentDeviceCode("");
            }
            continue;
        }

        auto &&batteryInfo = m_adbInterface->getBatteryInfo(m_deviceCodeSet[i]);
        m_deviceBatteryInfoMap.insert(m_deviceCodeSet[i], batteryInfo);

        auto &&cutActivityInfo = m_adbInterface->getCutActivityInfo(m_deviceCodeSet[i]);
        m_deviceCutActivityInfoMap.insert(m_deviceCodeSet[i], cutActivityInfo);

        DeviceBaceInfo deviceBaceInfo;
        QString retStr = m_adbTools->executeCommand(ADBTools::ADB, {"-s", m_deviceCodeSet[i], "shell",  R"(getprop ro.product.marketname)"}).simplified();
        if (retStr.isEmpty()) { // 兼容部分荣耀
            retStr = m_adbTools->executeCommand(ADBTools::ADB, {"-s", m_deviceCodeSet[i], "shell",  R"(getprop ro.config.marketing_name)"}).simplified();
        }
        if (retStr.isEmpty()) {
            retStr = m_adbTools->executeCommand(ADBTools::ADB, {"-s", m_deviceCodeSet[i], "shell", "getprop", "ro.product.model"}).simplified();
        }
        deviceBaceInfo.deviceName = retStr;
        deviceBaceInfo.battery = m_deviceBatteryInfoMap[m_deviceCodeSet[i]].level;
        deviceBaceInfo.isConnected = true;
        deviceBaceInfo.isWireless = m_deviceCodeSet[i].contains(':');
        if (deviceBaceInfo.isWireless) {
            deviceBaceInfo.ip = m_deviceCodeSet[i].split(':').first();
        }
        deviceBaceInfo.isCharging = m_deviceBatteryInfoMap[m_deviceCodeSet[i]].status == 2 ? true : false;
        deviceBaceInfo.deviceCode = m_deviceCodeSet[i];

        m_deviceBaceInfoMap.insert(m_deviceCodeSet[i], deviceBaceInfo);

        if (m_deviceListviewModel->hasDeviceCode(m_deviceCodeSet[i])) {
            m_deviceListviewModel->setInfo(m_deviceBaceInfoMap.value(m_deviceCodeSet[i]));
        } else { // 是一个新设备
            m_deviceListviewModel->appendRow(m_deviceBaceInfoMap.value(m_deviceCodeSet[i]));
        }
    }

    emit deviceRefreshFinish();
#if 0
    for (auto it = m_deviceBatteryInfoMap.begin(); it != m_deviceBatteryInfoMap.end(); ++it) {
        qWarning() << it.key() << "*1*" << it.value()->level;
    }

    for (auto it = m_deviceCutActivityInfoMap.begin(); it != m_deviceCutActivityInfoMap.end(); ++it) {
        qWarning() << it.key() << "*2*" << it.value()->cutActivity;
    }

    for (auto it = m_deviceBaceInfoMap.begin(); it != m_deviceBaceInfoMap.end(); ++it) {
        qWarning() << it.key() << "*3*" << it.value()->deviceName;
    }
#endif
}
