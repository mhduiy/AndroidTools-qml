#include "controlPageTool.h"
#include <QtQml>
#include "devicecontrol.h"

ControlPageTool::ControlPageTool(QObject *parent) : QObject(parent)
{
    qmlRegisterSingletonInstance("DeviceControl", 1, 0, "DeviceControl", DeviceControl::instance(this));
}

