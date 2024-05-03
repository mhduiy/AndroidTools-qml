#include "filetransfer.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/Notification.h"
#include "../utils/utils.hpp"
#include <QFile>
#include <QUrl>

FileTransfer::FileTransfer(QObject *parent) : QObject(parent)
{

}

void FileTransfer::transmission(const QString &source, const QString &targetDir)
{
    if (targetDir.isEmpty()) {
        NotificationControl::instance()->send("目标路径不能为空", NotificationControl::Error);
        return;
    }
    if (!QFile::exists(source)) {
        NotificationControl::instance()->send("待传输文件不合法", NotificationControl::Error);
    }

    QString filePath;
    QUrl url(source);
    if (url.isLocalFile()) {
        filePath = url.toLocalFile();
    } else {
        filePath = source;
    }

    auto cutDevice = ConnectManager::instance()->currentDeviceCode();
    qWarning() << filePath << url.isLocalFile();

    auto operatorFunc = [&cutDevice, &filePath, &targetDir](){
        NotificationControl::instance()->send("开始传输，文件很大可能传输失败", NotificationControl::Info);
        ADBTools::instance()->executeCommand(ADBTools::ADB, {"-s", cutDevice, "push", filePath, targetDir}, "", INT32_MAX);
        NotificationControl::instance()->send("传输完成", NotificationControl::Info);
    };

    asyncOperator(operatorFunc);
}
