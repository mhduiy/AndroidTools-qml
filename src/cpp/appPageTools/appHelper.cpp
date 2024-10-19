#include "appHelper.h"
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"
#include "../utils/notificationcontroller.h"
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QUrl>

AppHelper::AppHelper(QObject *parent)
{

}

void AppHelper::installApp(const QString &path)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    QString filePath;
    QUrl url(path);
    if (url.isLocalFile()) {
        filePath = url.toLocalFile();
    } else {
        filePath = path;
    }

    if (!QFile::exists(filePath)) {
        NotificationController::instance()->send("安装失败", "安装文件不合法", NotificationController::Error);
    }
    NotificationController::instance()->send("正在安装", "开始安装，请耐心等待");
    ADBInterface::instance()->installApp(deviceCode, filePath);
    NotificationController::instance()->send("安装成功", "安装执行完成");
    emit requestUpdateSoftList();
}

void AppHelper::clearData(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->clearData(deviceCode, packageName);
    NotificationController::instance()->send("成功","数据清除完成");
}

void AppHelper::uninstallApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->uninstallApp(deviceCode, packageName);
    NotificationController::instance()->send("成功", "卸载完成");
    emit requestUpdateSoftList();
}

void AppHelper::freezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->freezeApp(deviceCode, packageName);
    NotificationController::instance()->send("成功", "冻结成功");
    emit requestUpdateSoftList();
}

void AppHelper::unfreezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->unfreezeApp(deviceCode, packageName);
    NotificationController::instance()->send("成功","解冻完成");
    emit requestUpdateSoftList();
}

void AppHelper::extractApp(const QString &packagePath, const QString &targetPath, const QString &packageName)
{
    qWarning() << "sub thread" << QThread::currentThreadId();
    QString tar = targetPath;
    if (targetPath.isEmpty()) {
        tar = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QDir::separator() + packageName + ".apk";
    }
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    if (packagePath.isEmpty()) {
        qWarning() << "packagePath is empty, return , packageName: " << packageName;
        return;
    }
    ADBInterface::instance()->extractApp(deviceCode, packagePath, tar);
    qWarning() << tar;
    NotificationController::instance()->send("提取完成","默认保存在文档目录");
}

void AppHelper::stopApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->killActivity(packageName, deviceCode);
    NotificationController::instance()->send("指令已发送", "执行完成");
}

void AppHelper::startApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->startApp(deviceCode, packageName);
    NotificationController::instance()->send("指令已发送", "执行启动");
}

void AppHelper::startActivity(const QString &activity, const QStringList &args)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->startActivity(deviceCode, activity, args);
    NotificationController::instance()->send("指令已发送", "启动活动");
}

void AppHelper::updateDetailInfo(QString packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    this->m_info = ADBInterface::instance()->getAppDetailInfo(deviceCode, packageName);
    qWarning() << this->m_info.versionName;
    emit updateFinish();
}
