#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QObject>
#include <QSettings>

class GlobalSetting : public QObject{
    Q_OBJECT
public:
    void writeConfig(const QString& title, const QString& key, const QString &value);
    QString readConfig(const QString& title, const QString& key);
    static GlobalSetting* instance(QObject *parent = nullptr);
private:
    static GlobalSetting *_instance;
    explicit GlobalSetting(QObject *parent = nullptr);
    QSettings *settings = nullptr;
};

#endif // GLOBALSETTING_H
