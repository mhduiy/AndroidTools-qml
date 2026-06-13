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
    Q_PROPERTY(double memAvailable READ memAvailable NOTIFY systemInfoChanged)
    Q_PROPERTY(bool lowMemory READ lowMemory NOTIFY systemInfoChanged)
    Q_PROPERTY(QString cpuModel READ cpuModel NOTIFY systemInfoChanged)
    Q_PROPERTY(QString cpuArchitecture READ cpuArchitecture NOTIFY systemInfoChanged)
    Q_PROPERTY(int cpuCoreCount READ cpuCoreCount NOTIFY systemInfoChanged)
    Q_PROPERTY(int cpuMaxFrequency READ cpuMaxFrequency NOTIFY systemInfoChanged)
    Q_PROPERTY(int cpuMinFrequency READ cpuMinFrequency NOTIFY systemInfoChanged)
    Q_PROPERTY(QString gpuName READ gpuName NOTIFY systemInfoChanged)
    Q_PROPERTY(QString gpuVendor READ gpuVendor NOTIFY systemInfoChanged)
    Q_PROPERTY(int gpuCurrentFrequency READ gpuCurrentFrequency NOTIFY systemInfoChanged)
    Q_PROPERTY(int gpuMaxFrequency READ gpuMaxFrequency NOTIFY systemInfoChanged)
    Q_PROPERTY(int gpuTemperature READ gpuTemperature NOTIFY systemInfoChanged)
    Q_PROPERTY(int fps READ fps NOTIFY systemInfoChanged)
    Q_PROPERTY(QString foregroundAppName READ foregroundAppName NOTIFY systemInfoChanged)
    Q_PROPERTY(QString foregroundPackage READ foregroundPackage NOTIFY systemInfoChanged)
    Q_PROPERTY(QString foregroundActivity READ foregroundActivity NOTIFY systemInfoChanged)
    Q_PROPERTY(QString foregroundVersion READ foregroundVersion NOTIFY systemInfoChanged)
    Q_PROPERTY(int foregroundPid READ foregroundPid NOTIFY systemInfoChanged)
    Q_PROPERTY(int foregroundUid READ foregroundUid NOTIFY systemInfoChanged)
    Q_PROPERTY(double foregroundCpuUsage READ foregroundCpuUsage NOTIFY systemInfoChanged)
    Q_PROPERTY(double foregroundMemoryMB READ foregroundMemoryMB NOTIFY systemInfoChanged)
    Q_PROPERTY(double batteryPower READ batteryPower NOTIFY systemInfoChanged)
    Q_PROPERTY(long long batteryCapacity READ batteryCapacity NOTIFY systemInfoChanged)
    Q_PROPERTY(int batteryChargeCounter READ batteryChargeCounter NOTIFY systemInfoChanged)
    Q_PROPERTY(QString batteryTechnology READ batteryTechnology NOTIFY systemInfoChanged)
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
    double memAvailable() const;
    bool lowMemory() const;
    QString cpuModel() const;
    QString cpuArchitecture() const;
    int cpuCoreCount() const;
    int cpuMaxFrequency() const;
    int cpuMinFrequency() const;
    QString gpuName() const;
    QString gpuVendor() const;
    int gpuCurrentFrequency() const;
    int gpuMaxFrequency() const;
    int gpuTemperature() const;
    int fps() const;
    QString foregroundAppName() const;
    QString foregroundPackage() const;
    QString foregroundActivity() const;
    QString foregroundVersion() const;
    int foregroundPid() const;
    int foregroundUid() const;
    double foregroundCpuUsage() const;
    double foregroundMemoryMB() const;
    double batteryPower() const;
    long long batteryCapacity() const;
    int batteryChargeCounter() const;
    QString batteryTechnology() const;
    bool polling() const;

signals:
    void systemInfoChanged();
    void pollingChanged();

private slots:
    void fetchSystemInfo();

private:
    explicit SystemInfoProvider(QObject *parent = nullptr);
    QJsonObject getJson(const QString &path, int timeoutMs = 700);

    QNetworkAccessManager *m_nam;
    QTimer *m_timer;

    double m_cpuUsage = 0;
    double m_cpuTemp = 0;
    double m_gpuUsage = 0;
    double m_ramUsage = 0;
    double m_ramTotal = 0;
    double m_storageUsed = 0;
    double m_storageTotal = 0;
    double m_memAvailable = 0;
    bool m_lowMemory = false;
    QString m_cpuModel;
    QString m_cpuArchitecture;
    int m_cpuCoreCount = 0;
    int m_cpuMaxFrequency = 0;
    int m_cpuMinFrequency = 0;
    QString m_gpuName;
    QString m_gpuVendor;
    int m_gpuCurrentFrequency = 0;
    int m_gpuMaxFrequency = 0;
    int m_gpuTemperature = -1;
    int m_fps = 0;
    QString m_foregroundAppName;
    QString m_foregroundPackage;
    QString m_foregroundActivity;
    QString m_foregroundVersion;
    int m_foregroundPid = 0;
    int m_foregroundUid = 0;
    double m_foregroundCpuUsage = 0;
    double m_foregroundMemoryMB = 0;
    double m_batteryPower = 0;
    long long m_batteryCapacity = 0;
    int m_batteryChargeCounter = 0;
    QString m_batteryTechnology;
};
