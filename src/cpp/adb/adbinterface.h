#ifndef ADBINTERFACE_H
#define ADBINTERFACE_H

#include <QObject>
#include "adbtools.h"
#include <QMap>
#include <QTimer>
#include <QSharedPointer>

struct DeviceBaceInfo{
    QString deviceName;
    QString ip;
    int battery;
    bool isConnected;
    bool isWireless;
    bool isCharging;
    QString deviceCode;

    DeviceBaceInfo(QString deviceName, QString ip, int battery, bool isConnected, bool isWireless, bool isCharging, const QString &deviceCode = "")
        : deviceName(deviceName),
        ip(ip),
        battery(battery),
        isConnected(isConnected),
        isWireless(isWireless),
        isCharging(isCharging),
        deviceCode(deviceCode){}
    DeviceBaceInfo(){}
};

enum ChargingType {
    AC, USB, Wireless, Dock, None
};

struct DeviceBatteryInfo {
    ChargingType chargingType;
    uint maxChargingCut;
    uint maxChargingVol;
    uint chargingCounter;
    int8_t status;
    int8_t health;
    int8_t level;
    int8_t scale;
    uint voltage;
    float temperature;
};

struct DeviceCutActivityInfo {
    QString windowCode;
    QString cutPackage;
    QString cutActivity;
};

class ADBInterface : public QObject
{
    Q_OBJECT
public:
    explicit ADBInterface(QObject *parent = nullptr);

    bool startADBService();
    bool stopADBService();
    bool restartADBService();

    QString adbVersion();

    void refreshDeviceStatus();
    void startRefreshDevice();
    void stopRefreshDevice();

    QSharedPointer<DeviceBaceInfo> getDeviceBaceInfo(const QString &code);
    QSharedPointer<DeviceBatteryInfo> getDeviceBatteryInfo(const QString &code);
    QSharedPointer<DeviceCutActivityInfo> getDeviceCutActivityInfo(const QString &code);

    QVector<QString> getDeviceCodeSet();

signals:
    void deviceDisconnected(QString code);
    // 收到信号后不能马上获取设备的详细信息
    void deviceConnected(QString code);
    void deviceStatusUpdateFinish();

private:
    ADBTools *m_adbtools = ADBTools::instance();
    QVector<QString> m_deviceCodeSet;
    QMap<QString, QSharedPointer<DeviceBaceInfo>> m_deviceBaceInfoMap;
    QMap<QString, QSharedPointer<DeviceBatteryInfo>> m_deviceBatteryInfoMap;
    QMap<QString, QSharedPointer<DeviceCutActivityInfo>> m_deviceCutActivityInfoMap;
    QTimer *m_deviceCheckTimer;

};

#endif // ADBINTERFACE_H
