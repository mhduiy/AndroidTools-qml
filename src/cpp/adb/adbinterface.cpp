#include "adbinterface.h"
#include <QtConcurrent/QtConcurrent>
#include <QTimer>
#include <qdatetime.h>
#include <qlogging.h>
#include <QDesktopServices>
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
    m_adbTools->executeCommand(ADBTools::ADB, {"-s", deviceCode, "shell", "input", "keyevent", key});
}

void ADBInterface::startActivity(const QString &deviceCode, const QString &activity, const QStringList &args)
{
    QStringList adbArgs;
    adbArgs << "-s" << deviceCode << "shell" << "am" << "start" << "-n" << activity << args;
    QString ret = m_adbTools->executeCommand(ADBTools::ADB, adbArgs);
    NotificationControl::instance()->send("命令已执行，可能没有成功，请确认", NotificationControl::Info);
}

QList<AppListInfo> ADBInterface::getSoftListInfo(const QString &deviceCode)
{
    QStringList args;
    QList<AppListInfo> res;
    args << "-s" << deviceCode << "shell" << "pm" << "list" << "packages" << "-3" <<  "--show-versioncode";
    QStringList retList = m_adbTools->executeCommand(ADBTools::ADB, args).split('\n');
    int packageLen = QString("package:").length();
    int versionCodeLen = QString("versionCode:").length();
    for (const QString &lineInfo : retList) {
        QStringList &&infos = lineInfo.simplified().split(' ');
        if (infos.size() == 2) {
            AppListInfo appInfo;
            if (auto &packageNameInfo = infos.first(); packageNameInfo.size() > packageLen) {
                appInfo.packageName = packageNameInfo.remove(0, packageLen);
            }
            if (auto &versionCodeInfo = infos.last(); versionCodeInfo.size() > versionCodeLen) {
                appInfo.versionCode = versionCodeInfo.remove(0, versionCodeLen);
            }
            appInfo.state = AppState::APP_Enable;
            res.append(appInfo);
        }
    }

    args.clear();
    args << "-s" << deviceCode << "shell" << "pm" << "list" << "packages" <<  "-d";
    retList = m_adbTools->executeCommand(ADBTools::ADB, args).split('\n');
    for (const QString &lineInfo : retList) {
        QString &&info = lineInfo.simplified();
        QString disablePackageName;
        if (auto &packageNameInfo = info; packageNameInfo.size() > packageLen) {
            disablePackageName = packageNameInfo.remove(0, packageLen);
        }
        if (!disablePackageName.isEmpty()) {
            auto findIt = std::find_if(res.begin(), res.end(), [&disablePackageName](const AppListInfo &info){
                return info.packageName == disablePackageName;
            });
            if(findIt != res.end()) {
                (*findIt).state = AppState::APP_Disable;
            }
        }
    }
    return res;
}

AppDetailInfo ADBInterface::getAppDetailInfo(const QString &deviceCode, const QString &packageName)
{
    QStringList args;
    args << "-s" << deviceCode << "shell" << "dumpsys" << "package" << packageName;
    const QStringList retInfo = m_adbTools->executeCommand(ADBTools::ADB, args).split('\n');
    AppDetailInfo resInfo;
    for (QString lineInfo : retInfo) {
        lineInfo = lineInfo.simplified();
        if (lineInfo.startsWith("firstInstallTime", Qt::CaseInsensitive)) {
            resInfo.installDate = lineInfo.split('=').last();
            continue;
        } else if (lineInfo.startsWith("versionName", Qt::CaseInsensitive)) {
            resInfo.versionName = lineInfo.split('=').last();
            qWarning() << resInfo.versionName << lineInfo;
            continue;
        } else if (lineInfo.startsWith("installerPackageName", Qt::CaseInsensitive)) {
            resInfo.installUser = lineInfo.split('=').last();
            continue;
        } else if (lineInfo.startsWith("appId", Qt::CaseInsensitive)) {
            resInfo.appid = lineInfo.split('=').last();
            continue;
        } else if (lineInfo.startsWith("versionCode", Qt::CaseInsensitive)) {
            const QStringList infoList = lineInfo.split(' ');
            resInfo.minsdk = infoList.value(1).split('=').last();
            resInfo.targetsdk = infoList.value(2).split('=').last();
            continue;
        }
    }

    args.clear();
    args << "-s" << deviceCode << "shell" << "pm" << "path" << packageName;
    auto apkPathInfo = m_adbTools->executeCommand(ADBTools::ADB, args).simplified().split(':');
    resInfo.path = apkPathInfo.value(1);

    resInfo.packageName = packageName;
    return resInfo;
}


bool ADBInterface::installApp(const QString &deviceCode, const QString &path)
{
    QStringList args;
    args << "-s" << deviceCode << "install" << path;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return true;
}

bool ADBInterface::clearData(const QString &deviceCode, const QString &packageName)
{
    QStringList args;
    args << "-s" << deviceCode << "shell" << "pm" << "clear" << packageName;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return true;
}

bool ADBInterface::unfreezeApp(const QString &deviceCode, const QString &packageName)
{
    QStringList args;
    args << "-s" << deviceCode << "shell" << "pm" << "enable" << packageName;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return true;
}

bool ADBInterface::freezeApp(const QString &deviceCode, const QString &packageName)
{
    QStringList args;
    args << "-s" << deviceCode << "shell" << "pm" << "disable-user" << packageName;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return true;
}

bool ADBInterface::uninstallApp(const QString &deviceCode, const QString &packageName)
{
    QStringList args;
    args << "-s" << deviceCode << "uninstall" << packageName;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return true;
}

bool ADBInterface::extractApp(const QString &deviceCode, const QString &packagePath, const QString &targetPath)
{
    QStringList args;
    args << "-s" << deviceCode << "pull" << packagePath << targetPath;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return true;
}

void ADBInterface::startApp(const QString &deviceCode, const QString &packageName)
{
    QStringList args;
    args << "-s" << deviceCode << "shell" << "monkey" << "-p" << packageName << "-c" << "android.intent.category.LAUNCHER" << "1";
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}


void ADBInterface::shotScreen(const QString &deviceCode, const QString &outPath)
{
    QString filePath = outPath + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
    QStringList args;
    args << "-s" << deviceCode << "exec-out" << "screencap" << "-p";

    QProcess process;
    process.start("adb", args);
    if (process.waitForFinished()) {
        QByteArray output = process.readAllStandardOutput();
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(output);
            file.close();
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        } else {
            qWarning() << "Failed to open file for writing:" << filePath;
        }
    } else {
        qWarning() << "ADB command failed to finish";
    }
}

void ADBInterface::disConnectDeivce(const QString &deviceCode)
{
    QStringList args;
    args << "disconnect" << deviceCode;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}