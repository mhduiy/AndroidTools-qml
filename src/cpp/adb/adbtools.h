#ifndef ADBTOOLS_H
#define ADBTOOLS_H

#include <QObject>

class ADBTools : public QObject
{
    Q_OBJECT
public:
    enum APP{
        ADB,
        FASTBOOT
    };

    QString executeCommand(APP app = ADB, const QStringList &args = QStringList());

    static ADBTools *instance(QObject *parent = nullptr);
private:
    explicit ADBTools(QObject *parent = nullptr);
    ADBTools() = delete;
};

#endif // ADBTOOLS_H
