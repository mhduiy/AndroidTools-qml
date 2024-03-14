#ifndef ADBTOOLS_H
#define ADBTOOLS_H

#include <QObject>

class ADBTools : public QObject
{
    Q_OBJECT
public:
    explicit ADBTools(QObject *parent = nullptr);
    static ADBTools *instance(QObject *parent = nullptr);
};

#endif // ADBTOOLS_H
