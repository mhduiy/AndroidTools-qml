#pragma once

#include <QObject>
#include <QTimer>

#include "src/cpp/adb/adbdevice.h"
#include "src/cpp/adb/device.h"
#include "src/cpp/adb/fastbootdevice.h"

#include "../utils/defutils.hpp"

#define CONNECTMANAGER ConnectManager::instance()

namespace ADT {
class ConnectManager : public QObject
{
    Q_OBJECT
    SINGLETON(ConnectManager)
    DECLARE_PROPERTY(ADBDevice*, cutADBDevice)
    DECLARE_PROPERTY(FastbootDevice*, cutFastbootDevice)
    DECLARE_PROPERTY(bool, enableADBCheck)
    DECLARE_PROPERTY(bool, enableFastbootCheck)
public:
    void startCheckDevice();
    void stopCheckDevice();

    QVector<QSharedPointer<Device>> devices(ConnectStatus type = C_ADB) const;    

    Q_INVOKABLE void requestSetCutADBDevice(const QString &deviceCode);
    Q_INVOKABLE void requestSetCutFastbootDevice(const QString &deviceCode);

    Q_INVOKABLE void requestPairDevice(const QString &ipPort, const QString &pairCode);
    Q_INVOKABLE void requestConnectDevice(const QString &ipPort);

signals:
    void deviceDisconnected(QSharedPointer<Device> device);
    void deviceConnected(QSharedPointer<Device> device);
    void deviceRefreshFinish();

private:
    void refreshDevice();

    QVector<QString> getDeviceList(ConnectStatus type = C_ADB);
    bool hasDevice(const QString &deviceCode, ConnectStatus type = C_ADB);
    QSharedPointer<Device> addDevice(const QString &deviceCode, ConnectStatus type = C_ADB);

private:
    QVector<QSharedPointer<ADBDevice>> m_adbDeviceList;
    QVector<QSharedPointer<FastbootDevice>> m_fastbootDeviceList;
    QTimer *m_deviceCheckTimer;
};
} // namespace ADT
