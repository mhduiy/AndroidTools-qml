#include "appDetailControl.h"
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"
#include "../utils/utils.hpp"
#include "src/cpp/utils/Notification.h"
#include <QVariant>
#include <QStandardPaths>
#include <QDir>

AppDetailControl::AppDetailControl(QObject *parent) : QObject(parent)
{

}

void AppDetailControl::updateInfo(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        this->m_info = ADBInterface::instance()->getAppDetailInfo(deviceCode, packageName);
        qWarning() << this->m_info.versionName;
        emit this->valueChanged(this->m_info);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::installApp(const QString &path)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &path](){
        NotificationControl::instance()->send("开始安装，请耐心等待");
        ADBInterface::instance()->installApp(deviceCode, path);
        NotificationControl::instance()->send("安装执行完成");
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::clearData(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->clearData(deviceCode, packageName);
        NotificationControl::instance()->send("数据清除完成");
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::uninstallApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->uninstallApp(deviceCode, packageName);
        NotificationControl::instance()->send("卸载完成");
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::freezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->freezeApp(deviceCode, packageName);
        NotificationControl::instance()->send("冻结成功");
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::unfreezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->unfreezeApp(deviceCode, packageName);
        NotificationControl::instance()->send("解冻完成");
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::extractApp(const QString &packageName, const QString &targetPath)
{
    QString tar = targetPath;
    if (targetPath.isEmpty()) {
        tar = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QDir::separator() + packageName + ".apk";
    }
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    const QString &packagePath = m_info.path;
    if (packagePath.isEmpty()) {
        qWarning() << "packagePath is empty, return , packageName: " << packageName;
        return;
    }
    auto operatorFunc = [this, &deviceCode, &packagePath, &tar](){
        ADBInterface::instance()->extractApp(deviceCode, packagePath, tar);
        qWarning() << tar;
        NotificationControl::instance()->send("提取完成，保存在" + tar);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::stopApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->killActivity(packageName, deviceCode);
        NotificationControl::instance()->send("执行完成");
    };

    asyncOperator(operatorFunc);
}
