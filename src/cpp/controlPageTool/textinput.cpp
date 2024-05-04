#include "textinput.h"
#include "../utils/utils.hpp"
#include "../adb/adbinterface.h"
#include "../adb/connectmanager.h"
#include "../utils/keyconvert.h"

InputText::InputText(QObject *parent) : QObject(parent)
{

}

void InputText::pushKey(Qt::Key key)
{
    int cutKey = KeyConvert::instance()->convert(key);
    auto cutDevice = ConnectManager::instance()->currentDeviceCode();
    if (cutDevice.isEmpty() || cutKey < 0) {
        return;
    }
    auto operatorFunc = [cutKey, &cutDevice]() -> void {
        ADBInterface::instance()->pushKey(QString::number(cutKey), cutDevice);
    };

    asyncOperator(operatorFunc);
}
