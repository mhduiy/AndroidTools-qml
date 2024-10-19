#ifndef FASTBOOTDEVICEMANAGER_H
#define FASTBOOTDEVICEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include "../adb/adbtools.h"
#include "../utils/notificationcontroller.h"
#include "../utils/singleton.hpp"

class DeviceCheckTool : public QObject
{
    Q_OBJECT
public:
    explicit DeviceCheckTool(QObject *parent = nullptr);
    void startCheck();
    void stopCheck();
    QStringList getDevices();

signals:
    void checkFinish();
private:
    void checkDevice();
private:
    QTimer *m_checkTimer;
    QStringList m_devices;
    ADBTools *m_adbTool;
};

class FastBootDeviceManager : public QObject
{
    Q_OBJECT
    SINGLETON(FastBootDeviceManager)
    Q_PROPERTY(QString currentDeviceCode READ currentDeviceCode NOTIFY currentDeviceChanged)

public:
    void setCurrentDevice(const QString &deviceCode);
    void setCurrentDevice(int deviceIndex);

    Q_INVOKABLE void updateDevices();
    Q_INVOKABLE void rebootToFastBoot(const QString &deviceCode);
    Q_INVOKABLE void rebootToSystem(const QString &deviceCode);
    Q_INVOKABLE void rebootToRecovery(const QString &deviceCode);
    Q_INVOKABLE void powerOff(const QString &deviceCode);

    QString currentDeviceCode();
signals:
    void currentDeviceChanged(const QString &deviceCode);

private:
    void handleDeviceChanged();

private:
    QThread *m_deviceCheckThread;
    DeviceCheckTool *m_deviceCheckTool;

    QString m_currentDeviceCode;
};

#endif // FASTBOOTDEVICEMANAGER_H
