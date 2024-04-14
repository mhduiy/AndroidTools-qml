#include "infopagetool.h"
#include <QtQml>
#include "batteryControl.h"

InfoPageTool::InfoPageTool(QObject *parent) : QObject(parent)
{
    qmlRegisterSingletonInstance("BatteryControl", 1, 0, "BatteryControl", BatteryControl::instance(this));
}
