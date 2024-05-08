#ifndef APPHELPERTHREAD_H
#define APPHELPERTHREAD_H

#include <QObject>

class AppHelper : public QObject
{
    Q_OBJECT
public:
    explicit AppHelper(QObject *parent = nullptr);

public slots:
    void installApp(const QString &Path);
    void clearData(const QString &packageName);
    void uninstallApp(const QString &packageName);
    void freezeApp(const QString &packageName);
    void unfreezeApp(const QString &packageName);
    void extractApp(const QString &packagePath, const QString &targetPath, const QString &packageName);
    void stopApp(const QString &packageName);
};

#endif
