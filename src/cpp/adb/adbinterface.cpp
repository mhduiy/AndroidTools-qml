#include "adbinterface.h"
#include <QtConcurrent/QtConcurrent>
#include <QTimer>
#include "../utils/utils.hpp"
#include "../utils/Notification.h"

ADBInterface::ADBInterface(QObject *parent) : QObject(parent)
{

}

bool ADBInterface::startADBService()
{
    m_adbTools->executeCommand(ADBTools::ADB, {"start-server"});
    QStringList retStrList = m_adbTools->executeCommand(ADBTools::ADB, {"version"}).split('\n');
    for (QString &lineInfo : retStrList) {
        if (lineInfo.contains("Android Debug Bridge version")) {
            m_adbVersion = lineInfo.split(' ').last().simplified();
        }
    }
    emit adbStarted();
    return true;
}

bool ADBInterface::stopADBService()
{
    m_adbTools->executeCommand(ADBTools::ADB, {"kill-server"});
    emit adbKilled();
    return true;
}

bool ADBInterface::restartADBService()
{
    NotificationControl::instance()->send("重启中，请耐心等待", NotificationControl::Warning);
    stopADBService();
    startADBService();
    return true;
}

QString ADBInterface::adbVersion() const
{
    return m_adbVersion;
}

QVector<QString> ADBInterface::getDeviceCodes() const
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

DeviceBatteryInfo ADBInterface::getBatteryInfo(const QString &code) const
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

DeviceCutActivityInfo ADBInterface::getCutActivityInfo(const QString &code) const
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
                deviceCutActivity.windowCode = blockInfoList.value(0).right(blockInfoList.value(0).size() - 1);
                deviceCutActivity.cutPackage = blockInfoList.value(2).split('/').first();
                deviceCutActivity.cutActivity = blockInfoList.value(2).split('/').last();
                return deviceCutActivity;
            }
        }
    }
    return {};
}

DeviceDetailInfo ADBInterface::getDeviceDetailInfo(const QString &code)
{
    DeviceDetailInfo info;
    /*厂商*/
    QString ret;
    info.manufacturer = getDeviceProp(code, "ro.product.manufacturer");
    /*品牌*/
    info.brand = getDeviceProp(code, "ro.product.brand");
    /*型号*/
    info.model = getDeviceProp(code, "ro.product.model");
    /*设备名*/
    info.deviceName = getDeviceProp(code, "ro.product.marketname");
    /*设备代号*/
    info.deviceCode = getDeviceProp(code, "ro.product.name");
    /*系统信息*/
    info.systemInfo = getDeviceProp(code, "ro.custom.version");
    /*安卓版本*/
    info.androidVersion = getDeviceProp(code, "ro.build.version.release");
    /*分辨率*/
    info.resolving = m_adbTools->executeCommand(ADBTools::ADB, {"-s", code, "shell", "wm size"}).simplified().split(' ').last();
    /*Dpi*/
    info.dpi = getDeviceProp(code, "ro.sf.lcd_density");
    /*MAC地址*/
    info.macAddr = "";
    /*IP地址*/
    info.ipAddr = "";
    /*CPU*/
    info.cpuInfo = getDeviceProp(code, "ro.product.board");
    /*内存容量*/
    info.memory = "";
    /*SDK版本*/
    info.sdkVersion = getDeviceProp(code, "ro.build.version.sdk");
    /*序列号*/
    ret = getDeviceProp(code, "ro.serialno").simplified();
    info.serialNumber = ret.split(' ').value(0);

    return info;
}

void ADBInterface::killActivity(const QString &packageName, const QString &deviceCode)
{
    m_adbTools->executeCommand(ADBTools::ADB, {"-s", deviceCode, "shell", "am", "force-stop", packageName});
}

QString ADBInterface::getDeviceProp(const QString &deviceCode, const QString &prop)
{
    return m_adbTools->executeCommand(ADBTools::ADB, {"-s", deviceCode, "shell", "getprop", prop}).simplified();
}

void ADBInterface::pushKey(const QString &key, const QString &deviceCode)
{
    qWarning() << key;
    m_adbTools->executeCommand(ADBTools::ADB, {"-s", deviceCode, "shell", "input", "keyevent", key});
}
