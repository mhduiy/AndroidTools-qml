#include "batterydisguise.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/utils.hpp"

namespace ADT {

namespace {

void runBatteryCmd(const QStringList &tail)
{
    asyncOperator([tail](){
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        QStringList args;
        args << "-s" << device->code() << "shell"
             << "dumpsys" << "battery" << tail;
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    });
}

} // namespace

BatteryDisguise::BatteryDisguise(QObject *parent) : QObject(parent)
{

}

void BatteryDisguise::setBatteryLevel(quint8 level)
{
    runBatteryCmd({"set", "level", QString::number(level)});
}

void BatteryDisguise::stopCharge()
{
    runBatteryCmd({"set", "status", "2"});
}

void BatteryDisguise::restoreCharge()
{
    runBatteryCmd({"set", "status", "1"});
}

void BatteryDisguise::connectButNoCharge()
{
    runBatteryCmd({"unplug"});
}

void BatteryDisguise::restoreAll()
{
    runBatteryCmd({"reset"});
}

} // namespace ADT
