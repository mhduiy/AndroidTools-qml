#include "adbdevice.h"
#include "src/cpp/utils/utils.hpp"
#include "src/cpp/adb/adbtools.h"
#include "src/cpp/utils/constants.h"
#include <algorithm>
#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QTimer>
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

namespace ADT {

ADBDeviceWorker::ADBDeviceWorker(ADBDevice *device, QObject *parent)
    : QObject(parent)
    , m_device(device)
    , m_refreshTimer(new QTimer(this))
{
    m_refreshTimer->setInterval(3000);
    m_refreshTimer->setSingleShot(false);
    connect(m_refreshTimer, &QTimer::timeout, this, &ADBDeviceWorker::refreshRealtimeInfo);
}

ADBDeviceWorker::~ADBDeviceWorker()
{
    m_refreshTimer->stop();
}

void ADBDeviceWorker::refreshFixInfo()
{
    DeviceDetailInfo info;
    /*厂商*/
    QString ret;
    QString code = m_device->code();
    info.manufacturer = m_device->getDeviceProp(code, "ro.product.manufacturer");
    /*品牌*/
    info.brand = m_device->getDeviceProp(code, "ro.product.brand");    
    /*型号*/
    info.model = m_device->getDeviceProp(code, "ro.product.model");
    /*设备名*/
    info.deviceName = m_device->getDeviceProp(code, "ro.product.marketname");
    /*设备代号*/
    info.deviceCode = m_device->getDeviceProp(code, "ro.product.name");
    /*系统信息*/
    info.systemInfo = m_device->getDeviceProp(code, "ro.custom.version");
    /*安卓版本*/
    info.androidVersion = m_device->getDeviceProp(code, "ro.build.version.release");
    /*分辨率*/
    info.resolving = m_device->executeCommand({"-s", code, "shell", "wm size"}).simplified().split(' ').last();
    /*Dpi*/
    info.dpi = m_device->getDeviceProp(code, "ro.sf.lcd_density");
    /*MAC地址*/
    info.macAddr = "未知";
    /*IP地址*/
    info.ipAddr = "未知";
    /*SDK版本*/
    info.sdkVersion = m_device->getDeviceProp(code, "ro.build.version.sdk");
    /*序列号*/
    ret = m_device->getDeviceProp(code, "ro.serialno").simplified();
    info.serialNumber = ret.split(' ').value(0);
    // CPU最大频率
    ret = m_device->executeCommand({"-s", code, "shell", "cat /sys/devices/system/cpu/cpu*/cpufreq/cpuinfo_max_freq"});
    {
        QStringList maxs = ret.split('\n');
        int max = 0; // 初始化为0
        for (const auto &maxStr : maxs) {
            max = std::max(maxStr.toInt(), max);
        }
        info.maxFrep = QString("%1Mhz").arg(QString::number(max / 1000));
    }
    // CPU核心数量 CPU 信息
    ret = m_device->executeCommand({"-s", code, "shell", "cat /proc/cpuinfo"});
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
        info.cpuInfo = m_device->getDeviceProp(code, "ro.hardware");
    }

    // 内存信息
    ret = m_device->executeCommand({"-s", code, "shell", "cat /proc/meminfo | grep MemTotal:"});
    info.memory = QString::number(ret.simplified().split(' ').value(1).toUInt() / 1024.0 / 1024.0, 'g') + "GB";
    
    Q_EMIT fixInfoRefreshed(info);
}

void ADBDeviceWorker::startRefreshRealtimeInfo()
{
    m_refreshTimer->start();
}

void ADBDeviceWorker::stopRefreshRealtimeInfo()
{
    m_refreshTimer->stop();
}

void ADBDeviceWorker::refreshRealtimeInfo()
{
    refreshCutActivityInfo();
    refreshBatteryInfo();
}

void ADBDeviceWorker::refreshCutActivityInfo()
{
    QString deviceCode = m_device->code();
    QStringList retStrList = m_device->executeCommand({"-s", deviceCode, "shell", "dumpsys activity activities | grep topResumedActivity"}).split('\n');
    
    DeviceActivityInfo deviceCutActivity;
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
                Q_EMIT cutActivityRefreshed(deviceCutActivity);
            }
        }
    }
}

void ADBDeviceWorker::refreshBatteryInfo()
{
    QString deviceCode = m_device->code();
    QString retStr = m_device->executeCommand({"-s", deviceCode, "shell", "dumpsys battery"});
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
    
    Q_EMIT batteryInfoRefreshed(batteryInfo);
}

ADBDevice::ADBDevice(const QString &code, QObject *parent)
    : Device(code, parent)
    , m_worker(new ADBDeviceWorker(this))
    , m_workerThread(new QThread(this))
    , m_adbTools(ADBTools::instance())
{
    m_serverPro = new QProcess(this);
    startAndroidService();
    initData();
    initWorker();
}

ADBDevice::~ADBDevice()
{
    m_workerThread->quit();
    m_workerThread->wait();
    m_workerThread->deleteLater();
    m_worker->deleteLater();
}

