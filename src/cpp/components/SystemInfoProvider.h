#pragma once

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QJsonObject>

class SystemInfoProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double cpuUsage   READ cpuUsage   NOTIFY systemInfoChanged)
    Q_PROPERTY(double cpuTemp    READ cpuTemp    NOTIFY systemInfoChanged)
    Q_PROPERTY(double gpuUsage   READ gpuUsage   NOTIFY systemInfoChanged)
    Q_PROPERTY(double ramUsage   READ ramUsage   NOTIFY systemInfoChanged)
    Q_PROPERTY(double ramTotal   READ ramTotal   NOTIFY systemInfoChanged)
    Q_PROPERTY(double storageUsed READ storageUsed NOTIFY systemInfoChanged)
    Q_PROPERTY(double storageTotal READ storageTotal NOTIFY systemInfoChanged)
    Q_PROPERTY(bool   polling    READ polling    NOTIFY pollingChanged)

public:
    static SystemInfoProvider *instance();

    Q_INVOKABLE void startPolling();
    Q_INVOKABLE void stopPolling();

    double cpuUsage() const;
    double cpuTemp() const;
    double gpuUsage() const;
    double ramUsage() const;
    double ramTotal() const;
    double storageUsed() const;
    double storageTotal() const;
    bool polling() const;

signals:
    void systemInfoChanged();
    void pollingChanged();

private slots:
    void fetchSystemInfo();

private:
    explicit SystemInfoProvider(QObject *parent = nullptr);
    void parseResponse(const QJsonObject &root);

    QNetworkAccessManager *m_nam;
    QTimer *m_timer;

    double m_cpuUsage = 0;
    double m_cpuTemp = 0;
    double m_gpuUsage = 0;
    double m_ramUsage = 0;
    double m_ramTotal = 0;
    double m_storageUsed = 0;
    double m_storageTotal = 0;
};
