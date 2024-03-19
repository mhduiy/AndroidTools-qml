#include "adbinterface.h"
#include <QtConcurrent/QtConcurrent>
#include <QTimer>
#include "../utils/utils.hpp"

ADBInterface::ADBInterface(QObject *parent)
{
    m_deviceCheckTimer = new QTimer;
    m_deviceCheckTimer->setInterval(1500);
    connect(m_deviceCheckTimer, &QTimer::timeout, this, &ADBInterface::refreshDeviceStatus);

    connect(this, &ADBInterface::deviceConnected, [](QString code){
        qWarning() << code << " 已连接";
    });
    connect(this, &ADBInterface::deviceDisconnected, [](QString code){
        qWarning() << code << " 已断开";
    });
}

bool ADBInterface::startADBService()
{
    m_adbtools->executeCommand(ADBTools::ADB, {"start-server"});
    return true;
}

bool ADBInterface::stopADBService()
{
    m_adbtools->executeCommand(ADBTools::ADB, {"kill-server"});
    return true;
}

bool ADBInterface::restartADBService()
{
    stopADBService();
    startADBService();
    return true;
}

QString ADBInterface::adbVersion()
{
    QStringList retStrList = m_adbtools->executeCommand(ADBTools::ADB, {"version"}).split('\n');
    for (QString &lineInfo : retStrList) {
        if (lineInfo.contains("Android Debug Bridge version")) {
            return lineInfo.split(' ').last().simplified();
        }
    }
    return {};
}

