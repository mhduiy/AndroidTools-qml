#include "flashtools.h"
#include "../adb/adbtools.h"
#include <qcontainerfwd.h>
#include <QProcessEnvironment>
#include <qprocess.h>
#include <qtimer.h>
#include <qtpreprocessorsupport.h>
#include "../utils/Notification.h"
#include <QTimer>

FlashTools::FlashTools(QObject *parent)
    : QObject(parent)
{

}

void FlashTools::startBoot(const QString &deviceCode, const QString &bootImage)
{
    QStringList args;
    args << "-s" << deviceCode << "boot" << bootImage;
    executeCommand(ADBTools::FASTBOOT, args);
}

void FlashTools::flash(const QString &deviceCode, const QString &partName, const QString &imagePath)
{
    QStringList args;
    args << "-s" << deviceCode << "flash" << partName << imagePath;
    executeCommand(ADBTools::FASTBOOT, args);
}

void FlashTools::clear(const QString &deviceCode, const QString &partName)
{
    QStringList args;
    args << "-s" << deviceCode << "erase" << partName;
    executeCommand(ADBTools::FASTBOOT, args);
}

void FlashTools::executeScript(const QString &deviceCode, const QString &scrptPath, const QString &workDir)
{

}

void FlashTools::flashToXiaomi(const QString &deviceCode, const QString &packagePath)
{

}

void FlashTools::unzip(const QString &zipPath, const QString &tarPath)
{

}

void FlashTools::executeCommand(ADBTools::APP appType, const QStringList &args)
{
    Q_UNUSED(appType)

    QString programName;
    QStringList startArgs;

    // konsole --hold  -e  adb 
#ifdef Q_OS_LINUX
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString de = env.value("XDG_CURRENT_DESKTOP", "").toLower();
    if (de == "kde") {
        programName = "konsole";
        startArgs <<  "--hold" << "-e" << "fastboot" << args;
    }


#elif defined(Q_OS_WIN)
    QProcess::startDetached("cmd.exe");
#elif defined(Q_OS_MAC)
    QProcess::startDetached("Terminal.app");
#endif

    NotificationControl::instance()->send("两秒后在系统终端中执行命令");

    QTimer::singleShot(2000, this, [this, programName, startArgs](){
        QProcess::startDetached(programName, startArgs);
    });
}