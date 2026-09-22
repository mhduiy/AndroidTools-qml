#include "flashtools.h"

#include "../adb/adbtools.h"
#include "../utils/notificationcontroller.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QProcessEnvironment>
#include <QTextStream>
#include <QTimer>
#include <QUrl>

namespace ADT {

FlashTools::FlashTools(QObject *parent)
    : QObject(parent)
{

}

void FlashTools::startBoot(const QString &bootImage)
{
    QUrl url(bootImage);
    executeCommand({"boot", url.toLocalFile()}, QFileInfo(bootImage).absolutePath());
}

void FlashTools::flash(const QString &partName, const QString &imagePath)
{
    QUrl url(imagePath);
    executeCommand({"flash", partName, url.toLocalFile()}, QFileInfo(imagePath).absolutePath());
}

void FlashTools::clear(const QString &partName)
{
    executeCommand({"erase", partName});
}

void FlashTools::executeCommand(const QStringList &args, const QString &workDir)
{
    const QString command = "fastboot";
    const QString workingDir = workDir.isEmpty() ? QDir::currentPath() : workDir;

    QString program;
    QStringList terminalArgs;

#ifdef Q_OS_LINUX
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString de = env.value("XDG_CURRENT_DESKTOP", "").toLower();

    // 检测常用的Linux终端
    if (QFile::exists("/usr/bin/konsole") || de == "kde") {
        program = "konsole";
        terminalArgs << "--hold" << "--workdir" << workingDir << "-e" << command;
    } else if (QFile::exists("/usr/bin/gnome-terminal")) {
        program = "gnome-terminal";
        terminalArgs << "--working-directory" << workingDir << "--" << command;
    } else if (QFile::exists("/usr/bin/xfce4-terminal")) {
        program = "xfce4-terminal";
        terminalArgs << "--hold" << "--working-directory" << workingDir << "-x" << command;
    } else if (QFile::exists("/usr/bin/xterm")) {
        program = "xterm";
        terminalArgs << "-hold" << "-e" << QString("cd %1 && %2").arg(workingDir, command);
    } else {
        NotificationController::instance()->send("错误", "未找到支持的终端模拟器", NotificationController::Error);
        return;
    }

#elif defined(Q_OS_WIN)
    program = "cmd.exe";
    terminalArgs << "/k" << QString("cd /d \"%1\" && %2").arg(workingDir, command);

#elif defined(Q_OS_MAC)
    QString scriptPath = QDir::temp().filePath("run_command.command");
    QFile script(scriptPath);
    if (script.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&script);
        stream << "#!/bin/bash\n";
        stream << QString("cd \"%1\"\n").arg(workingDir);
        stream << QString("%1 %2\n").arg(command, args.join(" "));
        stream << "echo \"\nPress any key to exit...\"\n";
        stream << "read -n 1\n";
        script.close();

        QFile::setPermissions(scriptPath, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);

        program = "open";
        if (QFile::exists("/Applications/iTerm.app")) {
            terminalArgs << "-a" << "iTerm" << scriptPath;
        } else {
            terminalArgs << "-a" << "Terminal" << scriptPath;
        }
    } else {
        NotificationController::instance()->send("错误", "无法创建临时脚本文件", NotificationController::Error);
        return;
    }
#endif

    // 添加用户命令参数（对于macOS，参数已经写入脚本）
#ifndef Q_OS_MAC
    terminalArgs << args;
#endif

    NotificationController::instance()->send("执行中", QString("即将在%1中执行命令").arg(program), NotificationController::Info);

    QTimer::singleShot(500, this, [this, program, terminalArgs]() {
        bool success = QProcess::startDetached(program, terminalArgs);
        if (!success) {
            NotificationController::instance()->send("错误", "命令执行失败", NotificationController::Error);
        }
    });
}

} // namespace ADT
