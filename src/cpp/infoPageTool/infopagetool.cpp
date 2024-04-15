#include "infopagetool.h"
#include <QtQml>
#include "batteryControl.h"
#include "cutactivitycontrol.h"
#include "adbcontrol.h"
#include "detailinfocontrol.h"

InfoPageTool::InfoPageTool(QObject *parent) : QObject(parent)
{
    qmlRegisterSingletonInstance("BatteryControl", 1, 0, "BatteryControl", BatteryControl::instance(this));
    qmlRegisterSingletonInstance("CutActivityControl", 1, 0, "CutActivityControl", CutActivityControl::instance(this));
    qmlRegisterSingletonInstance("ADBControl", 1, 0, "ADBControl", ADBControl::instance(this));
    qmlRegisterSingletonInstance("DetailInfoControl", 1, 0, "DetailInfoControl", DetailInfoControl::instance(this));
}
