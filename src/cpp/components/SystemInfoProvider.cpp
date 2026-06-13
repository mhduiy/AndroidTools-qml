#include "SystemInfoProvider.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QEventLoop>

static SystemInfoProvider *s_instance = nullptr;

SystemInfoProvider *SystemInfoProvider::instance()
{
    if (!s_instance)
        s_instance = new SystemInfoProvider;
    return s_instance;
}

SystemInfoProvider::SystemInfoProvider(QObject *parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
    , m_timer(new QTimer(this))
{
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, this, &SystemInfoProvider::fetchSystemInfo);
}

void SystemInfoProvider::startPolling()
{
    if (m_timer->isActive()) return;
    m_timer->start();
    fetchSystemInfo();
    emit pollingChanged();
}

void SystemInfoProvider::stopPolling()
{
    if (!m_timer->isActive()) return;
    m_timer->stop();
    emit pollingChanged();
}

bool SystemInfoProvider::polling() const
{
    return m_timer->isActive();
}

void SystemInfoProvider::fetchSystemInfo()
{
    QJsonObject cpu = getJson("/cpu");
    if (!cpu.isEmpty()) {
        m_cpuModel = cpu["model"].toString();
        m_cpuArchitecture = cpu["architecture"].toString();
        m_cpuCoreCount = cpu["coreCount"].toInt();
        m_cpuUsage = cpu["currentUsage"].toDouble(cpu["overall_usage"].toDouble());
        m_cpuTemp = cpu["temperature"].toDouble();
        m_cpuMaxFrequency = cpu["maxFrequency"].toInt();
        m_cpuMinFrequency = cpu["minFrequency"].toInt();
    }

    QJsonObject gpu = getJson("/gpu");
    if (!gpu.isEmpty()) {
        m_gpuName = gpu["name"].toString();
        m_gpuVendor = gpu["vendor"].toString();
        m_gpuUsage = gpu["usage"].toDouble();
        m_gpuCurrentFrequency = gpu["currentFrequency"].toInt();
        m_gpuMaxFrequency = gpu["maxFrequency"].toInt();
        m_gpuTemperature = gpu["temperature"].toInt(-1);
    }

    QJsonObject mem = getJson("/memory");
    if (!mem.isEmpty()) {
        m_ramTotal = mem["totalMemory"].toDouble() / 1024.0 / 1024.0 / 1024.0;
        m_ramUsage = mem["usedMemory"].toDouble() / 1024.0 / 1024.0 / 1024.0;
        m_memAvailable = mem["availableMemory"].toDouble() / 1024.0 / 1024.0 / 1024.0;
        m_lowMemory = mem["lowMemory"].toBool();
        m_storageTotal = mem["totalStorage"].toDouble() / 1024.0 / 1024.0 / 1024.0;
        m_storageUsed = mem["usedStorage"].toDouble() / 1024.0 / 1024.0 / 1024.0;
    }

    QJsonObject battery = getJson("/battery");
    if (!battery.isEmpty()) {
        m_batteryPower = battery["power"].toDouble();
        m_batteryCapacity = battery["capacity"].toVariant().toLongLong();
        m_batteryChargeCounter = battery["chargeCounter"].toInt();
        m_batteryTechnology = battery["technology"].toString();
    }

    QJsonObject current = getJson("/current-app");
    if (!current.isEmpty()) {
        m_foregroundAppName = current["appName"].toString();
        m_foregroundPackage = current["packageName"].toString();
        m_foregroundActivity = current["activityName"].toString();
        m_foregroundVersion = current["version"].toString();
        m_foregroundPid = current["pid"].toInt();
        m_foregroundUid = current["uid"].toInt();
        m_foregroundCpuUsage = current["cpuUsage"].toDouble();
        m_foregroundMemoryMB = current["memoryUsageMB"].toDouble();
        m_fps = current["fps"].toInt();
    }

    emit systemInfoChanged();
}

QJsonObject SystemInfoProvider::getJson(const QString &path, int timeoutMs)
{
    QNetworkRequest req(QUrl(QStringLiteral("http://localhost:18888") + path));
    req.setTransferTimeout(timeoutMs);

    QNetworkReply *reply = m_nam->get(req);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return {};
    }

    const QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return {};
    }

    QJsonObject root = doc.object();
    if (root.contains("error")) {
        return {};
    }
    return root;
}

double SystemInfoProvider::cpuUsage() const   { return m_cpuUsage; }
double SystemInfoProvider::cpuTemp() const    { return m_cpuTemp; }
double SystemInfoProvider::gpuUsage() const   { return m_gpuUsage; }
double SystemInfoProvider::ramUsage() const   { return m_ramUsage; }
double SystemInfoProvider::ramTotal() const   { return m_ramTotal; }
double SystemInfoProvider::storageUsed() const  { return m_storageUsed; }
double SystemInfoProvider::storageTotal() const { return m_storageTotal; }
double SystemInfoProvider::memAvailable() const { return m_memAvailable; }
bool SystemInfoProvider::lowMemory() const { return m_lowMemory; }
QString SystemInfoProvider::cpuModel() const { return m_cpuModel; }
QString SystemInfoProvider::cpuArchitecture() const { return m_cpuArchitecture; }
int SystemInfoProvider::cpuCoreCount() const { return m_cpuCoreCount; }
int SystemInfoProvider::cpuMaxFrequency() const { return m_cpuMaxFrequency; }
int SystemInfoProvider::cpuMinFrequency() const { return m_cpuMinFrequency; }
QString SystemInfoProvider::gpuName() const { return m_gpuName; }
QString SystemInfoProvider::gpuVendor() const { return m_gpuVendor; }
int SystemInfoProvider::gpuCurrentFrequency() const { return m_gpuCurrentFrequency; }
int SystemInfoProvider::gpuMaxFrequency() const { return m_gpuMaxFrequency; }
int SystemInfoProvider::gpuTemperature() const { return m_gpuTemperature; }
int SystemInfoProvider::fps() const { return m_fps; }
QString SystemInfoProvider::foregroundAppName() const { return m_foregroundAppName; }
QString SystemInfoProvider::foregroundPackage() const { return m_foregroundPackage; }
QString SystemInfoProvider::foregroundActivity() const { return m_foregroundActivity; }
QString SystemInfoProvider::foregroundVersion() const { return m_foregroundVersion; }
int SystemInfoProvider::foregroundPid() const { return m_foregroundPid; }
int SystemInfoProvider::foregroundUid() const { return m_foregroundUid; }
double SystemInfoProvider::foregroundCpuUsage() const { return m_foregroundCpuUsage; }
double SystemInfoProvider::foregroundMemoryMB() const { return m_foregroundMemoryMB; }
double SystemInfoProvider::batteryPower() const { return m_batteryPower; }
long long SystemInfoProvider::batteryCapacity() const { return m_batteryCapacity; }
int SystemInfoProvider::batteryChargeCounter() const { return m_batteryChargeCounter; }
QString SystemInfoProvider::batteryTechnology() const { return m_batteryTechnology; }
