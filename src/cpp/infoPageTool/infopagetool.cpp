#include "infopagetool.h"
#include <QtQml>
#include "batteryControl.h"
#include "cutactivitycontrol.h"

InfoPageTool::InfoPageTool(QObject *parent) : QObject(parent)
{
    qmlRegisterSingletonInstance("BatteryControl", 1, 0, "BatteryControl", BatteryControl::instance(this));
    qmlRegisterSingletonInstance("CutActivityControl", 1, 0, "CutActivityControl", CutActivityControl::instance(this));
}
