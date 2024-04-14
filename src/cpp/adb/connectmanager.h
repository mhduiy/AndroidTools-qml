#ifndef CONNECTMANAGER_H
#define CONNECTMANAGER_H

#include <QObject>
#include "../utils/singleton.hpp"

class ConnectManager : public QObject
{
    Q_OBJECT
    SINGLETON(ConnectManager)
public:
    QString currentDeviceCode();
    bool setCurrentDeviceCode();

private:
    void refreshDevice();
};

#endif // CONNECTMANAGER_H
