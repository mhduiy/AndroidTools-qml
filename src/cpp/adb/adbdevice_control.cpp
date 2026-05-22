#include "adbdevice.h"
#include "src/cpp/adb/adbtools.h"
#include <QDebug>
#include <QProcess>
#include <QDesktopServices>
#include <QDateTime>
#include <QFile>
#include <QUrl>

namespace ADT {

void ADBDevice::shotScreen(const QString &outPath)
{
    QString filePath = outPath + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
    QStringList args;
    args << "-s" << code() << "exec-out" << "screencap" << "-p";

    QProcess process;
    process.start("adb", args);
    if (process.waitForFinished()) {
        QByteArray output = process.readAllStandardOutput();
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(output);
            file.close();
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        } else {
            qWarning() << "Failed to open file for writing:" << filePath;
        }
    } else {
        qWarning() << "ADB command failed to finish";
    }
}

void ADBDevice::pushKey(const QString &key)
{
    QStringList args;
    args << "-s" << code() << "shell" << "input" << "keyevent" << key;
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::setBatteryLevel(quint8 level)
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "set" << "level" << QString::number(level);
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::stopCharge()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "set" << "ac" << "0";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::restoreCharge()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "reset";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::connectButNoCharge()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "set" << "usb" << "0";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::restoreBatteryAll()
{
    QStringList args;
    args << "-s" << code() << "shell" << "dumpsys" << "battery" << "reset";
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::control(ControlType controlType, int controlItem)
{
    static const QHash<int, QString> MusicControlArgsMap {
        { 0, "shell input keyevent 88" },
        { 1, "shell input keyevent 86" },
        { 2, "shell input keyevent 85" },
        { 3, "shell input keyevent 87" },
        { 4, "shell input keyevent 25" },
        { 5, "shell input keyevent 24" },
    };

    static const QHash<int, QString> KeyControlArgsMap {
        { 0, "shell input keyevent 82" },
        { 1, "shell input keyevent 3" },
        { 2, "shell input keyevent 4" },
        { 3, "shell input keyevent 26" },
        { 4, "shell input keyevent 221" },
        { 5, "shell input keyevent 220" },
        { 6, "shutdown" },
        { 7, "reboot" },
        { 8, "reboot recovery" },
        { 9, "reboot bootloader" },
        { 10, "shell input keyevent 27" },
        { 11, "shell input keyevent 164" },
        { 12, "shell input keyevent 231" },
        { 13, "shell input keyevent 122" },
        { 14, "shell input keyevent 123" }
    };

    static const QHash<int, QString> BoardControlArgsMap {
        { 0, "shell am broadcast -a android.net.conn.CONNECTIVITY_CHANGE" },
        { 1, "shell am broadcast -a android.intent.action.SCREEN_ON" },
        { 2, "shell am broadcast -a android.intent.action.SCREEN_OFF" },
        { 3, "shell am broadcast -a android.intent.action.BATTERY_LOW" },
        { 4, "shell am broadcast -a android.intent.action.BATTERY_OKAY" },
        { 5, "shell am broadcast -a android.intent.action.BOOT_COMPLETED" },
        { 6, "shell am broadcast -a android.intent.action.DEVICE_STORAGE_LOW" },
        { 7, "shell am broadcast -a android.intent.action.DEVICE_STORAGE_OK" },
        { 8, "shell am broadcast -a android.intent.action.PACKAGE_ADDED" },
        { 9, "shell am broadcast -a android.net.wifi.STATE_CHANGE" },
        { 10, "shell am broadcast -a android.net.wifi.WIFI_STATE_CHANGED" },
        { 11, "shell am broadcast -a android.intent.action.BATTERY_CHANGED" },
        { 12, "shell am broadcast -a android.intent.action.INPUT_METHOD_CHANGED" },
        { 13, "shell am broadcast -a android.intent.action.ACTION_POWER_CONNECTED" },
        { 14, "shell am broadcast -a android.intent.action.ACTION_POWER_DISCONNECTED" },
        { 15, "shell am broadcast -a android.intent.action.DREAMING_STARTED" },
        { 16, "shell am broadcast -a android.intent.action.DREAMING_STOPPED" },
        { 17, "shell am broadcast -a android.intent.action.WALLPAPER_CHANGED" },
        { 18, "shell am broadcast -a android.intent.action.HEADSET_PLUG" },
        { 19, "shell am broadcast -a android.intent.action.MEDIA_UNMOUNTED" },
        { 20, "shell am broadcast -a android.intent.action.MEDIA_MOUNTED" },
        { 21, "shell am broadcast -a android.os.action.POWER_SAVE_MODE_CHANGED" }
    };

    QStringList args;
    args << "-s" << code();

    if (controlType == ControlType::Music) {
        QString cmd = MusicControlArgsMap.value(controlItem);
        if (!cmd.isEmpty()) {
            args << cmd.split(' ');
        }
    } else if (controlType == ControlType::Key) {
        QString cmd = KeyControlArgsMap.value(controlItem);
        if (!cmd.isEmpty()) {
            args << cmd.split(' ');
        }
    } else if (controlType == ControlType::Broadcast) {
        QString cmd = BoardControlArgsMap.value(controlItem);
        if (!cmd.isEmpty()) {
            args << cmd.split(' ');
        }
    }

    if (args.size() > 2) {
        m_adbTools->executeCommand(ADBTools::ADB, args);
    }
}

void ADBDevice::extractFile(const QString &source, const QString &targetDir)
{
    QStringList args;
    args << "-s" << code() << "pull" << source << targetDir;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}

void ADBDevice::pushFile(const QString &source, const QString &targetDir)
{
    QStringList args;
    args << "-s" << code() << "push" << source << targetDir;
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}

void ADBDevice::setResolution(quint16 width, quint16 height)
{
    QStringList args;
    args << "-s" << code() << "shell" << "wm" << "size" << QString("%1x%2").arg(width).arg(height);
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::setDPI(quint16 dpi)
{
    QStringList args;
    args << "-s" << code() << "shell" << "wm" << "density" << QString::number(dpi);
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::restoreResolutionAndDpi()
{
    QStringList args1, args2;
    args1 << "-s" << code() << "shell" << "wm" << "size" << "reset";
    args2 << "-s" << code() << "shell" << "wm" << "density" << "reset";
    m_adbTools->executeCommand(ADBTools::ADB, args1);
    m_adbTools->executeCommand(ADBTools::ADB, args2);
}

} // namespace ADT
