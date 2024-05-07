#include "appDetailControl.h"
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"
#include "../utils/utils.hpp"
#include <QVariant>

AppDetailControl::AppDetailControl(QObject *parent) : QObject(parent)
{

}

void AppDetailControl::updateInfo(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        this->m_info = ADBInterface::instance()->getAppDetailInfo(deviceCode, packageName);
        qWarning() << this->m_info.path;
        emit this->valueChanged(this->m_info);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::installApp(const QString &path)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &path](){
        ADBInterface::instance()->installApp(deviceCode, path);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::clearData(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->clearData(deviceCode, packageName);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::uninstallApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->uninstallApp(deviceCode, packageName);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::freezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->freezeApp(deviceCode, packageName);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::unfreezeApp(const QString &packageName)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    auto operatorFunc = [this, &deviceCode, &packageName](){
        ADBInterface::instance()->unfreezeApp(deviceCode, packageName);
    };

    asyncOperator(operatorFunc);
}

void AppDetailControl::extractApp(const QString &packageName, const QString &targetPath)
{
    const QString &deviceCode = ConnectManager::instance()->currentDeviceCode();
    const QString &packagePath = m_info.path;
    if (packagePath.isEmpty()) {
        qWarning() << "packagePath is empty, return , packageName: " << packageName;
        return;
    }
    auto operatorFunc = [this, &deviceCode, &packagePath, &targetPath](){
        ADBInterface::instance()->extractApp(deviceCode, packagePath, targetPath);
    };

    asyncOperator(operatorFunc);
}
