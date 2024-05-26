#ifndef APPDETAILCONTROL_H
#define APPDETAILCONTROL_H

#include <QObject>
#include <QVariant>
#include <QThread>
#include "appHelper.h"
#include "../adb/adbinterface.h"
#include "../utils/singleton.hpp"

class AppDetailControl : public QObject
{
    Q_OBJECT
    SINGLETON(AppDetailControl)
    Q_PROPERTY(QVariant packageName READ getPackageName NOTIFY valueChanged)
    Q_PROPERTY(QVariant versionCode READ getVersionCode NOTIFY valueChanged)
    Q_PROPERTY(QVariant installDate READ getInstallDate NOTIFY valueChanged)
    Q_PROPERTY(QVariant installUser READ getInstallUser NOTIFY valueChanged)
    Q_PROPERTY(QVariant targetSdk READ getTargetSdk NOTIFY valueChanged)
    Q_PROPERTY(QVariant minSdk READ getMinSdk NOTIFY valueChanged)
    Q_PROPERTY(QVariant appId READ getAppId NOTIFY valueChanged)
public:

    QVariant getPackageName() { return m_info.packageName; };
    QVariant getVersionCode() { return m_info.versionName; } ;
    QVariant getInstallDate()  { return m_info.installDate; };
    QVariant getInstallUser() { return m_info.installUser; } ;
    QVariant getTargetSdk() { return m_info.targetsdk; } ;
    QVariant getMinSdk() { return m_info.minsdk; } ;
    QVariant getAppId() {return m_info.appid;} ;

    Q_INVOKABLE void updateInfo(const QString &packageName);
    Q_INVOKABLE void installApp(const QString &Path);
    Q_INVOKABLE void clearData(const QString &packageName);
    Q_INVOKABLE void uninstallApp(const QString &packageName);
    Q_INVOKABLE void freezeApp(const QString &packageName);
    Q_INVOKABLE void unfreezeApp(const QString &packageName);
    Q_INVOKABLE void extractApp(const QString &packageName, const QString &targetPath);
    Q_INVOKABLE void stopApp(const QString &packageName);

    Q_INVOKABLE void startApp(const QString &packageName);
    Q_INVOKABLE void startActivity(const QString &activity, const QStringList &args);
signals:
    void valueChanged(const AppDetailInfo &info);

private:
    AppDetailInfo m_info;
    QThread *m_appHelperThread;
    AppHelper *m_appHelper;
};

#endif
