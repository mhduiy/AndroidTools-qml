#ifndef ADBTOOLS_H
#define ADBTOOLS_H

#include <QObject>
#include <QMutex>

class ADBTools : public QObject
{
    Q_OBJECT
public:
    enum APP{
        ADB,
        FASTBOOT
    };

    QString executeCommand(APP app = ADB, const QStringList &args = QStringList(), const QString &writeStr = "", const int timeout = 3000);

    static ADBTools *instance(QObject *parent = nullptr);
private:
    explicit ADBTools(QObject *parent = nullptr);
    ADBTools() = delete;
    QMutex *m_mutex;
};

#endif // ADBTOOLS_H
