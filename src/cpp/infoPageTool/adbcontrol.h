#ifndef ADBCONTROL_H
#define ADBCONTROL_H

#include <QObject>
#include "../utils/singleton.hpp"

class ADBControl : public QObject
{
    Q_OBJECT
    SINGLETON(ADBControl)
    Q_PROPERTY(QString adbVersion READ getADBVersion NOTIFY valueChanged)

signals:
    void valueChanged();

public:
    QString getADBVersion() const {return m_adbVersion;};
    Q_INVOKABLE void restartADB();
    Q_INVOKABLE void pairDevice(const QString ipAndPort, const QString &pairCode);
    Q_INVOKABLE void connectDevice(const QString ipAndPort);

private:
    void updateADBVersion();

private:
    QString m_adbVersion;
};

#endif // ADBCONTROL_H
