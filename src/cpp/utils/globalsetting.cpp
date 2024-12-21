#include <QStandardPaths>
#include <QDir>
#include "globalsetting.h"
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <QTimer>
#include <QApplication>

GlobalSetting* GlobalSetting::_instance = nullptr;

GlobalSetting::GlobalSetting(QObject *parent) : QObject(parent) {
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    m_timer = new QTimer();
    m_timer->setInterval(1000);
    m_timer->setSingleShot(true);
    // 创建目录
    cacheDir = cacheDir + QDir::separator() + QApplication::applicationName();
    QDir dir(cacheDir);
    if(!dir.exists(cacheDir)) {
        dir.mkpath(cacheDir);
    }

    qDebug() << "config file:" << cacheDir + "/config.ini";
    settings = new QSettings(cacheDir + "/config.ini", QSettings::IniFormat);

    connect(m_timer, &QTimer::timeout, this, &GlobalSetting::syncConfig);
}

void GlobalSetting::writeConfig(const QString &title, const QString &key, const QVariant &value) {
    auto handleFunc = [title, key, value, this](){
        qWarning() << title << key << value;
        settings->setValue(QString("%1/%2").arg(title, key), value);
    };
    m_timer->start();
    m_peddingTasks.enqueue(handleFunc);
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
    while(!m_peddingTasks.isEmpty()) {
        auto handleFunc = m_peddingTasks.dequeue();
        handleFunc();
    }
}

GlobalSetting *GlobalSetting::instance(QObject *parent) {
    static QMutex mutex;
    QMutexLocker mutexLocker(&mutex);
    if(_instance == nullptr) {
        _instance = new GlobalSetting(parent);
    }
    return _instance;
}
