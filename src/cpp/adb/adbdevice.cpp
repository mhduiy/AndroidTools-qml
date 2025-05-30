#include "adbdevice.h"
#include "../utils/utils.hpp"
#include <algorithm>
#include <QDebug>
#include <QProcess>
#include <QDesktopServices>
#include <QDateTime>
#include <QFile>
#include <QUrl>

ADBDevice::ADBDevice(QObject *parent)
    : Device(parent)
    , m_adbTools(ADBTools::instance())
    , m_fixInfoRefreshTimer(new QTimer(this))
    , m_realtimeRefreshTimer(new QTimer(this))
    , m_refreshThread(new QThread(this))
    , m_fixInfoRefreshed(false)
{
    qInfo() << "创建ADBDevice实例";
    
    // 初始化设备信息
    setdetailInfo(QSharedPointer<DeviceDetailInfo>::create());
    setbatteryInfo(QSharedPointer<DeviceBatteryInfo>::create());
    setcutActivityInfo(QSharedPointer<DeviceCutActivityInfo>::create());
    
    // 设置固定信息刷新定时器（只刷新一次）
    m_fixInfoRefreshTimer->setInterval(1000); // 1秒后刷新固定信息
    m_fixInfoRefreshTimer->setSingleShot(true);
    
    // 设置实时信息刷新定时器
    m_realtimeRefreshTimer->setInterval(3000); // 3秒刷新一次实时信息
    m_realtimeRefreshTimer->setSingleShot(false);
    
    // 将定时器移到子线程中运行
    m_fixInfoRefreshTimer->moveToThread(m_refreshThread);
    m_realtimeRefreshTimer->moveToThread(m_refreshThread);
    
    // 连接定时器信号
    connect(m_fixInfoRefreshTimer, &QTimer::timeout, this, &ADBDevice::onFixInfoRefreshTimeout, Qt::QueuedConnection);
    connect(m_realtimeRefreshTimer, &QTimer::timeout, this, &ADBDevice::onRealtimeRefreshTimeout, Qt::QueuedConnection);
    
    // 启动子线程
    m_refreshThread->start();
    qInfo() << "ADBDevice子线程已启动";
    
    // 启动自动刷新
    startAutoRefresh();
}

ADBDevice::~ADBDevice()
{
    qInfo() << "销毁ADBDevice实例";
    if (m_refreshThread) {
        m_fixInfoRefreshTimer->stop();
        m_realtimeRefreshTimer->stop();
        m_refreshThread->quit();
        m_refreshThread->wait();
        qInfo() << "ADBDevice子线程已停止";
    }
}

void ADBDevice::startAutoRefresh()
{
    qInfo() << "启动ADBDevice自动刷新";
    // 启动固定信息刷新定时器
    QMetaObject::invokeMethod(m_fixInfoRefreshTimer, "start", Qt::QueuedConnection);
    
    // 启动实时信息刷新定时器
    QMetaObject::invokeMethod(m_realtimeRefreshTimer, "start", Qt::QueuedConnection);
}

void ADBDevice::onFixInfoRefreshTimeout()
{
    if (!m_fixInfoRefreshed && !code().isEmpty()) {
        qInfo() << "开始刷新设备固定信息 - 设备代码:" << code();
        
        // 刷新固定信息
        DeviceDetailInfo info = refreshFixInfo();
        
        qInfo() << "设备固定信息刷新完成:";
        qInfo() << "  厂商:" << info.manufacturer;
        qInfo() << "  品牌:" << info.brand;
        qInfo() << "  型号:" << info.model;
        qInfo() << "  设备名:" << info.deviceName;
        qInfo() << "  Android版本:" << info.androidVersion;
        qInfo() << "  内存:" << info.memory;
        qInfo() << "  CPU:" << info.cpuInfo;
        
        // 更新设备详细信息到新属性
        updateDetailInfoFromStruct(info);
        
        // 更新旧的共享指针（保持兼容性）
        auto detailInfoPtr = detailInfo();
        if (detailInfoPtr) {
            *detailInfoPtr = info;
            emit detailInfoChanged(detailInfoPtr);
        }
        
        m_fixInfoRefreshed = true;
        qInfo() << "设备固定信息已设置完成";
    } else if (code().isEmpty()) {
        qWarning() << "设备代码为空，无法刷新固定信息";
    }
}

