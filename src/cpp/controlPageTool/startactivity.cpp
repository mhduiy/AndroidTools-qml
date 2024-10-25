#include "startactivity.h"
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"
#include "../utils/utils.hpp"

StartActivity::StartActivity(QObject *parent) : QObject(parent)
{

}

void StartActivity::start(const QString &activity, const QString &argsStr)
{
    if (activity.isEmpty()) {
        return;
    }

    const QString &cutDevice = ConnectManager::instance()->currentDeviceCode();

    auto operatorFunc = [activity, argsStr, cutDevice]() -> void{
        ADBInterface::instance()->startActivity(cutDevice, activity, argsStr.split(' '));
    };

    asyncOperator(operatorFunc);
}
