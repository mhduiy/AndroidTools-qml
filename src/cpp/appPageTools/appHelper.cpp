#include "appHelper.h"
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"
#include "../utils/Notification.h"
#include <QStandardPaths>
#include <QDir>
#include <QThread>


AppHelper::AppHelper(QObject *parent)
{

}

void AppHelper::installApp(const QString &path)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    NotificationControl::instance()->send("开始安装，请耐心等待");
    ADBInterface::instance()->installApp(deviceCode, path);
    NotificationControl::instance()->send("安装执行完成");
}

void AppHelper::clearData(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->clearData(deviceCode, packageName);
    NotificationControl::instance()->send("数据清除完成");
}

void AppHelper::uninstallApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->uninstallApp(deviceCode, packageName);
    NotificationControl::instance()->send("卸载完成");
}

void AppHelper::freezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->freezeApp(deviceCode, packageName);
    NotificationControl::instance()->send("冻结成功");
}

void AppHelper::unfreezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->unfreezeApp(deviceCode, packageName);
    NotificationControl::instance()->send("解冻完成");
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
    NotificationControl::instance()->send("提取完成，默认保存在文档目录");
}

void AppHelper::stopApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->killActivity(packageName, deviceCode);
    NotificationControl::instance()->send("执行完成");
}

void AppHelper::startApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->startApp(deviceCode, packageName);
    NotificationControl::instance()->send("执行启动");
}

void AppHelper::startActivity(const QString &activity, const QStringList &args)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    ADBInterface::instance()->startActivity(deviceCode, activity, args);
    NotificationControl::instance()->send("启动活动");
}
