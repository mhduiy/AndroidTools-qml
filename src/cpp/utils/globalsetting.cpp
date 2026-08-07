#include <QStandardPaths>
#include <QDir>
#include "globalsetting.h"
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <QCoreApplication>

GlobalSetting* GlobalSetting::_instance = nullptr;

GlobalSetting::GlobalSetting(QObject *parent) : QObject(parent) {
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->setSingleShot(true);
    cacheDir += QDir::separator() + QCoreApplication::applicationName();
    QDir dir(cacheDir);
    if (!dir.exists(cacheDir)) {
        dir.mkpath(cacheDir);
    }

    settings = new QSettings(cacheDir + "/config.ini", QSettings::IniFormat, this);
    connect(m_timer, &QTimer::timeout, this, &GlobalSetting::syncConfig);
}

void GlobalSetting::writeConfig(const QString &title, const QString &key, const QVariant &value) {
    settings->setValue(QString("%1/%2").arg(title, key), value);
    m_timer->start();
}

QVariant GlobalSetting::readConfig(const QString &title, const QString &key, const QVariant &fallback) {
    checkConfig(title, key, fallback);
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

void GlobalSetting::syncConfig()
{
    settings->sync();
}

GlobalSetting *GlobalSetting::instance(QObject *parent) {
    static QMutex mutex;
    QMutexLocker mutexLocker(&mutex);
    if(_instance == nullptr) {
        _instance = new GlobalSetting(parent);
    }
    return _instance;
}
