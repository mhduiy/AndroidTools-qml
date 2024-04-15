#ifndef ADBINTERFACE_H
#define ADBINTERFACE_H

#include <QObject>
#include "adbtools.h"
#include <QMap>
#include <QTimer>
#include <QSharedPointer>
#include "../utils/singleton.hpp"

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
    SINGLETON(ADBInterface)
public:

    bool startADBService();
    bool stopADBService();
    bool restartADBService();

    QString adbVersion();
    QVector<QString> getDeviceCodes();
    DeviceBatteryInfo getBatteryInfo(const QString &code);
    DeviceCutActivityInfo getCutActivityInfo(const QString &code);
    void killActivity(const QString &packageName, const QString &deviceCode = "");
signals:

private:
    ADBTools *m_adbTools = ADBTools::instance();

};

#endif // ADBINTERFACE_H