void ADBDevice::onRealtimeRefreshTimeout()
{
    if (code().isEmpty()) {
        qWarning() << "设备代码为空，跳过实时信息刷新";
        return;
    }
    
    qDebug() << "开始刷新设备实时信息 - 设备代码:" << code();
    
    // 刷新电池信息
    DeviceBatteryInfo batteryInfo = refreshBatteryInfo();
    updateBatteryInfoFromStruct(batteryInfo);
    
    // 更新旧的共享指针（保持兼容性）
    auto batteryInfoPtr = this->batteryInfo();
    if (batteryInfoPtr) {
        *batteryInfoPtr = batteryInfo;
        emit batteryInfoChanged(batteryInfoPtr);
    }
    
    qDebug() << "电池信息更新 - 电量:" << batteryInfo.level 
             << "% 充电状态:" << batteryInfo.chargingType 
             << " 温度:" << batteryInfo.temperature << "°C";
    
    // 刷新当前活动信息
    DeviceCutActivityInfo cutActivityInfo = refreshCutActivityInfo();
    updateActivityInfoFromStruct(cutActivityInfo);
    
    // 更新旧的共享指针（保持兼容性）
    auto cutActivityInfoPtr = this->cutActivityInfo();
    if (cutActivityInfoPtr) {
        *cutActivityInfoPtr = cutActivityInfo;
        emit cutActivityInfoChanged(cutActivityInfoPtr);
    }
    
    if (!cutActivityInfo.cutPackage.isEmpty()) {
        qDebug() << "当前活动更新 - 包名:" << cutActivityInfo.cutPackage 
                 << " 活动:" << cutActivityInfo.cutActivity;
    }
}

void ADBDevice::updateDetailInfoFromStruct(const DeviceDetailInfo &info)
{
    setManufacturer(info.manufacturer);
    setBrand(info.brand);
    setModel(info.model);
    setDeviceName(info.deviceName);
    setDeviceCode(info.deviceCode);
    setSystemInfo(info.systemInfo);
    setAndroidVersion(info.androidVersion);
    setResolution(info.resolving);  // 注意字段名mapping
    setDpi(info.dpi);
    setMacAddr(info.macAddr);
    setIpAddr(info.ipAddr);
    setCpuInfo(info.cpuInfo);
    setMemory(info.memory);
    setSdkVersion(info.sdkVersion);
    setSerialNumber(info.serialNumber);
    setMaxFreq(info.maxFrep);  // 注意字段名mapping
    setMaxCoreNum(info.maxCoreNum);
}

void ADBDevice::updateBatteryInfoFromStruct(const DeviceBatteryInfo &info)
{
    setChargingType(info.chargingType);
    setMaxChargingCut(info.maxChargingCut);
    setMaxChargingVol(info.maxChargingVol);
    setChargingCounter(info.chargingCounter);
    setBatteryStatus(info.status);
    setBatteryHealth(info.health);
    setBatteryLevel(info.level);
    setBatteryScale(info.scale);
    setBatteryVoltage(info.voltage);
    setBatteryCurrent(info.current);
    setBatteryTemperature(info.temperature);
}

void ADBDevice::updateActivityInfoFromStruct(const DeviceCutActivityInfo &info)
{
    setWindowCode(info.windowCode);
    setCurrentPackage(info.cutPackage);
    setCurrentActivity(info.cutActivity);
}

// === 设备详细信息访问器实现 ===
QString ADBDevice::manufacturer() const { return m_manufacturer; }
void ADBDevice::setManufacturer(const QString &val) { 
    if (m_manufacturer != val) { 
        m_manufacturer = val; 
        emit manufacturerChanged(val); 
    } 
}

QString ADBDevice::brand() const { return m_brand; }
void ADBDevice::setBrand(const QString &val) { 
    if (m_brand != val) { 
        m_brand = val; 
        emit brandChanged(val); 
    } 
}

QString ADBDevice::model() const { return m_model; }
void ADBDevice::setModel(const QString &val) { 
    if (m_model != val) { 
        m_model = val; 
        emit modelChanged(val); 
    } 
}

QString ADBDevice::deviceName() const { return m_deviceName; }
void ADBDevice::setDeviceName(const QString &val) { 
    if (m_deviceName != val) { 
        m_deviceName = val; 
        emit deviceNameChanged(val); 
    } 
}

QString ADBDevice::deviceCode() const { return m_deviceCode; }
void ADBDevice::setDeviceCode(const QString &val) { 
    if (m_deviceCode != val) { 
        m_deviceCode = val; 
        emit deviceCodeChanged(val); 
    } 
}

