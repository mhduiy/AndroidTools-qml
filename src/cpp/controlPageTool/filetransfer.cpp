#include "filetransfer.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/Notification.h"
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
    NotificationControl::instance()->send("开始传输，文件很大可能传输失败", NotificationControl::Info);
    ADBTools::instance()->executeCommand(ADBTools::ADB, {"-s", deviceCode, "push", source, targetDir}, "", INT32_MAX);
    NotificationControl::instance()->send("传输完成", NotificationControl::Info);
}

FileTransfer::FileTransfer(QObject *parent)
: QObject(parent)
, m_handler(new FileTransferHandler(this))
, m_handleThread(new QThread(this))
{
    m_handleThread->start();
    m_handler->moveToThread(m_handleThread);
}

void FileTransfer::transmission(const QString &source, const QString &targetDir)
{
    if (targetDir.isEmpty()) {
        NotificationControl::instance()->send("目标路径不能为空", NotificationControl::Error);
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
        NotificationControl::instance()->send("待传输文件不合法", NotificationControl::Error);
    }

    auto cutDevice = ConnectManager::instance()->currentDeviceCode();
    qWarning() << filePath << url.toLocalFile();
    QMetaObject::invokeMethod(m_handler, "transmission", cutDevice, filePath, targetDir);
}
