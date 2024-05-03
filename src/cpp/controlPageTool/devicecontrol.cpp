#include "devicecontrol.h"
#include <QtQml>
#include <QtConcurrent/QtConcurrentRun>
#include "../adb/connectmanager.h"
#include "../utils/Notification.h"
#include "../utils/utils.hpp"

const QHash<int, QString> DeviceControl::MusicControlArgsMap
{
    { PreviouSong    , "shell input keyevent 88" },
    { StopPlay       , "shell input keyevent 86" },
    { PlayAndPause   , "shell input keyevent 85" },
    { NextSong       , "shell input keyevent 87" },
    { VolumeAdd      , "shell input keyevent 24" },
    { VolumeReduce   , "shell input keyevent 25" },
};

const QHash<int, QString> DeviceControl::KeyControlArgsMap
{
    { Menu               , "shell input keyevent 82" },
    { Home               , "shell input keyevent 3" },
    { Back               , "shell input keyevent 4" },
    { Power              , "shell input keyevent 26" },
    { AddBrightness      , "shell input keyevent 221" },
    { ReduceBrightness   , "shell input keyevent 220" },
    { Poweroff           , "shutdown" },
    { Reboot             , "reboot" },
    { RebootToRec        , "reboot recovery" },
    { RebootToFB         , "reboot bootloader" },
    { Shot               , "shell input keyevent 27" },
    { Mute               , "shell input keyevent 164" },
    { StartAssistant     , "shell input keyevent 231" },
    { CursorToBegin      , "shell input keyevent 122" },
    { CursorToEnd        , "shell input keyevent 123" }
};

const QHash<int, QString> DeviceControl::BoardControlArgsMap
{
    { NetWorkChanged        , "shell am broadcast -a android.net.conn.CONNECTIVITY_CHANGE" },
    { ScrennOpened          , "shell am broadcast -a android.intent.action.SCREEN_ON" },
    { ScrennOffed           , "shell am broadcast -a android.intent.action.SCREEN_OFF" },
    { LowPower              , "shell am broadcast -a android.intent.action.BATTERY_LOW" },
    { PowerRec              , "shell am broadcast -a android.intent.action.BATTERY_OKAY" },
    { BootFinish            , "shell am broadcast -a android.intent.action.BOOT_COMPLETED" },
    { StorageLow            , "shell am broadcast -a android.intent.action.DEVICE_STORAGE_LOW" },
    { StorageRec            , "shell am broadcast -a android.intent.action.DEVICE_STORAGE_OK" },
    { InstallApp            , "shell am broadcast -a android.intent.action.PACKAGE_ADDED" },
    { WifiChanged1          , "shell am broadcast -a android.net.wifi.STATE_CHANGE" },
    { WifiChanged2          , "shell am broadcast -a android.net.wifi.WIFI_STATE_CHANGED" },
    { BatteryLevelChanged   , "shell am broadcast -a android.intent.action.BATTERY_CHANGED" },
    { InputMethodChanged    , "shell am broadcast -a android.intent.action.INPUT_METHOD_CHANGED" },
    { PowerConnected        , "shell am broadcast -a android.intent.action.ACTION_POWER_CONNECTED" },
    { PowerDisConnected     , "shell am broadcast -a android.intent.action.ACTION_POWER_DISCONNECTED" },
    { SystemSleep           , "shell am broadcast -a android.intent.action.DREAMING_STARTED" },
    { StopSleep             , "shell am broadcast -a android.intent.action.DREAMING_STOPPED" },
    { WallpaperChanged      , "shell am broadcast -a android.intent.action.WALLPAPER_CHANGED" },
    { EarphoneConnected     , "shell am broadcast -a android.intent.action.HEADSET_PLUG" },
    { UninstallMedia        , "shell am broadcast -a android.intent.action.MEDIA_UNMOUNTED" },
    { InstallMedia          , "shell am broadcast -a android.intent.action.MEDIA_MOUNTED" },
    { EnablePowerSave       , "shell am broadcast -a android.os.action.POWER_SAVE_MODE_CHANGED" }
};

DeviceControl::DeviceControl(QObject *parent) : QObject(parent)
{
    m_adbtools = ADBTools::instance();
}

void DeviceControl::control(ControlType controlType, int controlItem)
{
    auto controlFunc = [this, controlType, controlItem](){
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
        QStringList args{"-s", cutDevice};
        if (controlType == CTRL_Music) {
            args << MusicControlArgsMap.value(controlItem).split(' ');
        } else if (controlType == CTRL_Key) {
            args << KeyControlArgsMap.value(controlItem).split(' ');
        } else if (controlType == CTRL_BoardCast) {
            args << BoardControlArgsMap.value(controlItem).split(' ');
        } else {
            return;
        }
        if (args.size() <= 2) {
            NotificationControl::instance()->send("索引错误，请联系开发者", NotificationControl::Error);
            return;
        }
        m_adbtools->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(controlFunc);
}
