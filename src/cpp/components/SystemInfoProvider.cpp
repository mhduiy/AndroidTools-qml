#include "SystemInfoProvider.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

static SystemInfoProvider *s_instance = nullptr;
namespace {
QJsonObject parseJsonReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        return {};
    }

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error != QJsonParseError::NoError || !document.isObject()) {
        return {};
    }

    const QJsonObject root = document.object();
    return root.contains("error") ? QJsonObject{} : root;
}
}

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
    if (m_pendingRequests > 0) {
        return;
    }

    const QStringList paths = {"/cpu", "/gpu", "/memory", "/battery", "/current-app"};
    m_pendingRequests = paths.size();

    for (const QString &path : paths) {
        QNetworkRequest request(QUrl(QStringLiteral("http://localhost:18888") + path));
        request.setTransferTimeout(700);
        QNetworkReply *reply = m_nam->get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, path]() {
            const QJsonObject data = parseJsonReply(reply);
            reply->deleteLater();

            if (path == "/cpu" && !data.isEmpty()) {
                m_cpuModel = data["model"].toString();
                m_cpuArchitecture = data["architecture"].toString();
                m_cpuCoreCount = data["coreCount"].toInt();
                m_cpuUsage = data["currentUsage"].toDouble(data["overall_usage"].toDouble());
                m_cpuTemp = data["temperature"].toDouble();
                m_cpuMaxFrequency = data["maxFrequency"].toInt();
                m_cpuMinFrequency = data["minFrequency"].toInt();
            } else if (path == "/gpu" && !data.isEmpty()) {
                m_gpuName = data["name"].toString();
                m_gpuVendor = data["vendor"].toString();
                m_gpuUsage = data["usage"].toDouble();
                m_gpuCurrentFrequency = data["currentFrequency"].toInt();
                m_gpuMaxFrequency = data["maxFrequency"].toInt();
                m_gpuTemperature = data["temperature"].toInt(-1);
            } else if (path == "/memory" && !data.isEmpty()) {
                m_ramTotal = data["totalMemory"].toDouble() / 1024.0 / 1024.0 / 1024.0;
                m_ramUsage = data["usedMemory"].toDouble() / 1024.0 / 1024.0 / 1024.0;
                m_memAvailable = data["availableMemory"].toDouble() / 1024.0 / 1024.0 / 1024.0;
                m_lowMemory = data["lowMemory"].toBool();
                m_storageTotal = data["totalStorage"].toDouble() / 1024.0 / 1024.0 / 1024.0;
                m_storageUsed = data["usedStorage"].toDouble() / 1024.0 / 1024.0 / 1024.0;
            } else if (path == "/battery" && !data.isEmpty()) {
                m_batteryPower = data["power"].toDouble();
                m_batteryCapacity = data["capacity"].toVariant().toLongLong();
                m_batteryChargeCounter = data["chargeCounter"].toInt();
                m_batteryTechnology = data["technology"].toString();
            } else if (path == "/current-app" && !data.isEmpty()) {
                m_foregroundAppName = data["appName"].toString();
                m_foregroundPackage = data["packageName"].toString();
                m_foregroundActivity = data["activityName"].toString();
                m_foregroundVersion = data["version"].toString();
                m_foregroundPid = data["pid"].toInt();
                m_foregroundUid = data["uid"].toInt();
                m_foregroundCpuUsage = data["cpuUsage"].toDouble();
                m_foregroundMemoryMB = data["memoryUsageMB"].toDouble();
                m_fps = data["fps"].toInt();
            }

            if (--m_pendingRequests == 0) {
                emit systemInfoChanged();
            }
        });
    }
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
