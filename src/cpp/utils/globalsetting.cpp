#include <QStandardPaths>
#include <QDir>
#include "globalSetting.h"
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

GlobalSetting* GlobalSetting::_instance = nullptr;

GlobalSetting::GlobalSetting(QObject *parent) : QObject(parent) {
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    // 创建目录
    QDir dir(cacheDir);
    if(!dir.exists(cacheDir)) {
        dir.mkpath(cacheDir);
    }
    settings = new QSettings(cacheDir + "/config.ini", QSettings::IniFormat);
}

void GlobalSetting::writeConfig(const QString &title, const QString &key, const QString &value) {
    settings->setValue(QString("%1/%2").arg(title, key), value);
}

QString GlobalSetting::readConfig(const QString &title, const QString &key) {
    return settings->value(QString("%1/%2").arg(title, key), "").toString();
}

GlobalSetting *GlobalSetting::instance(QObject *parent) {
    static QMutex mutex;
    QMutexLocker mutexLocker(&mutex);
    if(_instance == nullptr) {
        _instance = new GlobalSetting(parent);
    }
    return _instance;
}