QString ADBDevice::systemInfo() const { return m_systemInfo; }
void ADBDevice::setSystemInfo(const QString &val) { 
    if (m_systemInfo != val) { 
        m_systemInfo = val; 
        emit systemInfoChanged(val); 
    } 
}

QString ADBDevice::androidVersion() const { return m_androidVersion; }
void ADBDevice::setAndroidVersion(const QString &val) { 
    if (m_androidVersion != val) { 
        m_androidVersion = val; 
        emit androidVersionChanged(val); 
    } 
}

QString ADBDevice::resolution() const { return m_resolution; }
void ADBDevice::setResolution(const QString &val) { 
    if (m_resolution != val) { 
        m_resolution = val; 
        emit resolutionChanged(val); 
    } 
}

QString ADBDevice::dpi() const { return m_dpi; }
void ADBDevice::setDpi(const QString &val) { 
    if (m_dpi != val) { 
        m_dpi = val; 
        emit dpiChanged(val); 
    } 
}

QString ADBDevice::macAddr() const { return m_macAddr; }
void ADBDevice::setMacAddr(const QString &val) { 
    if (m_macAddr != val) { 
        m_macAddr = val; 
        emit macAddrChanged(val); 
    } 
}

QString ADBDevice::ipAddr() const { return m_ipAddr; }
void ADBDevice::setIpAddr(const QString &val) { 
    if (m_ipAddr != val) { 
        m_ipAddr = val; 
        emit ipAddrChanged(val); 
    } 
}

QString ADBDevice::cpuInfo() const { return m_cpuInfo; }
void ADBDevice::setCpuInfo(const QString &val) { 
    if (m_cpuInfo != val) { 
        m_cpuInfo = val; 
        emit cpuInfoChanged(val); 
    } 
}

QString ADBDevice::memory() const { return m_memory; }
void ADBDevice::setMemory(const QString &val) { 
    if (m_memory != val) { 
        m_memory = val; 
        emit memoryChanged(val); 
    } 
}

QString ADBDevice::sdkVersion() const { return m_sdkVersion; }
void ADBDevice::setSdkVersion(const QString &val) { 
    if (m_sdkVersion != val) { 
        m_sdkVersion = val; 
        emit sdkVersionChanged(val); 
    } 
}

QString ADBDevice::serialNumber() const { return m_serialNumber; }
void ADBDevice::setSerialNumber(const QString &val) { 
    if (m_serialNumber != val) { 
        m_serialNumber = val; 
        emit serialNumberChanged(val); 
    } 
}

QString ADBDevice::maxFreq() const { return m_maxFreq; }
void ADBDevice::setMaxFreq(const QString &val) { 
    if (m_maxFreq != val) { 
        m_maxFreq = val; 
        emit maxFreqChanged(val); 
    } 
}

QString ADBDevice::maxCoreNum() const { return m_maxCoreNum; }
void ADBDevice::setMaxCoreNum(const QString &val) { 
    if (m_maxCoreNum != val) { 
        m_maxCoreNum = val; 
        emit maxCoreNumChanged(val); 
    } 
}

// === 电池信息访问器实现 ===
ChargingType ADBDevice::chargingType() const { return m_chargingType; }
void ADBDevice::setChargingType(ChargingType val) { 
    if (m_chargingType != val) { 
        m_chargingType = val; 
        emit chargingTypeChanged(val); 
    } 
}

uint ADBDevice::maxChargingCut() const { return m_maxChargingCut; }
void ADBDevice::setMaxChargingCut(uint val) { 
    if (m_maxChargingCut != val) { 
        m_maxChargingCut = val; 
        emit maxChargingCutChanged(val); 
    } 
}

uint ADBDevice::maxChargingVol() const { return m_maxChargingVol; }
void ADBDevice::setMaxChargingVol(uint val) { 
    if (m_maxChargingVol != val) { 
        m_maxChargingVol = val; 
        emit maxChargingVolChanged(val); 
    } 
}

uint ADBDevice::chargingCounter() const { return m_chargingCounter; }
void ADBDevice::setChargingCounter(uint val) { 
    if (m_chargingCounter != val) { 
        m_chargingCounter = val; 
        emit chargingCounterChanged(val); 
    } 
}

