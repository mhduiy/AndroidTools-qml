#ifndef ADBINTERFACE_H
#define ADBINTERFACE_H

#include <QObject>
#include "adbtools.h"
#include <QMap>
#include <QTimer>

struct DeviceBaceInfo{
    QString deviceName;
    QString ip;
    int battery;
    bool isConnected;
    bool isWireless;
    bool isCharging;

    DeviceBaceInfo(QString deviceName, QString ip, int battery, bool isConnected, bool isWireless, bool isCharging)
        : deviceName(deviceName),
        ip(ip),
        battery(battery),
        isConnected(isConnected),
        isWireless(isWireless),
        isCharging(isCharging){}
    DeviceBaceInfo(){}
};

enum ChargingType {
    AC, USB, Wireless, Dock
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

    DeviceBaceInfo *getDeviceBaceInfo(const QString &code);
    DeviceBatteryInfo *getDeviceBatteryInfo(const QString &code);
    DeviceCutActivityInfo *getDeviceCutActivityInfo(const QString &code);

signals:
    void deviceDisconnected(QString code);
    void deviceConnected(QString code);

private:
    ADBTools *m_adbtools = ADBTools::instance();
    QVector<QString> m_deviceCodeSet;
    QMap<QString, DeviceBaceInfo *> m_deviceBaceInfoMap;
    QMap<QString, DeviceBatteryInfo *> m_deviceBatteryInfoMap;
    QMap<QString, DeviceCutActivityInfo *> m_deviceCutActivityInfoMap;
    QTimer *m_deviceCheckTimer;

};

#endif // ADBINTERFACE_H
