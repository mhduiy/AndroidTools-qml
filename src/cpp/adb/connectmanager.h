#ifndef CONNECTMANAGER_H
#define CONNECTMANAGER_H

#include <QObject>
#include "../utils/singleton.hpp"
#include "../adb/adbtools.h"
#include "../adb/adbinterface.h"
#include "../components/deivicelistviewmodel.h"

class ConnectManager : public QObject
{
    Q_OBJECT
    SINGLETON(ConnectManager)
public:
    QString currentDeviceCode();
    bool setCurrentDeviceCode(const QString &code);

    /**
     * @brief startCheckDevice 开始检测设备
     */
    void startCheckDevice();
    void stopCheckDevice();

    DeviceBaceInfo getDeviceBaceInfo(const QString &code = "");
    DeviceBatteryInfo getDeviceBatteryInfo(const QString &code = "");
    DeviceCutActivityInfo getDeviceCutActivityInfo(const QString &code = "");

signals:
    void deviceDisconnected(const QString &code);
    void deviceConnected(const QString &code);

    void deviceRefreshFinish();
    void currentDeviceChanged(const QString &code);

private:
    void refreshDevice();

private:
    QVector<QString> m_deviceCodeSet;
    QString m_currentDeviceCode;
    ADBTools *m_adbTools = ADBTools::instance();
    ADBInterface *m_adbInterface = ADBInterface::instance();

    QMap<QString, DeviceBaceInfo> m_deviceBaceInfoMap;
    QMap<QString, DeviceBatteryInfo> m_deviceBatteryInfoMap;
    QMap<QString, DeviceCutActivityInfo> m_deviceCutActivityInfoMap;

    DeviceListviewModel *m_deviceListviewModel;
    QTimer *m_deviceCheckTimer;
};

#endif // CONNECTMANAGER_H
