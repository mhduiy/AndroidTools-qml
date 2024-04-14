#include "batteryControl.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>

BatteryControl::BatteryControl(QObject *parent)
{
    m_level = 0;
    m_chargeMode = ChargingType::None;
    m_health = 100;
    m_voltage = 5000;
    m_current = 5000;
    m_power = m_voltage * m_current / 1000000;
    m_temperature = 10.0;
}

void BatteryControl::updateBatteryInfo(const DeviceBatteryInfo &info)
{
    setLevel(info.level);
    setChargeMode(info.chargingType);
    sethealth(info.health);
    setVoltage(info.voltage);
    setCurrent(info.maxChargingCut);
    setPower(m_voltage * m_current / 1000000);
    setTemperature(info.temperature);
}
