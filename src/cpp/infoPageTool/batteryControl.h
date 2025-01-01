#ifndef BATTERYCONTROL_H
#define BATTERYCONTROL_H

#include <QObject>
#include "../adb/adbinterface.h"
#include "../utils/singleton.hpp"

class BatteryControl : public QObject
{
    Q_OBJECT
    SINGLETON(BatteryControl)
    Q_PROPERTY(quint8 level READ getLevel WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(int chargeMode READ getChargeMode WRITE setChargeMode NOTIFY chargeModeChanged)
    Q_PROPERTY(quint8 health READ gethealth WRITE sethealth NOTIFY healthChanged)
    Q_PROPERTY(uint voltage READ getVoltage WRITE setVoltage NOTIFY voltageChanged)
    Q_PROPERTY(uint current READ getCurrent WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(float power READ getPower WRITE setPower NOTIFY powerChanged)
    Q_PROPERTY(float temperature READ getTemperature WRITE setTemperature NOTIFY temperatureChanged)

public:
    void updateBatteryInfo();

    quint8 getLevel() const {return m_level;}
    void setLevel(quint8 level) {m_level = level; emit levelChanged(level);};

    int getChargeMode() const {return m_chargeMode;}
    void setChargeMode(int chargeMode) {m_chargeMode = chargeMode; emit chargeModeChanged(chargeMode);};

    quint8 gethealth() const {return m_health;}
    void sethealth(quint8 health) {m_health = health; emit healthChanged(health);};

    uint getVoltage() const {return m_voltage;}
    void setVoltage(uint voltage) {m_voltage = voltage; emit voltageChanged(voltage);};

    uint getCurrent() const {return m_current;}
    void setCurrent(uint current) {m_current = current; emit currentChanged(current);};

    float getPower() const {return m_power;}
    void setPower(float power) {m_power = power; emit powerChanged(power);};

    float getTemperature() const {return m_temperature;}
    void setTemperature(float temperature) {m_temperature = temperature; emit temperatureChanged(temperature);};

signals:
    void levelChanged(quint8 level);
    void chargeModeChanged(int chargeMode);
    void healthChanged(quint8 health);
    void voltageChanged(uint voltage);
    void currentChanged(uint current);
    void powerChanged(float power);
    void temperatureChanged(float temperature);

private:
    quint8 m_level;
    int m_chargeMode;
    quint8 m_health;
    uint m_voltage;
    uint m_current;
    float m_power;
    float m_temperature;
};

#endif
