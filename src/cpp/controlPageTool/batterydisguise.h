#ifndef BATTERYDISGUISE_H
#define BATTERYDISGUISE_H

#include <QObject>
#include "../utils/singleton.hpp"

class BatteryDisguise : public QObject
{
    Q_OBJECT
    SINGLETON(BatteryDisguise)
public:
    Q_INVOKABLE void setBatteryLevel(quint8 level);
    Q_INVOKABLE void stopCharge();
    Q_INVOKABLE void restoreCharge();
    Q_INVOKABLE void connectButNoCharge();
    Q_INVOKABLE void restoreAll();
};

#endif // BATTERYDISGUISE_H
