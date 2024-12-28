#ifndef APPHELPERTHREAD_H
#define APPHELPERTHREAD_H

#include "src/cpp/adb/adbinterface.h"
#include <QObject>

class AppHelper : public QObject
{
    Q_OBJECT
public:
    explicit AppHelper(QObject *parent = nullptr);

public slots:
    void installApp(const QString &Path, bool r = false, bool s = false, bool d = false, bool g = false);
    void clearData(const QString &packageName);
    void uninstallApp(const QString &packageName);
    void freezeApp(const QString &packageName);
    void unfreezeApp(const QString &packageName);
    void extractApp(const QString &packagePath, const QString &targetPath, const QString &packageName);
    void stopApp(const QString &packageName);

    void startApp(const QString &packageName);
    void startActivity(const QString &activity, const QStringList &args);

    void updateDetailInfo(QString packageName);
    inline AppDetailInfo getInfo() const { return m_info; }

signals:
    void updateFinish();
    void requestUpdateSoftList();

private:
    AppDetailInfo m_info;
};

#endif
