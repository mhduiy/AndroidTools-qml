#pragma once

#include "device.h"
#include "adbtools.h"
#include <QTimer>
#include <QThread>

namespace ADT {

Q_NAMESPACE

// 前向声明
class ADBDevice;

// @brief ADB命令执行结果
struct CommandResult {
    bool success = false;           // 命令是否成功执行
    int exitCode = -1;              // 进程退出码
    QString output;                 // 标准输出内容
    QString errorOutput;            // 标准错误输出内容
    QString command;                // 执行的命令(用于调试)
    int executionTime = 0;          // 命令执行时间(毫秒)
    
    // 构造函数
    CommandResult() = default;
    CommandResult(bool _success, int _exitCode, const QString& _output, 
                    const QString& _errorOutput = "", const QString& _command = "", 
                    int _executionTime = 0)
        : success(_success), exitCode(_exitCode), output(_output)
        , errorOutput(_errorOutput), command(_command), executionTime(_executionTime) {}
    
    // 判断是否成功的便捷方法
    bool isSuccess() const { return success && exitCode == 0; }
    
    // 获取所有输出(标准输出优先，如果为空则返回错误输出)
    QString getAllOutput() const { 
        return output.isEmpty() ? errorOutput : output; 
    }
};

// @brief 设备当前活动信息
struct DeviceActivityInfo {
    QString windowCode;
    QString cutPackage;
    QString cutActivity;
};

// @brief 设备详细信息
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

    QString maxFrep;    // CPU 最大频率
    QString maxCoreNum; // CPU 核心数量
};

// @brief 应用详细信息
struct AppDetailInfo {
    QString packageName;
    QString versionName;
    QString installDate;
    QString installUser;
    QString targetsdk;
    QString minsdk;
    QString appid;
    QString path;
    
    AppDetailInfo(const QString &_packageName = "", const QString &_versioncode = "", 
                  const QString &_installDate = "", const QString &_installUser = "", 
                  const QString &_targetsdk = "", const QString &_minsdk = "", 
                  const QString _appid = "", const QString &_path = "")
        : packageName(_packageName), versionName(_versioncode), installDate(_installDate)
        , installUser(_installUser), targetsdk(_targetsdk), minsdk(_minsdk)
        , appid(_appid), path(_path) {}
};

// 重新声明枚举用于QML注册
enum AppState {
    Enable, 
    Disable, 
    Unknown
};
Q_ENUM_NS(AppState)

struct AppListInfo {
    QString packageName;
    AppState state;
    QString versionCode;
    
    AppListInfo(const QString &_name = "", AppState _state = AppState::Unknown, 
                const QString &_versionCode = "000000")
        : packageName(_name), state(_state), versionCode(_versionCode) {}
};

// 重新声明枚举用于QML注册
enum ControlType {
    Music,
    Key,
    Broadcast
};
Q_ENUM_NS(ControlType)

// 重新声明枚举用于QML注册
enum MusicControl {
    PreviousSong = 0,
    StopPlay,
    PlayAndPause,
    NextSong,
    VolumeReduce,
    VolumeAdd
};
Q_ENUM_NS(MusicControl)

// 重新声明枚举用于QML注册
enum KeyControl {
    Menu = 0,
    Home,
    Back,
    Power,
    AddBrightness,
    ReduceBrightness,
    Poweroff,
    Reboot,
    RebootToRec,
    RebootToFB,
    Shot,
    Mute,
    StartAssistant,
    CursorToBegin,
    CursorToEnd
};
Q_ENUM_NS(KeyControl)

// 重新声明枚举用于QML注册
enum BroadcastControl {
    NetworkChanged = 0,
    ScreenOpened,
    ScreenClosed,
    LowPower,
    PowerRecovered,
    BootFinish,
    StorageLow,
    StorageRecovered,
    InstallApp,
    WifiChanged1,
    WifiChanged2,
    BatteryLevelChanged,
    InputMethodChanged,
    PowerConnected,
    PowerDisconnected,
    SystemSleep,
    StopSleep,
    WallpaperChanged,
    EarphoneConnected,
    UninstallMedia,
    InstallMedia,
    EnablePowerSave
};
Q_ENUM_NS(BroadcastControl)

// 重新声明枚举用于QML注册
enum ChargingType {
    AC, 
    USB, 
    Wireless, 
    Dock, 
    None
};
Q_ENUM_NS(ChargingType)

