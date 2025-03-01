#include "appDetailControl.h"
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"
#include "../utils/utils.hpp"
#include "src/cpp/appPageTool/appHelper.h"
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

    connect(m_appHelper, &AppHelper::updateFinish, this, [this](){
        auto info = this->m_appHelper->getInfo();
        m_info = info;
        emit valueChanged(m_info);
    });

    connect(m_appHelper, &AppHelper::requestUpdateSoftList, this, &AppDetailControl::requestUpdateSoftList);
}

AppDetailControl::~AppDetailControl()
{
    qInfo() << "AppDetail Thread exiting";
    m_appHelperThread->quit();
    m_appHelperThread->wait();
    qInfo() << "AppDetail Thread exited";
}

void AppDetailControl::updateInfo(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "updateDetailInfo", Qt::QueuedConnection, Q_ARG(QString, packageName));
}

void AppDetailControl::installApp(const QString &path, bool r, bool s, bool d, bool g)
{
    qWarning() << "main Thread" << QThread::currentThreadId();
    QMetaObject::invokeMethod(m_appHelper, "installApp", Qt::QueuedConnection, Q_ARG(QString, path), Q_ARG(bool, r), Q_ARG(bool, s), Q_ARG(bool, d), Q_ARG(bool, g));
}

void AppDetailControl::clearData(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "clearData", Qt::QueuedConnection,  Q_ARG(QString, packageName));
}

void AppDetailControl::uninstallApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "uninstallApp", Qt::QueuedConnection,  Q_ARG(QString, packageName));
}

void AppDetailControl::freezeApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "freezeApp", Qt::QueuedConnection,  Q_ARG(QString, packageName));
}

void AppDetailControl::unfreezeApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "unfreezeApp", Qt::QueuedConnection,  Q_ARG(QString, packageName));
}

void AppDetailControl::extractApp(const QString &packageName, const QString &targetPath)
{
    QMetaObject::invokeMethod(m_appHelper, "extractApp", Qt::QueuedConnection, Q_ARG(QString, m_info.path), Q_ARG(QString, targetPath), Q_ARG(QString, packageName));
}

void AppDetailControl::stopApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "stopApp", Qt::QueuedConnection,  Q_ARG(QString, packageName));
}

void AppDetailControl::startApp(const QString &packageName)
{
    QMetaObject::invokeMethod(m_appHelper, "startApp", Qt::QueuedConnection,  Q_ARG(QString, packageName));
}

void AppDetailControl::startActivity(const QString &activity, const QStringList &args)
{
    QMetaObject::invokeMethod(m_appHelper, "startActivity", Qt::QueuedConnection, Q_ARG(QString,  activity), Q_ARG(QStringList, args));
}
