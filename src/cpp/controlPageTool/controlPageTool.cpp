#include "controlPageTool.h"
#include <QtQml>
#include "devicecontrol.h"
#include "batterydisguise.h"
#include "resolutioncontrol.h"

ControlPageTool::ControlPageTool(QObject *parent) : QObject(parent)
{
    qmlRegisterSingletonInstance("DeviceControl", 1, 0, "DeviceControl", DeviceControl::instance(this));
    qmlRegisterSingletonInstance("BatteryDisguise", 1, 0, "BatteryDisguise", BatteryDisguise::instance(this));
    qmlRegisterSingletonInstance("ResolutionControl", 1, 0, "ResolutionControl", ResolutionControl::instance(this));
}

