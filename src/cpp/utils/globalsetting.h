#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QObject>
#include <QSettings>

class GlobalSetting : public QObject{
    Q_OBJECT
public:
    void writeConfig(const QString& title, const QString& key, const QVariant &value);
    QVariant readConfig(const QString& title, const QString& key, const QVariant &fallback = {});
    // 检查键值是否存在，不存在则设置为给定的默认值
    bool checkConfig(const QString &title, const QString &key, const QVariant &defaultValue);
    static GlobalSetting* instance(QObject *parent = nullptr);
private:
    static GlobalSetting *_instance;
    explicit GlobalSetting(QObject *parent = nullptr);
    QSettings *settings = nullptr;
};

#endif // GLOBALSETTING_H
