#include "../adb/adbtools.h"
#include "../utils/notificationcontroller.h"
#include "flashtools.h"

#include <QProcessEnvironment>
#include <QTimer>
#include <QUrl>
#include <QDebug>

namespace ADT {

FlashTools::FlashTools(QObject *parent)
    : QObject(parent)
{

}

void FlashTools::startBoot(const QString &deviceCode, const QString &bootImage)
{
    QStringList args;
    QUrl url(bootImage);
    args << "boot" << url.toLocalFile();
    executeCommand(ADBTools::FASTBOOT, args, QFileInfo(bootImage).absolutePath());
}

void FlashTools::flash(const QString &deviceCode, const QString &partName, const QString &imagePath)
{
    QStringList args;
    QUrl url(imagePath);
    args << "flash" << partName << url.toLocalFile();
    executeCommand(ADBTools::FASTBOOT, args, QFileInfo(imagePath).absolutePath());
}

void FlashTools::clear(const QString &deviceCode, const QString &partName)
{
    QStringList args;
    args << "erase" << partName;
    executeCommand(ADBTools::FASTBOOT, args);
}

void FlashTools::executeScript(const QString &deviceCode, const QString &scriptPath, const QString &workDir)
{
    if (scriptPath.isEmpty()) {
        NotificationController::instance()->send("错误", "脚本路径不能为空", NotificationController::Error);
        return;
    }

    QFile script(scriptPath);
    if (!script.exists()) {
        NotificationController::instance()->send("错误", "脚本文件不存在", NotificationController::Error);
        return;
    }

    QString actualWorkDir = workDir.isEmpty() ? QFileInfo(scriptPath).absolutePath() : workDir;
    
    QStringList args;
    args << scriptPath;
    executeCommand(ADBTools::FASTBOOT, args, actualWorkDir);
}

void FlashTools::flashToXiaomi(const QString &deviceCode, const QString &packagePath)
{
    if (packagePath.isEmpty()) {
        NotificationController::instance()->send("错误", "刷机包路径不能为空", NotificationController::Error);
        return;
    }

    QFileInfo packageInfo(packagePath);
    if (!packageInfo.exists()) {
        NotificationController::instance()->send("错误", "刷机包文件不存在", NotificationController::Error);
        return;
    }

    QStringList args;
    args << "update" << packagePath;
    executeCommand(ADBTools::FASTBOOT, args, packageInfo.absolutePath());
}

void FlashTools::executeCommand(const QString &command, const QStringList &args, const QString &workDir)
{
    QString program;
    QStringList terminalArgs;
    
#ifdef Q_OS_LINUX
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString de = env.value("XDG_CURRENT_DESKTOP", "").toLower();
    
    // 检测常用的Linux终端
    if (QFile::exists("/usr/bin/konsole") || de == "kde") {
        program = "konsole";
        terminalArgs << "--hold" << "--workdir" << workDir << "-e" << command;
    } else if (QFile::exists("/usr/bin/gnome-terminal")) {
        program = "gnome-terminal";
        terminalArgs << "--working-directory" << workDir << "--" << command;
    } else if (QFile::exists("/usr/bin/xfce4-terminal")) {
        program = "xfce4-terminal";
        terminalArgs << "--hold" << "--working-directory" << workDir << "-x" << command;
    } else if (QFile::exists("/usr/bin/xterm")) {
        program = "xterm";
        terminalArgs << "-hold" << "-e" << QString("cd %1 && %2").arg(workDir, command);
    } else {
        NotificationController::instance()->send("错误", "未找到支持的终端模拟器", NotificationController::Error);
        return;
    }
    
#elif defined(Q_OS_WIN)
    program = "cmd.exe";
    terminalArgs << "/k" << QString("cd /d \"%1\" && %2").arg(workDir, command);
    
#elif defined(Q_OS_MAC)
    QString scriptPath = QDir::temp().filePath("run_command.command");
    QFile script(scriptPath);
    if (script.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&script);
        stream << "#!/bin/bash\n";
        stream << QString("cd \"%1\"\n").arg(workDir);
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

    qDebug() << "Executing command in terminal:" << program << terminalArgs;
    qDebug() << "Working directory:" << workDir;
    
    NotificationController::instance()->send("执行中", QString("即将在%1中执行命令").arg(program), NotificationController::Info);
    
    QTimer::singleShot(500, this, [this, program, terminalArgs]() {
        bool success = QProcess::startDetached(program, terminalArgs);
        if (!success) {
            NotificationController::instance()->send("错误", "命令执行失败", NotificationController::Error);
        }
    });
}

void FlashTools::unzip(const QString &zipPath, const QString &tarPath)
{
    if (zipPath.isEmpty() || tarPath.isEmpty()) {
        NotificationController::instance()->send("错误", "压缩包路径或目标路径不能为空", NotificationController::Error);
        return;
    }

    QFileInfo zipInfo(zipPath);
    if (!zipInfo.exists()) {
        NotificationController::instance()->send("错误", "压缩包文件不存在", NotificationController::Error);
        return;
    }

    QDir targetDir(tarPath);
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(".")) {
            NotificationController::instance()->send("错误", "无法创建目标目录", NotificationController::Error);
            return;
        }
    }

#ifdef Q_OS_WIN
    QStringList args;
    args << "x" << "-y" << "-o" + tarPath << zipPath;
    executeCommand("7z", args, tarPath);
#else
    QStringList args;
    args << "-xf" << zipPath << "-C" << tarPath;
    executeCommand("tar", args, tarPath);
#endif
}

void FlashTools::executeCommand(ADBTools::APP appType, const QStringList &args, const QString &workDir)
{
    QString program;
    QStringList terminalArgs;
    QString command = (appType == ADBTools::FASTBOOT) ? "fastboot" : "adb";
    
    // 设置工作目录
    QString workingDir = workDir.isEmpty() ? QDir::currentPath() : workDir;
    
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

    qDebug() << "Executing command in terminal:" << program << terminalArgs;
    qDebug() << "Working directory:" << workingDir;
    
    NotificationController::instance()->send("执行中", QString("即将在%1中执行命令").arg(program), NotificationController::Info);
    
    QTimer::singleShot(500, this, [this, program, terminalArgs]() {
        bool success = QProcess::startDetached(program, terminalArgs);
        if (!success) {
            NotificationController::instance()->send("错误", "命令执行失败", NotificationController::Error);
        }
    });
}

} // namespace ADT