// @brief 设备电池信息
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
    uint current;
    float temperature;
};

// 重新声明枚举用于QML注册
enum SoftListType {
    ThirdParty,
    System,
    All
};
Q_ENUM_NS(SoftListType)

class ADBDeviceWorker : public QObject
{
    Q_OBJECT
public:
    explicit ADBDeviceWorker(ADBDevice *device, QObject *parent = nullptr);
    ~ADBDeviceWorker();

public slots:
    void refreshFixInfo();
    void startRefreshRealtimeInfo();
    void stopRefreshRealtimeInfo();

signals:
    void fixInfoRefreshed(const DeviceDetailInfo &info);
    void cutActivityRefreshed(const DeviceActivityInfo &activityInfo);
    void batteryInfoRefreshed(const DeviceBatteryInfo &batteryInfo);

private slots:
    void refreshRealtimeInfo();

private:
    void refreshCutActivityInfo();
    void refreshBatteryInfo();

private:
    QTimer *m_refreshTimer;
    ADBDevice *m_device;
};

class ADBDevice : public Device
{
    Q_OBJECT

    // === 设备详细信息属性 ===
    Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged)
    Q_PROPERTY(QString brand READ brand WRITE setBrand NOTIFY brandChanged)
    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(QString deviceCode READ deviceCode WRITE setDeviceCode NOTIFY deviceCodeChanged)
    Q_PROPERTY(QString systemInfo READ systemInfo WRITE setSystemInfo NOTIFY systemInfoChanged)
    Q_PROPERTY(QString androidVersion READ androidVersion WRITE setAndroidVersion NOTIFY androidVersionChanged)
    Q_PROPERTY(QString resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
    Q_PROPERTY(QString dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(QString macAddr READ macAddr WRITE setMacAddr NOTIFY macAddrChanged)
    Q_PROPERTY(QString ipAddr READ ipAddr WRITE setIpAddr NOTIFY ipAddrChanged)
    Q_PROPERTY(QString cpuInfo READ cpuInfo WRITE setCpuInfo NOTIFY cpuInfoChanged)
    Q_PROPERTY(QString memory READ memory WRITE setMemory NOTIFY memoryChanged)
    Q_PROPERTY(QString sdkVersion READ sdkVersion WRITE setSdkVersion NOTIFY sdkVersionChanged)
    Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber NOTIFY serialNumberChanged)
    Q_PROPERTY(QString maxFreq READ maxFreq WRITE setMaxFreq NOTIFY maxFreqChanged)
    Q_PROPERTY(QString maxCoreNum READ maxCoreNum WRITE setMaxCoreNum NOTIFY maxCoreNumChanged)

    // === 电池信息属性 ===
    Q_PROPERTY(ChargingType chargingType READ chargingType WRITE setChargingType NOTIFY chargingTypeChanged)
    Q_PROPERTY(uint maxChargingCut READ maxChargingCut WRITE setMaxChargingCut NOTIFY maxChargingCutChanged)
    Q_PROPERTY(uint maxChargingVol READ maxChargingVol WRITE setMaxChargingVol NOTIFY maxChargingVolChanged)
    Q_PROPERTY(uint chargingCounter READ chargingCounter WRITE setChargingCounter NOTIFY chargingCounterChanged)
    Q_PROPERTY(int batteryStatus READ batteryStatus WRITE setBatteryStatus NOTIFY batteryStatusChanged)
    Q_PROPERTY(int batteryHealth READ batteryHealth WRITE setBatteryHealth NOTIFY batteryHealthChanged)
    Q_PROPERTY(int batteryLevel READ batteryLevel WRITE setBatteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(int batteryScale READ batteryScale WRITE setBatteryScale NOTIFY batteryScaleChanged)
    Q_PROPERTY(uint batteryVoltage READ batteryVoltage WRITE setBatteryVoltage NOTIFY batteryVoltageChanged)
    Q_PROPERTY(uint batteryCurrent READ batteryCurrent WRITE setBatteryCurrent NOTIFY batteryCurrentChanged)
    Q_PROPERTY(float batteryTemperature READ batteryTemperature WRITE setBatteryTemperature NOTIFY batteryTemperatureChanged)

    // === 当前活动信息属性 ===
    Q_PROPERTY(QString windowCode READ windowCode WRITE setWindowCode NOTIFY windowCodeChanged)
    Q_PROPERTY(QString currentPackage READ currentPackage WRITE setCurrentPackage NOTIFY currentPackageChanged)
    Q_PROPERTY(QString currentActivity READ currentActivity WRITE setCurrentActivity NOTIFY currentActivityChanged)

public:
    explicit ADBDevice(const QString &code, QObject *parent = nullptr);
    ~ADBDevice();

    // === 设备详细信息访问器 ===
    QString manufacturer() const;
    void setManufacturer(const QString &val);

    QString brand() const;
    void setBrand(const QString &val);

    QString model() const;
    void setModel(const QString &val);

    QString deviceName() const;
    void setDeviceName(const QString &val);

    QString deviceCode() const;
    void setDeviceCode(const QString &val);

    QString systemInfo() const;
    void setSystemInfo(const QString &val);

    QString androidVersion() const;
    void setAndroidVersion(const QString &val);

    QString resolution() const;
    void setResolution(const QString &val);

    QString dpi() const;
    void setDpi(const QString &val);

    QString macAddr() const;
    void setMacAddr(const QString &val);

    QString ipAddr() const;
    void setIpAddr(const QString &val);

    QString cpuInfo() const;
    void setCpuInfo(const QString &val);

    QString memory() const;
    void setMemory(const QString &val);

    QString sdkVersion() const;
    void setSdkVersion(const QString &val);

    QString serialNumber() const;
    void setSerialNumber(const QString &val);

    QString maxFreq() const;
    void setMaxFreq(const QString &val);

    QString maxCoreNum() const;
    void setMaxCoreNum(const QString &val);

    // === 电池信息访问器 ===
    ChargingType chargingType() const;
    void setChargingType(ChargingType val);

    uint maxChargingCut() const;
    void setMaxChargingCut(uint val);

    uint maxChargingVol() const;
    void setMaxChargingVol(uint val);

    uint chargingCounter() const;
    void setChargingCounter(uint val);

    int batteryStatus() const;
    void setBatteryStatus(int val);

    int batteryHealth() const;
    void setBatteryHealth(int val);

    int batteryLevel() const;
    void setBatteryLevel(int val);

    int batteryScale() const;
    void setBatteryScale(int val);

    uint batteryVoltage() const;
    void setBatteryVoltage(uint val);

    uint batteryCurrent() const;
    void setBatteryCurrent(uint val);

    float batteryTemperature() const;
    void setBatteryTemperature(float val);

    // === 当前活动信息访问器 ===
    QString windowCode() const;
    void setWindowCode(const QString &val);

    QString currentPackage() const;
    void setCurrentPackage(const QString &val);

    QString currentActivity() const;
    void setCurrentActivity(const QString &val);

    // 获取应用列表
    QList<AppListInfo> getSoftListInfo(SoftListType type = SoftListType::All) const;
    // 获取应用详细信息
    AppDetailInfo getAppDetailInfo(const QString &packageName) const;
    // 安装应用
    bool installApp(const QString &Path, bool r = false, bool s = false, bool d = false, bool g = false);
    // 清除数据
    bool clearData(const QString &packageName);
    // 卸载应用
    bool uninstallApp(const QString &packageName);
    // 冻结应用
    bool freezeApp(const QString &packageName);
    // 解冻应用
    bool unfreezeApp(const QString &packageName);
    // 导出应用
    bool extractApp(const QString &packageName, const QString &targetPath);
    // 停止运行
    bool stopApp(const QString &packageName);
    // 启动应用
    void startApp(const QString &packageName);

    // 停止活动
    Q_INVOKABLE void killActivity(const QString &packageName);
    // 启动活动
    Q_INVOKABLE void startActivity(const QString &activity, const QStringList &args = QStringList());
    
    // 推送按键
    Q_INVOKABLE void pushKey(const QString &key);

    // 截图
    Q_INVOKABLE void shotScreen(const QString &outPath);

    // 设置电量
    Q_INVOKABLE void setBatteryLevel(quint8 level);
    // 停止充电
    Q_INVOKABLE void stopCharge();
    // 恢复充电
    Q_INVOKABLE void restoreCharge();
    // 连接但不充电
    Q_INVOKABLE void connectButNoCharge();
    // 恢复所有电池设置
    Q_INVOKABLE void restoreBatteryAll();

    // 控制设备
    Q_INVOKABLE void control(ControlType controlType, int controlItem);

    // 提取文件
    Q_INVOKABLE void extractFile(const QString &source, const QString &targetDir);
    // 推送文件
    Q_INVOKABLE void pushFile(const QString &source, const QString &targetDir);

    // 设置设备分辨率
    Q_INVOKABLE void setResolution(quint16 width, quint16 height);
    // 设置dpi
    Q_INVOKABLE void setDPI(quint16 dpi);
    // 恢复设备分辨率和dpi
    Q_INVOKABLE void restoreResolutionAndDpi();

    // 断开设备连接
    void requestDisConnect() Q_DECL_OVERRIDE;

    QString executeCommand(const QStringList &args, const QString &writeStr = "", const int timeout = 3000) const;
    CommandResult executeCommandDetailed(const QStringList &args, const QString &writeStr = "", const int timeout = 3000) const;
    
    // 获取设备属性
    QString getDeviceProp(const QString &deviceCode, const QString &prop);

signals:
    // 设备详细信息信号
    void manufacturerChanged(const QString &val);
    void brandChanged(const QString &val);
    void modelChanged(const QString &val);
    void deviceNameChanged(const QString &val);
    void deviceCodeChanged(const QString &val);
    void systemInfoChanged(const QString &val);
    void androidVersionChanged(const QString &val);
    void resolutionChanged(const QString &val);
    void dpiChanged(const QString &val);
    void macAddrChanged(const QString &val);
    void ipAddrChanged(const QString &val);
    void cpuInfoChanged(const QString &val);
    void memoryChanged(const QString &val);
    void sdkVersionChanged(const QString &val);
    void serialNumberChanged(const QString &val);
    void maxFreqChanged(const QString &val);
    void maxCoreNumChanged(const QString &val);

    // 电池信息信号
    void chargingTypeChanged(ChargingType val);
    void maxChargingCutChanged(uint val);
    void maxChargingVolChanged(uint val);
    void chargingCounterChanged(uint val);
    void batteryStatusChanged(int val);
    void batteryHealthChanged(int val);
    void batteryLevelChanged(int val);
    void batteryScaleChanged(int val);
    void batteryVoltageChanged(uint val);
    void batteryCurrentChanged(uint val);
    void batteryTemperatureChanged(float val);

    // 当前活动信息信号
    void windowCodeChanged(const QString &val);
    void currentPackageChanged(const QString &val);
    void currentActivityChanged(const QString &val);

private slots:
    void onFixInfoRefreshed(const DeviceDetailInfo &info);
    void onCutActivityRefreshed(const DeviceActivityInfo &activityInfo);
    void onBatteryInfoRefreshed(const DeviceBatteryInfo &batteryInfo);

private:
    void updateDetailInfoFromStruct(const DeviceDetailInfo &info);
    void updateBatteryInfoFromStruct(const DeviceBatteryInfo &info);
    void updateActivityInfoFromStruct(const DeviceActivityInfo &info);

    void initWorker();
    void initData();

private:
    ADBDeviceWorker *m_worker;
    QThread *m_workerThread;
    ADBTools *m_adbTools;

    // === 设备详细信息成员变量 ===
    QString m_manufacturer;
    QString m_brand;
    QString m_model;
    QString m_deviceName;
    QString m_deviceCode;
    QString m_systemInfo;
    QString m_androidVersion;
    QString m_resolution;
    QString m_dpi;
    QString m_macAddr;
    QString m_ipAddr;
    QString m_cpuInfo;
    QString m_memory;
    QString m_sdkVersion;
    QString m_serialNumber;
    QString m_maxFreq;
    QString m_maxCoreNum;

    // === 电池信息成员变量 ===
    ChargingType m_chargingType = ChargingType::None;
    uint m_maxChargingCut = 0;
    uint m_maxChargingVol = 0;
    uint m_chargingCounter = 0;
    int m_batteryStatus = 0;
    int m_batteryHealth = 0;
    int m_batteryLevel = 0;
    int m_batteryScale = 0;
    uint m_batteryVoltage = 0;
    uint m_batteryCurrent = 0;
    float m_batteryTemperature = 0.0f;

    // === 当前活动信息成员变量 ===
    QString m_windowCode;
    QString m_currentPackage;
    QString m_currentActivity;
};

} // namespace ADT