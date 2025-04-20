#include "batteryControl.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>
#include "../adb/connectmanager.h"

BatteryControl::BatteryControl(QObject *parent) : QObject(parent)
{
    m_level = 0;
    m_chargeMode = ChargingType::None;
    m_health = 100;
    m_voltage = 5000;
    m_current = 5000;
    m_power = m_voltage * m_current / 1000000.0;
    m_temperature = 10.0;

    connect(ConnectManager::instance(), &ConnectManager::deviceRefreshFinish, this, &BatteryControl::updateBatteryInfo);
}

void BatteryControl::updateBatteryInfo()
{
    const DeviceBatteryInfo &&info = ConnectManager::instance()->getDeviceBatteryInfo();
    setLevel(info.level);
    setChargeMode(info.chargingType);
    sethealth(info.health);
    setVoltage(info.voltage);
    setCurrent(info.current);
    setPower(m_voltage * m_current / 1000000.0);
    setTemperature(info.temperature);
}