int ADBDevice::batteryStatus() const { return m_batteryStatus; }
void ADBDevice::setBatteryStatus(int val) { 
    if (m_batteryStatus != val) { 
        m_batteryStatus = val; 
        emit batteryStatusChanged(val); 
    } 
}

int ADBDevice::batteryHealth() const { return m_batteryHealth; }
void ADBDevice::setBatteryHealth(int val) { 
    if (m_batteryHealth != val) { 
        m_batteryHealth = val; 
        emit batteryHealthChanged(val); 
    } 
}

int ADBDevice::batteryLevel() const { return m_batteryLevel; }
void ADBDevice::setBatteryLevel(int val) { 
    if (m_batteryLevel != val) { 
        m_batteryLevel = val; 
        emit batteryLevelChanged(val); 
    } 
}

int ADBDevice::batteryScale() const { return m_batteryScale; }
void ADBDevice::setBatteryScale(int val) { 
    if (m_batteryScale != val) { 
        m_batteryScale = val; 
        emit batteryScaleChanged(val); 
    } 
}

uint ADBDevice::batteryVoltage() const { return m_batteryVoltage; }
void ADBDevice::setBatteryVoltage(uint val) { 
    if (m_batteryVoltage != val) { 
        m_batteryVoltage = val; 
        emit batteryVoltageChanged(val); 
    } 
}

uint ADBDevice::batteryCurrent() const { return m_batteryCurrent; }
void ADBDevice::setBatteryCurrent(uint val) { 
    if (m_batteryCurrent != val) { 
        m_batteryCurrent = val; 
        emit batteryCurrentChanged(val); 
    } 
}

float ADBDevice::batteryTemperature() const { return m_batteryTemperature; }
void ADBDevice::setBatteryTemperature(float val) { 
    if (m_batteryTemperature != val) { 
        m_batteryTemperature = val; 
        emit batteryTemperatureChanged(val); 
    } 
}

// === 当前活动信息访问器实现 ===
QString ADBDevice::windowCode() const { return m_windowCode; }
void ADBDevice::setWindowCode(const QString &val) { 
    if (m_windowCode != val) { 
        m_windowCode = val; 
        emit windowCodeChanged(val); 
    } 
}

QString ADBDevice::currentPackage() const { return m_currentPackage; }
void ADBDevice::setCurrentPackage(const QString &val) { 
    if (m_currentPackage != val) { 
        m_currentPackage = val; 
        emit currentPackageChanged(val); 
    } 
}

QString ADBDevice::currentActivity() const { return m_currentActivity; }
void ADBDevice::setCurrentActivity(const QString &val) { 
    if (m_currentActivity != val) { 
        m_currentActivity = val; 
        emit currentActivityChanged(val); 
    } 
}

void ADBDevice::requestDisConnect()
{

}

DeviceDetailInfo ADBDevice::refreshFixInfo()
{
    qDebug() << "执行refreshFixInfo() - 获取设备固定信息";
    
    DeviceDetailInfo info;
    /*厂商*/
    QString ret;
    QString code = this->code();
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
    info.macAddr = "未知";
    /*IP地址*/
    info.ipAddr = "未知";
    /*SDK版本*/
    info.sdkVersion = getDeviceProp(code, "ro.build.version.sdk");
    /*序列号*/
    ret = getDeviceProp(code, "ro.serialno").simplified();
    info.serialNumber = ret.split(' ').value(0);
    // CPU最大频率
    ret = m_adbTools->executeCommand(ADBTools::ADB, {"-s", code, "shell", "cat /sys/devices/system/cpu/cpu*/cpufreq/cpuinfo_max_freq"});
    {
        QStringList maxs = ret.split('\n');
        int max = 0; // 初始化为0
        for (const auto &maxStr : maxs) {
            max = std::max(maxStr.toInt(), max);
        }
        info.maxFrep = QString("%1Mhz").arg(QString::number(max / 1000));
    }
    // CPU核心数量 CPU 信息
    ret = m_adbTools->executeCommand(ADBTools::ADB, {"-s", code, "shell", "cat /proc/cpuinfo"});
    {
        QStringList retList = ret.split('\n');
        for (const auto &cpuInfo : retList) {
            if (cpuInfo.startsWith("CPU architecture")) {
                info.maxCoreNum = cpuInfo.split(':').last().simplified();
            } else if (cpuInfo.startsWith("Hardware")) {
                info.cpuInfo = cpuInfo.split(':').last().simplified();
                break;
            }
        }
    }

    if (info.cpuInfo.isEmpty()) {
        info.cpuInfo = getDeviceProp(code, "ro.hardware");
    }

    // 内存信息
    ret = m_adbTools->executeCommand(ADBTools::ADB, {"-s", code, "shell", "cat /proc/meminfo | grep MemTotal:"});
    info.memory = QString::number(ret.simplified().split(' ').value(1).toUInt() / 1024.0 / 1024.0, 'g') + "GB";

    qDebug() << "refreshFixInfo()完成";
    return info;
}

