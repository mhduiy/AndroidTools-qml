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

struct DeviceDetailInfo {
    QString manufacturer;   // 厂商
    QString brand;          // 品牌
    QString model;          // 型号
    QString deviceName;     // 设备名
    QString deviceCode;     // 设备代号
    QString systemInfo;     // 系统信息
    QString androidVersion; // 安卓版本
    QString resolving;      //分辨率
    QString dpi;            // dpi
    QString macAddr;        // mac地址
    QString ipAddr;         // ip地址
    QString cpuInfo;        // cpu信息
    QString memory;         // 内存容量
    QString sdkVersion;     // sdk版本
    QString serialNumber;   // 序列号
};

enum AppState
{
    APP_Enable, APP_Disable, APP_Unkown
};

struct AppListInfo
{
    QString packageName;
    AppState state;
    QString versionCode;
    AppListInfo(const QString &_name = "", AppState _state = APP_Unkown, const QString &_versionCode = "000000")
    {
        packageName = _name;
        state = _state;
        versionCode = _versionCode;
    };
};

struct AppDetailInfo {
    QString packageName;
    QString versioName;
    QString installDate;
    QString installUser;
    QString targetsdk;
    QString minsdk;
    QString appid;
    QString path;
    AppDetailInfo(const QString &_packageName = "", const QString &_versioncode = "", const QString &_installDate = "", const QString &_installUser = "", const QString &_targetsdk = "", const QString &_minsdk = "", const QString _appid = "", const QString &_path = "")
    : packageName(_packageName)
    , versioName(_versioncode)
    , installDate(_installDate)
    , installUser(_installUser)
    , targetsdk(_targetsdk)
    , minsdk(_minsdk)
    , appid(_appid)
    , path(_path)
    {

    }
};

class ADBInterface : public QObject
{
    Q_OBJECT
    SINGLETON(ADBInterface)
public slots:
    bool startADBService();
    bool stopADBService();
    bool restartADBService();
public:

    QString adbVersion() const;
    QVector<QString> getDeviceCodes() const;
    DeviceBatteryInfo getBatteryInfo(const QString &code) const;
    DeviceCutActivityInfo getCutActivityInfo(const QString &code) const;
    DeviceDetailInfo getDeviceDetailInfo(const QString &code);
    void killActivity(const QString &packageName, const QString &deviceCode = "");
    void pushKey(const QString &key, const QString &deviceCode);
    void startActivity(const QString &deviceCode, const QString &activity, const QStringList &args);
    QList<AppListInfo> getSoftListInfo(const QString &deviceCode);
    AppDetailInfo getAppDetailInfo(const QString &deviceCode, const QString &packageName);

    bool installApp(const QString &deviceCode, const QString &path);                                   //安装软件
    bool clearData(const QString &deviceCode, const QString &packageName);                             //清除数据
    bool unfreezeApp(const QString &deviceCode, const QString &packageName);                           //解冻软件
    bool freezeApp(const QString &deviceCode, const QString &packageName);                             //冻结软件
    bool uninstallApp(const QString &deviceCode, const QString &packageName);                          //卸载软件
    bool extractApp(const QString &deviceCode, const QString &packagePath, const QString &targetPath); //提取软件
signals:
    void adbStarted();
    void adbKilled();

private:
    QString getDeviceProp(const QString &deviceCode, const QString &prop);

private:
    ADBTools *m_adbTools = ADBTools::instance();
    QString m_adbVersion = "";
};

#endif // ADBINTERFACE_H
