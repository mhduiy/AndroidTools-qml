#include "appDetailControl.h"
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"
#include "../utils/utils.hpp"
#include <QVariant>
#include <QStandardPaths>
#include <QDir>
#include <QMetaObject>

AppDetailControl::AppDetailControl(QObject *parent) 
: QObject(parent)
, m_appHelperThread(new QThread(this))
, m_appHelper(new AppHelper(nullptr))
{
    m_appHelper->moveToThread(m_appHelperThread);
    m_appHelperThread->start();
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
    qWarning() << "main Thread" << QThread::currentThreadId();
    QMetaObject::invokeMethod(m_appHelper, "installApp", Qt::QueuedConnection, path);
}

void AppDetailControl::clearData(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "clearData", Qt::QueuedConnection, packageName);
}

void AppDetailControl::uninstallApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "uninstallApp", Qt::QueuedConnection, packageName);
}

void AppDetailControl::freezeApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "freezeApp", Qt::QueuedConnection, packageName);
}

void AppDetailControl::unfreezeApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "unfreezeApp", Qt::QueuedConnection, packageName);
}

void AppDetailControl::extractApp(const QString &packageName, const QString &targetPath)
{
    qWarning() << "main Thread" << QThread::currentThreadId();
    QMetaObject::invokeMethod(m_appHelper, "extractApp", Qt::QueuedConnection, m_info.path, targetPath, packageName);
}

void AppDetailControl::stopApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "stopApp", Qt::QueuedConnection, packageName);
}