DeviceBatteryInfo ADBDevice::refreshBatteryInfo()
{
    qDebug() << "执行refreshBatteryInfo() - 获取电池信息";
    
    QString deviceCode = this->code();
    QString retStr = m_adbTools->executeCommand(ADBTools::ADB, {"-s", deviceCode, "shell", "dumpsys battery"});
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
    // 自测只有Android 15及其以上版本支持
    batteryInfo.current = retMap["Battery current"].toInt() < 0 ? retMap["Battery current"].toInt() * -1 : retMap["Battery current"].toInt();
    
    qDebug() << "refreshBatteryInfo()完成 - 电量:" << batteryInfo.level << "%";
    return batteryInfo;
}

DeviceCutActivityInfo ADBDevice::refreshCutActivityInfo()
{
    qDebug() << "执行refreshCutActivityInfo() - 获取当前活动信息";
    
    QString deviceCode = this->code();
    QStringList retStrList = m_adbTools->executeCommand(ADBTools::ADB, {"-s", deviceCode, "shell", "dumpsys activity activities | grep topResumedActivity"}).split('\n');
    
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
                qDebug() << "refreshCutActivityInfo()完成 - 找到活动:" << deviceCutActivity.cutPackage;
                return deviceCutActivity;
            }
        }
    }
    qDebug() << "refreshCutActivityInfo()完成 - 未找到当前活动";
    return deviceCutActivity;
}

QString ADBDevice::executeCommand(const QStringList &args, const QString &writeStr, const int timeout) const
{
    return m_adbTools->executeCommand(ADBTools::ADB, args, writeStr, timeout);
}

QString ADBDevice::getDeviceProp(const QString &deviceCode, const QString &prop)
{
    QStringList args;
    args << "-s" << deviceCode << "shell" << "getprop" << prop;
    return m_adbTools->executeCommand(ADBTools::ADB, args).simplified();
}

// 应用管理方法实现

bool ADBDevice::installApp(const QString &path, bool r, bool s, bool d, bool g)
{
    QStringList args;
    args << "-s" << code() << "install";
    
    if (r) args << "-r";
    if (s) args << "-s";
    if (d) args << "-d";
    if (g) args << "-g";
    
    args << path;
    
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return result.contains("Success");
}

bool ADBDevice::clearData(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "pm" << "clear" << packageName;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return result.contains("Success");
}

bool ADBDevice::uninstallApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "uninstall" << packageName;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return result.contains("Success");
}

bool ADBDevice::freezeApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "pm" << "disable-user" << packageName;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return !result.contains("Error");
}

bool ADBDevice::unfreezeApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "pm" << "enable" << packageName;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return !result.contains("Error");
}

bool ADBDevice::extractApp(const QString &packageName, const QString &targetPath)
{
    // 首先获取APK路径
    QStringList pathArgs;
    pathArgs << "-s" << code() << "shell" << "pm" << "path" << packageName;
    QString pathResult = m_adbTools->executeCommand(ADBTools::ADB, pathArgs);
    
    if (pathResult.isEmpty() || !pathResult.contains("package:")) {
        return false;
    }
    
    QString apkPath = pathResult.simplified().split(':').value(1);
    if (apkPath.isEmpty()) {
        return false;
    }
    
    // 提取APK文件
    QStringList args;
    args << "-s" << code() << "pull" << apkPath << targetPath;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return !result.contains("failed");
}

void ADBDevice::startApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "monkey" << "-p" << packageName 
         << "-c" << "android.intent.category.LAUNCHER" << "1";
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}

