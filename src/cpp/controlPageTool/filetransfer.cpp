#include "filetransfer.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/notificationcontroller.h"
#include "../utils/utils.hpp"
#include <QFile>
#include <QUrl>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qthread.h>

FileTransferHandler::FileTransferHandler(QObject *parent)
:QObject(parent)
{

}

void FileTransferHandler::transmission(QString deviceCode, QString source, QString targetDir)
{
    NotificationController::instance()->send("开始传输", "文件很大可能传输失败", NotificationController::Info);
    ADBTools::instance()->executeCommand(ADBTools::ADB, {"-s", deviceCode, "push", source, targetDir}, "", INT32_MAX);
    NotificationController::instance()->send("传输完成", "传输完成", NotificationController::Info);
}

FileTransfer::FileTransfer(QObject *parent)
: QObject(parent)
, m_handler(new FileTransferHandler)
, m_handleThread(new QThread(this))
{
    m_handleThread->start();
    m_handler->moveToThread(m_handleThread);
}

FileTransfer::~FileTransfer()
{
    qInfo() << "FileTransfer Thread exiting";
    m_handleThread->quit();
    m_handleThread->wait();
    qInfo() << "FileTransfer Thread exited";
}

void FileTransfer::transmission(const QString &source, const QString &targetDir)
{
    if (targetDir.isEmpty()) {
        NotificationController::instance()->send("传输失败", "目标路径不能为空", NotificationController::Error);
        return;
    }

    QString filePath;
    QUrl url(source);
    if (url.isLocalFile()) {
        filePath = url.toLocalFile();
    } else {
        filePath = source;
    }

    if (!QFile::exists(filePath)) {
        NotificationController::instance()->send("传输失败", "待传输文件不合法", NotificationController::Error);
    }

    auto cutDevice = ConnectManager::instance()->currentDeviceCode();
    qWarning() << filePath << url.toLocalFile();
    QMetaObject::invokeMethod(m_handler, "transmission", Q_ARG(QString, cutDevice), Q_ARG(QString, filePath), Q_ARG(QString, targetDir));
}