bool ADBDevice::startAndroidService()
{
    qInfo() << "Starting androidtools-server on device:" << code();
    if (!QFile::exists(ANDROID_TOOLS_SERVER_TARGET_PATH)) {
        // 确保目标目录存在
        QFileInfo fileInfo(ANDROID_TOOLS_SERVER_TARGET_PATH);
        QDir dir = fileInfo.dir();
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QFile resourceFile(ANDROID_TOOLS_SERVER_PATH);
        if (!resourceFile.exists()) {
            qWarning() << "Resource file does not exist!";
            return false;
        }
        if (!resourceFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open resource file:" << resourceFile.error();
            return false;
        }
        QFile targetFile(ANDROID_TOOLS_SERVER_TARGET_PATH);
        if (!targetFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Cannot open target file:" << targetFile.error();
            return false;
        }
        QByteArray data = resourceFile.readAll();
        if (targetFile.write(data) == -1) {
            qWarning() << "Failed to write file:" << targetFile.error();
            return false;
        }
        resourceFile.close();
        targetFile.close();    
    }

    // 推送文件到设备
    pushFile(ANDROID_TOOLS_SERVER_TARGET_PATH, "/data/local/tmp/androidtools-server.dex");

    // 设置权限
    auto ret = m_adbTools->executeShellCommand("chmod 755 /data/local/tmp/androidtools-server.dex");
    if (!ret.success) {
        qWarning() << "Set server dex file permission failed:" << ret.getAllOutput();
        return false;
    }

    // 端口转发 TODO: 多设备导致端口冲突
    ret = m_adbTools->executeCommandDetailed(ADBTools::ADB, {"forward", "tcp:18888", "tcp:18888"});
    if (!ret.success) {
        qWarning() << "ADB forward port failed:" << ret.getAllOutput();
        return false;
    }

    // 启动服务
    m_serverPro->start("adb", {"-s", code(), "shell", "CLASSPATH=/data/local/tmp/androidtools-server.dex app_process / com.mhduiy.androidtoolsserver.SystemInfoServer 18888"});
    m_serverPro->waitForStarted(3000);
    return true;
}

bool ADBDevice::killAndroidService()
{
    m_serverPro->terminate();
    m_serverPro->waitForFinished(1000);
    if (m_serverPro->state() != QProcess::NotRunning) {
        m_serverPro->kill();
        m_serverPro->waitForFinished(1000);
    }
    return true;
}

void ADBDevice::initWorker()
{
    m_worker->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished, m_workerThread, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished, this, &ADBDevice::deleteLater);

    connect(m_worker, &ADBDeviceWorker::fixInfoRefreshed, this, &ADBDevice::onFixInfoRefreshed);
    connect(m_worker, &ADBDeviceWorker::cutActivityRefreshed, this, &ADBDevice::onCutActivityRefreshed);
    connect(m_worker, &ADBDeviceWorker::batteryInfoRefreshed, this, &ADBDevice::onBatteryInfoRefreshed);

    m_workerThread->start();

    QMetaObject::invokeMethod(m_worker, "refreshFixInfo", Qt::QueuedConnection);
    QMetaObject::invokeMethod(m_worker, "startRefreshRealtimeInfo", Qt::QueuedConnection);
    
}

void ADBDevice::onFixInfoRefreshed(const DeviceDetailInfo &info)
{
    updateDetailInfoFromStruct(info);
}

void ADBDevice::onCutActivityRefreshed(const DeviceActivityInfo &activityInfo)
{
    updateActivityInfoFromStruct(activityInfo);
}

void ADBDevice::onBatteryInfoRefreshed(const DeviceBatteryInfo &batteryInfo)
{
    updateBatteryInfoFromStruct(batteryInfo);
    setisCharging(batteryInfo.chargingType != None);
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

void ADBDevice::updateActivityInfoFromStruct(const DeviceActivityInfo &info)
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

QString ADBDevice::executeCommand(const QStringList &args, const QString &writeStr, const int timeout) const
{
    return m_adbTools->executeCommand(ADBTools::ADB, args, writeStr, timeout);
}

CommandResult ADBDevice::executeCommandDetailed(const QStringList &args, const QString &writeStr, const int timeout) const
{
    return m_adbTools->executeCommandDetailed(ADBTools::ADB, args, writeStr, timeout);
}

QString ADBDevice::getDeviceProp(const QString &deviceCode, const QString &prop)
{
    QStringList args;
    args << "-s" << deviceCode << "shell" << "getprop" << prop;
    return m_adbTools->executeCommand(ADBTools::ADB, args).simplified();
}

void ADBDevice::initData()
{
    m_networkManager = new QNetworkAccessManager(this);
    // 匹配 IPv4:端口
    QRegularExpression ipv4PortRegex(
        R"(^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?):[0-9]{1,5}$)"
    );
    setisWireless(ipv4PortRegex.match(code()).hasMatch());
}

NetResult ADBDevice::syncCallNetGetMethod(const QUrl &url) const
{
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const bool success = reply->error() == QNetworkReply::NoError;
    const QByteArray data = reply->readAll();
    reply->deleteLater();
    return {success, data};
}

} // namespace ADT
