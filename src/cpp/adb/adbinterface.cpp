#include "adbinterface.h"
#include <QtConcurrent/QtConcurrent>
#include <QTimer>
#include "../utils/utils.hpp"

ADBInterface::ADBInterface(QObject *parent) : QObject(parent)
{

}

bool ADBInterface::startADBService()
{
    m_adbTools->executeCommand(ADBTools::ADB, {"start-server"});
    return true;
}

bool ADBInterface::stopADBService()
{
    m_adbTools->executeCommand(ADBTools::ADB, {"kill-server"});
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
    QStringList retStrList = m_adbTools->executeCommand(ADBTools::ADB, {"version"}).split('\n');
    for (QString &lineInfo : retStrList) {
        if (lineInfo.contains("Android Debug Bridge version")) {
            return lineInfo.split(' ').last().simplified();
        }
    }
    return {};
}

QVector<QString> ADBInterface::getDeviceCodes()
{
    QVector<QString> devices;
    QStringList retStrList = m_adbTools->executeCommand(ADBTools::ADB, {"devices"}).split('\n');
    // 检查现有设备连接
    for (QString &lineInfo : retStrList) {
        lineInfo = lineInfo.simplified();
        if (QStringList &&blockInfo = lineInfo.split(' '); blockInfo.size() == 2 && blockInfo.last() == "device") {
            QString deviceCode = blockInfo.first().simplified();
            devices.push_back(deviceCode);
        }
    }
    return devices;
}

DeviceBatteryInfo ADBInterface::getBatteryInfo(const QString &code)
{
    QString retStr = m_adbTools->executeCommand(ADBTools::ADB, {"-s", code, "shell", "dumpsys battery"});
    QMap<QString, QString> retMap = serializationInformation(retStr);
    DeviceBatteryInfo batteryInfo;
    if (retMap["AC powered"] == "true") {
        batteryInfo.chargingType = AC;
    } else if (retMap["USB powered"] == "true") {
        batteryInfo.chargingType = USB;
    } else if (retMap["Wireless powered"] == "true") {
        batteryInfo.chargingType = Wireless;
    } else if (retMap["Dock powered"] == "true") {
        batteryInfo.chargingType = Dock;
    } else {
        batteryInfo.chargingType = None;
    }
    batteryInfo.maxChargingCut = retMap["Max charging current"].toInt() / 1000;
    batteryInfo.maxChargingVol = retMap["Max charging voltage"].toInt() / 1000;
    batteryInfo.chargingCounter = retMap["Charge counter"].toInt() / 1000;
    batteryInfo.status = retMap["status"].toInt();
    batteryInfo.health = retMap["health"].toInt();
    batteryInfo.level = retMap["level"].toInt();
    batteryInfo.scale = retMap["scale"].toInt();
    batteryInfo.voltage = retMap["voltage"].toInt();
    batteryInfo.temperature = retMap["temperature"].toDouble() / 10.0;
    return batteryInfo;
}

DeviceCutActivityInfo ADBInterface::getCutActivityInfo(const QString &code)
{
    QStringList retStrList = m_adbTools->executeCommand(ADBTools::ADB, {"-s", code, "shell", "dumpsys activity activities | grep topResumedActivity"}).split('\n');
    DeviceCutActivityInfo deviceCutActivity;
    for (QString &lineInfo : retStrList) {
        if (lineInfo.contains("topResumedActivity")) {
            lineInfo = lineInfo.simplified();
            int index = lineInfo.indexOf('{');
            if (lineInfo.isEmpty() || index < 0) continue;
            QStringList blockInfoList = lineInfo.mid(index, lineInfo.size() - index - 1).split(' ');
            if (blockInfoList.size() == 4) {
                deviceCutActivity.windowCode = blockInfoList.value(0);
                deviceCutActivity.cutPackage = blockInfoList.value(2).split('/').first();
                deviceCutActivity.cutActivity = blockInfoList.value(2).split('/').last();
                return deviceCutActivity;
            }
        }
    }
    return {};
}
