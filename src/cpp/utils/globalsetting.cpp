#include <QStandardPaths>
#include <QDir>
#include "globalsetting.h"
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <qapplication.h>
#include <qdir.h>
#include <qlogging.h>
#include <qvariant.h>

GlobalSetting* GlobalSetting::_instance = nullptr;

GlobalSetting::GlobalSetting(QObject *parent) : QObject(parent) {
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    // 创建目录
    cacheDir = cacheDir + QDir::separator() + QApplication::applicationName();
    QDir dir(cacheDir);
    if(!dir.exists(cacheDir)) {
        dir.mkpath(cacheDir);
    }

    qDebug() << "config file:" << cacheDir + "/config.ini";
    settings = new QSettings(cacheDir + "/config.ini", QSettings::IniFormat);
}

void GlobalSetting::writeConfig(const QString &title, const QString &key, const QVariant &value) {
    qWarning() << title << key << value;
    settings->setValue(QString("%1/%2").arg(title, key), value);
}

QVariant GlobalSetting::readConfig(const QString &title, const QString &key, const QVariant &fallback) {
    return settings->value(QString("%1/%2").arg(title, key), fallback);
}

bool GlobalSetting::checkConfig(const QString &title, const QString &key, const QVariant &defaultValue)
{
    settings->beginGroup(title);
    if (!settings->contains(key)) {
        settings->setValue(key, defaultValue);
        settings->endGroup();
        return false;
    }
    settings->endGroup();
    return true;
}

GlobalSetting *GlobalSetting::instance(QObject *parent) {
    static QMutex mutex;
    QMutexLocker mutexLocker(&mutex);
    if(_instance == nullptr) {
        _instance = new GlobalSetting(parent);
    }
    return _instance;
}
