#include "controlPageTool.h"
#include <QtQml>
#include "devicecontrol.h"
#include "batterydisguise.h"
#include "resolutioncontrol.h"
#include "filetransfer.h"
#include "textinput.h"

ControlPageTool::ControlPageTool(QObject *parent) : QObject(parent)
{
    qmlRegisterSingletonInstance("DeviceControl", 1, 0, "DeviceControl", DeviceControl::instance(this));
    qmlRegisterSingletonInstance("BatteryDisguise", 1, 0, "BatteryDisguise", BatteryDisguise::instance(this));
    qmlRegisterSingletonInstance("ResolutionControl", 1, 0, "ResolutionControl", ResolutionControl::instance(this));
    qmlRegisterSingletonInstance("FileTransfer", 1, 0, "FileTransfer", FileTransfer::instance(this));
    qmlRegisterSingletonInstance("InputText", 1, 0, "InputText", InputText::instance(this));
}