void ADBInterface::refreshDeviceStatus()
{
    qWarning() << "检测中：" << QThread::currentThreadId();
    QVector<QString> devices;

    QStringList retStrList = m_adbtools->executeCommand(ADBTools::ADB, {"devices"}).split('\n');
    // 检查现有设备连接
    for (QString &lineInfo : retStrList) {
        lineInfo = lineInfo.simplified();
        if (QStringList &&blockInfo = lineInfo.split(' '); blockInfo.size() == 2 && blockInfo.last() == "device") {
            QString deviceCode = blockInfo.first().simplified();
            devices.push_back(deviceCode);
            // 如果是一个新设备
            if (!deviceCode.isEmpty() && !m_deviceCodeSet.contains(deviceCode)) {
                m_deviceCodeSet.push_back(deviceCode);
                emit deviceConnected(deviceCode);
            }
        }
    }

    // 清除原有内存

    for (auto it = m_deviceBatteryInfoMap.begin(); it != m_deviceBatteryInfoMap.end(); ++it) {
        delete it.value();
    }

    m_deviceBatteryInfoMap.clear();

    for (auto it = m_deviceCutActivityInfoMap.begin(); it != m_deviceCutActivityInfoMap.end(); ++it) {
        delete it.value();
    }

    m_deviceCutActivityInfoMap.clear();

    for (auto it = m_deviceBaceInfoMap.begin(); it != m_deviceBaceInfoMap.end(); ++it) {
        delete it.value();
    }

    m_deviceBaceInfoMap.clear();

    for (int i = m_deviceCodeSet.size() - 1; i >= 0; i--) {
        // 检查是否有设备断开连接
        if (!devices.contains(m_deviceCodeSet[i])) {
            emit deviceDisconnected(m_deviceCodeSet[i]);
            m_deviceCodeSet.remove(i);
            continue;
        }

        QString retStr = m_adbtools->executeCommand(ADBTools::ADB, {"-s", m_deviceCodeSet[i], "shell", "dumpsys battery"});
        QMap<QString, QString> retMap = serializationInformation(retStr);
        DeviceBatteryInfo *batteryInfo = new DeviceBatteryInfo();
        if (retMap["AC powered"] == "true") {
            batteryInfo->chargingType = AC;
        } else if (retMap["USB powered"] == "true") {
            batteryInfo->chargingType = USB;
        } else if (retMap["Wireless powered"] == "true") {
            batteryInfo->chargingType = Wireless;
        } else if (retMap[" Dock powered"] == "true") {
            batteryInfo->chargingType = Dock;
        }
        batteryInfo->maxChargingCut = retMap["Max charging current"].toInt() / 1000;
        batteryInfo->maxChargingVol = retMap["Max charging voltage"].toInt() / 1000;
        batteryInfo->chargingCounter = retMap["Charge counter"].toInt() / 1000;
        batteryInfo->status = retMap["status"].toInt();
        batteryInfo->health = retMap["health"].toInt();
        batteryInfo->level = retMap["level"].toInt();
        batteryInfo->scale = retMap["scale"].toInt();
        batteryInfo->voltage = retMap["voltage"].toInt();
        batteryInfo->temperature = retMap["temperature"].toDouble() / 10.0;

        m_deviceBatteryInfoMap.insert(m_deviceCodeSet[i], batteryInfo);

        QStringList retStrList = m_adbtools->executeCommand(ADBTools::ADB, {"-s", m_deviceCodeSet[i], "shell", "dumpsys activity activities | grep topResumedActivity"}).split('\n');
        for (QString &lineInfo : retStrList) {
            qWarning() << lineInfo;
            if (lineInfo.contains("topResumedActivity")) {
                lineInfo = lineInfo.simplified();
                int index = lineInfo.indexOf('{');
                if (lineInfo.isEmpty() || index < 0) continue;
                qWarning() << "*****" << index << " " << lineInfo.size() - index - 1 << " " << lineInfo.size();
                QStringList blockInfoList = lineInfo.mid(index, lineInfo.size() - index - 1).split(' ');
                if (blockInfoList.size() == 4) {
                    DeviceCutActivityInfo *deviceCutActivity = new DeviceCutActivityInfo();
                    deviceCutActivity->windowCode = blockInfoList.value(0);
                    deviceCutActivity->cutPackage = blockInfoList.value(2).split('/').first();
                    deviceCutActivity->cutActivity = blockInfoList.value(2).split('/').last();

                    m_deviceCutActivityInfoMap.insert(m_deviceCodeSet[i], deviceCutActivity);
                    break;
                }
            }
        }
        DeviceBaceInfo *deviceBaceInfo = new DeviceBaceInfo();

        retStr = m_adbtools->executeCommand(ADBTools::ADB, {"-s", m_deviceCodeSet[i], "shell",  R"(getprop ro.product.marketname)"});
        deviceBaceInfo->deviceName = retStr.simplified();
        deviceBaceInfo->battery = m_deviceBatteryInfoMap[m_deviceCodeSet[i]]->level;
        deviceBaceInfo->isConnected = true;
        deviceBaceInfo->isWireless = false;
        deviceBaceInfo->isCharging = m_deviceBatteryInfoMap[m_deviceCodeSet[i]]->status == 2 ? true : false;

        m_deviceBaceInfoMap.insert(m_deviceCodeSet[i], deviceBaceInfo);
    }

    for (auto it = m_deviceBatteryInfoMap.begin(); it != m_deviceBatteryInfoMap.end(); ++it) {
        qWarning() << it.key() << "*1*" << it.value()->level;
    }

    for (auto it = m_deviceCutActivityInfoMap.begin(); it != m_deviceCutActivityInfoMap.end(); ++it) {
        qWarning() << it.key() << "*2*" << it.value()->cutActivity;
    }

    for (auto it = m_deviceBaceInfoMap.begin(); it != m_deviceBaceInfoMap.end(); ++it) {
        qWarning() << it.key() << "*3*" << it.value()->deviceName;
    }
}

void ADBInterface::startRefreshDevice()
{
    m_deviceCheckTimer->start();
}

void ADBInterface::stopRefreshDevice()
{
    m_deviceCheckTimer->stop();
}

DeviceBaceInfo *ADBInterface::getDeviceBaceInfo(const QString &code)
{
    return m_deviceBaceInfoMap.value(code);
}

DeviceBatteryInfo *ADBInterface::getDeviceBatteryInfo(const QString &code)
{
    return m_deviceBatteryInfoMap.value(code);
}

DeviceCutActivityInfo *ADBInterface::getDeviceCutActivityInfo(const QString &code)
{
    return m_deviceCutActivityInfoMap.value(code);
}
