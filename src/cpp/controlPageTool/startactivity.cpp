#include "startactivity.h"
#include "../adb/connectmanager.h"
#include "../adb/adbtools.h"
#include "../utils/utils.hpp"

namespace ADT {

StartActivity::StartActivity(QObject *parent) : QObject(parent)
{

}

void StartActivity::start(const QString &activity, const QString &argsStr)
{
    if (activity.isEmpty()) {
        return;
    }

    auto operatorFunc = [activity, argsStr]() -> void{
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        const QString cutDevice = device->code();
        
        QStringList args;
        args << "-s" << cutDevice << "shell" << "am" << "start";
        if (!argsStr.isEmpty()) {
            args << argsStr.split(' ');
        }
        args << activity;
        
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}

} // namespace ADT