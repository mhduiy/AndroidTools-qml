#ifndef DEVICECONTROL_H
#define DEVICECONTROL_H

#include <QObject>
#include <QHash>
#include "../utils/singleton.hpp"
#include "../adb/adbtools.h"

class DeviceControl : public QObject
{
    Q_OBJECT
    SINGLETON(DeviceControl)

public:
    enum ControlType {
        CTRL_Music,
        CTRL_Key,
        CTRL_BoardCast
    };
    Q_ENUM(ControlType)

    enum MusicControlEnum{
        PreviouSong = 0,
        StopPlay,
        PlayAndPause,
        NextSong,
        VolumeReduce,
        VolumeAdd
    };
    Q_ENUM(MusicControlEnum)

    enum KeyControlEnum{
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
    Q_ENUM(KeyControlEnum)

    enum BoardcastControlEnum{
        NetWorkChanged = 0,
        ScrennOpened,
        ScrennOffed,
        LowPower,
        PowerRec,
        BootFinish,
        StorageLow,
        StorageRec,
        InstallApp,
        WifiChanged1,
        WifiChanged2,
        BatteryLevelChanged,
        InputMethodChanged,
        PowerConnected,
        PowerDisConnected,
        SystemSleep,
        StopSleep,
        WallpaperChanged,
        EarphoneConnected,
        UninstallMedia,
        InstallMedia,
        EnablePowerSave
    };
    Q_ENUM(BoardcastControlEnum)

    const static QHash<int, QString> MusicControlArgsMap;
    const static QHash<int, QString> KeyControlArgsMap;
    const static QHash<int, QString> BoardControlArgsMap;
public:
    Q_INVOKABLE void control(ControlType controlType, int controlItem);

private:
    ADBTools *m_adbtools;
};

#endif // DEVICECONTROL_H
