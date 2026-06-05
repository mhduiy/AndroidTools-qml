#include "SystemInfoProvider.h"
#include <QJsonDocument>
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
    QNetworkRequest req(QUrl("http://localhost:18888/system"));
    req.setTransferTimeout(1500);

    QNetworkReply *reply = m_nam->get(req);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return;

    QJsonObject root = doc.object();
    if (root.contains("error")) return;

    if (root.contains("cpusummary")) {
        QJsonObject cpu = root["cpusummary"].toObject();
        m_cpuUsage = cpu["currentUsage"].toDouble();
        m_cpuTemp = cpu["temperature"].toDouble();
    }

    if (root.contains("gpusummary")) {
        QJsonObject gpu = root["gpusummary"].toObject();
        m_gpuUsage = gpu["usage"].toDouble();
    }

    if (root.contains("memorysummary")) {
        QJsonObject mem = root["memorysummary"].toObject();
        m_ramTotal = mem["totalMemory"].toDouble() / 1024.0;
        m_ramUsage = mem["usedMemory"].toDouble() / 1024.0;
        m_storageTotal = mem["totalStorage"].toDouble() / 1024.0;
        m_storageUsed = mem["usedStorage"].toDouble() / 1024.0;
    }

    emit systemInfoChanged();
}

double SystemInfoProvider::cpuUsage() const   { return m_cpuUsage; }
double SystemInfoProvider::cpuTemp() const    { return m_cpuTemp; }
double SystemInfoProvider::gpuUsage() const   { return m_gpuUsage; }
double SystemInfoProvider::ramUsage() const   { return m_ramUsage; }
double SystemInfoProvider::ramTotal() const   { return m_ramTotal; }
double SystemInfoProvider::storageUsed() const  { return m_storageUsed; }
double SystemInfoProvider::storageTotal() const { return m_storageTotal; }
