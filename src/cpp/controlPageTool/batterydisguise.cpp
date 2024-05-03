#include "batterydisguise.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/utils.hpp"

BatteryDisguise::BatteryDisguise(QObject *parent) : QObject(parent)
{

}

void BatteryDisguise::setBatteryLevel(quint8 level)
{
    auto operatorFunc = [level](){
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
        QStringList args;
        args << "-s" << cutDevice << "shell"
             << "dumpsys" << "battery" << "set" << "level" << QString::number(level);
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}

void BatteryDisguise::stopCharge()
{
    auto operatorFunc = [](){
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
        QStringList args;
        args << "-s" << cutDevice << "shell"
             << "dumpsys" << "battery" << "set" << "status" << "2";
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}

void BatteryDisguise::restoreCharge()
{
    auto operatorFunc = [](){
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
        QStringList args;
        args << "-s" << cutDevice << "shell"
             << "dumpsys" << "battery" << "set" << "status" << "1";
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}

void BatteryDisguise::connectButNoCharge()
{
    auto operatorFunc = [](){
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
        QStringList args;
        args << "-s" << cutDevice << "shell"
             << "dumpsys" << "battery" << "unplug";
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}

void BatteryDisguise::restoreAll()
{
    auto operatorFunc = [](){
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
        QStringList args;
        args << "-s" << cutDevice << "shell"
             << "dumpsys" << "battery" << "reset";
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}
