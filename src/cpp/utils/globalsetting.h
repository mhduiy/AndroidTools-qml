#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include "defutils.hpp"

#define AppSettings GlobalSetting::instance()

class GlobalSetting : public QObject{
    Q_OBJECT
    SINGLETON(GlobalSetting)
public:
    void writeConfig(const QString& title, const QString& key, const QVariant &value);
    QVariant readConfig(const QString& title, const QString& key, const QVariant &fallback = {});
    // 检查键值是否存在，不存在则设置为给定的默认值
    bool checkConfig(const QString &title, const QString &key, const QVariant &defaultValue);

private:
    void syncConfig();

private:
    QSettings *settings = nullptr;
    QTimer *m_timer = nullptr;
};

#endif // GLOBALSETTING_H
