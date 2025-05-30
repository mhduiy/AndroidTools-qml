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
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        const QString cutDevice = device->code();
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
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        const QString cutDevice = device->code();
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
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        const QString cutDevice = device->code();
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
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        const QString cutDevice = device->code();
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
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        const QString cutDevice = device->code();
        QStringList args;
        args << "-s" << cutDevice << "shell"
             << "dumpsys" << "battery" << "reset";
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
} 