void ADBDevice::killActivity(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "am" << "force-stop" << packageName;
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::startActivity(const QString &activity, const QStringList &args)
{
    QStringList adbArgs;
    adbArgs << "-s" << code() << "shell" << "am" << "start" << "-n" << activity << args;
    m_adbTools->executeCommand(ADBTools::ADB, adbArgs);
}

QList<AppListInfo> ADBDevice::getSoftListInfo() const
{
    QStringList args;
    QList<AppListInfo> res;
    
    // 获取第三方应用列表
    args << "-s" << code() << "shell" << "pm" << "list" << "packages" << "-3" << "--show-versioncode";
    QStringList retList = m_adbTools->executeCommand(ADBTools::ADB, args).split('\n');
    
    int packageLen = QString("package:").length();
    int versionCodeLen = QString("versionCode:").length();
    
    for (const QString &lineInfo : retList) {
        QStringList infos = lineInfo.simplified().split(' ');
        if (infos.size() == 2) {
            AppListInfo appInfo;
            if (auto packageNameInfo = infos.first(); packageNameInfo.size() > packageLen) {
                appInfo.packageName = packageNameInfo.remove(0, packageLen);
            }
            if (auto versionCodeInfo = infos.last(); versionCodeInfo.size() > versionCodeLen) {
                appInfo.versionCode = versionCodeInfo.remove(0, versionCodeLen);
            }
            appInfo.state = AppState::APP_Enable;
            res.append(appInfo);
        }
    }

    // 获取禁用的应用列表
    args.clear();
    args << "-s" << code() << "shell" << "pm" << "list" << "packages" << "-d";
    retList = m_adbTools->executeCommand(ADBTools::ADB, args).split('\n');
    
    for (const QString &lineInfo : retList) {
        QString info = lineInfo.simplified();
        QString disablePackageName;
        if (auto packageNameInfo = info; packageNameInfo.size() > packageLen) {
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

AppDetailInfo ADBDevice::getAppDetailInfo(const QString &packageName) const
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "package" << packageName;
    const QStringList retInfo = m_adbTools->executeCommand(ADBTools::ADB, args).split('\n');
    
    AppDetailInfo resInfo;
    for (QString lineInfo : retInfo) {
        lineInfo = lineInfo.simplified();
        if (lineInfo.startsWith("firstInstallTime", Qt::CaseInsensitive)) {
            resInfo.installDate = lineInfo.split('=').last();
            continue;
        } else if (lineInfo.startsWith("versionName", Qt::CaseInsensitive)) {
            resInfo.versionName = lineInfo.split('=').last();
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

    // 获取APK路径
    args.clear();
    args << "-s" << code() << "shell" << "pm" << "path" << packageName;
    auto apkPathInfo = m_adbTools->executeCommand(ADBTools::ADB, args).simplified().split(':');
    resInfo.path = apkPathInfo.value(1);

    resInfo.packageName = packageName;
    return resInfo;
}

void ADBDevice::shotScreen(const QString &outPath)
{
    QString filePath = outPath + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
    QStringList args;
    args << "-s" << code() << "exec-out" << "screencap" << "-p";

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

void ADBDevice::pushKey(const QString &key)
{
    QStringList args;
    args << "-s" << code() << "shell" << "input" << "keyevent" << key;
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::setBatteryLevel(quint8 level)
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "set" << "level" << QString::number(level);
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::stopCharge()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "set" << "ac" << "0";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::restoreCharge()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "reset";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::connectButNoCharge()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "set" << "usb" << "0";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::restoreBatteryAll()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "reset";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::control(ControlType controlType, int controlItem)
{
    // 音乐控制映射
    static const QHash<int, QString> MusicControlArgsMap {
        { 0, "shell input keyevent 88" },  // PreviouSong
        { 1, "shell input keyevent 86" },  // StopPlay  
        { 2, "shell input keyevent 85" },  // PlayAndPause
        { 3, "shell input keyevent 87" },  // NextSong
        { 4, "shell input keyevent 25" },  // VolumeReduce
        { 5, "shell input keyevent 24" },  // VolumeAdd
    };
    
    // 按键控制映射  
    static const QHash<int, QString> KeyControlArgsMap {
        { 0, "shell input keyevent 82" },   // Menu
        { 1, "shell input keyevent 3" },    // Home
        { 2, "shell input keyevent 4" },    // Back
        { 3, "shell input keyevent 26" },   // Power
        { 4, "shell input keyevent 221" },  // AddBrightness
        { 5, "shell input keyevent 220" },  // ReduceBrightness
        { 6, "shutdown" },                  // Poweroff
        { 7, "reboot" },                    // Reboot
        { 8, "reboot recovery" },           // RebootToRec
        { 9, "reboot bootloader" },         // RebootToFB
        { 10, "shell input keyevent 27" },  // Shot
        { 11, "shell input keyevent 164" }, // Mute
        { 12, "shell input keyevent 231" }, // StartAssistant
        { 13, "shell input keyevent 122" }, // CursorToBegin
        { 14, "shell input keyevent 123" }  // CursorToEnd
    };
    
    // 广播控制映射
    static const QHash<int, QString> BoardControlArgsMap {
        { 0, "shell am broadcast -a android.net.conn.CONNECTIVITY_CHANGE" },
        { 1, "shell am broadcast -a android.intent.action.SCREEN_ON" },
        { 2, "shell am broadcast -a android.intent.action.SCREEN_OFF" },
        { 3, "shell am broadcast -a android.intent.action.BATTERY_LOW" },
        { 4, "shell am broadcast -a android.intent.action.BATTERY_OKAY" },
        { 5, "shell am broadcast -a android.intent.action.BOOT_COMPLETED" },
        { 6, "shell am broadcast -a android.intent.action.DEVICE_STORAGE_LOW" },
        { 7, "shell am broadcast -a android.intent.action.DEVICE_STORAGE_OK" },
        { 8, "shell am broadcast -a android.intent.action.PACKAGE_ADDED" },
        { 9, "shell am broadcast -a android.net.wifi.STATE_CHANGE" },
        { 10, "shell am broadcast -a android.net.wifi.WIFI_STATE_CHANGED" },
        { 11, "shell am broadcast -a android.intent.action.BATTERY_CHANGED" },
        { 12, "shell am broadcast -a android.intent.action.INPUT_METHOD_CHANGED" },
        { 13, "shell am broadcast -a android.intent.action.ACTION_POWER_CONNECTED" },
        { 14, "shell am broadcast -a android.intent.action.ACTION_POWER_DISCONNECTED" },
        { 15, "shell am broadcast -a android.intent.action.DREAMING_STARTED" },
        { 16, "shell am broadcast -a android.intent.action.DREAMING_STOPPED" },
        { 17, "shell am broadcast -a android.intent.action.WALLPAPER_CHANGED" },
        { 18, "shell am broadcast -a android.intent.action.HEADSET_PLUG" },
        { 19, "shell am broadcast -a android.intent.action.MEDIA_UNMOUNTED" },
        { 20, "shell am broadcast -a android.intent.action.MEDIA_MOUNTED" },
        { 21, "shell am broadcast -a android.os.action.POWER_SAVE_MODE_CHANGED" }
    };
    
    QStringList args;
    args << "-s" << code();
    
    if (controlType == CTRL_Music) {
        QString cmd = MusicControlArgsMap.value(controlItem);
        if (!cmd.isEmpty()) {
            args << cmd.split(' ');
        }
    } else if (controlType == CTRL_Key) {
        QString cmd = KeyControlArgsMap.value(controlItem);
        if (!cmd.isEmpty()) {
            args << cmd.split(' ');
        }
    } else if (controlType == CTRL_BoardCast) {
        QString cmd = BoardControlArgsMap.value(controlItem);
        if (!cmd.isEmpty()) {
            args << cmd.split(' ');
        }
    }
    
    if (args.size() > 2) {
        m_adbTools->executeCommand(ADBTools::ADB, args);
    }
}

void ADBDevice::extractFile(const QString &source, const QString &targetDir)
{
    QStringList args;
    args << "-s" << code() << "pull" << source << targetDir;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}

void ADBDevice::pushFile(const QString &source, const QString &targetDir)
{
    QStringList args;
    args << "-s" << code() << "push" << source << targetDir;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}

void ADBDevice::setResolution(quint16 width, quint16 height)
{
    QStringList args;
    args << "-s" << code() << "shell" << "wm" << "size" << QString("%1x%2").arg(width).arg(height);
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::setDPI(quint16 dpi)
{
    QStringList args;
    args << "-s" << code() << "shell" << "wm" << "density" << QString::number(dpi);
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::restoreResolutionAndDpi()
{
    QStringList args1, args2;
    args1 << "-s" << code() << "shell" << "wm" << "size" << "reset";
    args2 << "-s" << code() << "shell" << "wm" << "density" << "reset";
    m_adbTools->executeCommand(ADBTools::ADB, args1);
    m_adbTools->executeCommand(ADBTools::ADB, args2);
}