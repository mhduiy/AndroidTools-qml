#include "textinput.h"
#include "../utils/utils.hpp"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/keyconvert.h"

InputText::InputText(QObject *parent) : QObject(parent)
{

}

void InputText::pushKey(Qt::Key key)
{
    int cutKey = KeyConvert::instance()->convert(key);
    auto device = ConnectManager::instance()->cutADBDevice();
    if (!device || cutKey < 0) {
        return;
    }
    
    auto operatorFunc = [cutKey, device]() -> void {
        const QString cutDevice = device->code();
        QStringList args;
        args << "-s" << cutDevice << "shell" << "input" << "keyevent" << QString::number(cutKey);
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}
