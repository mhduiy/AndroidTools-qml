#include "flashPageTool.h"
#include "fastbootdevicemanager.h"
#include "flashtools.h"
#include <QtQml>

namespace ADT {

FlashPageTool::FlashPageTool(QObject *parent)
    : QObject(parent)
{
    qmlRegisterSingletonInstance("FastBootDeviceManager", 1, 0, "FastBootDeviceManager", FastBootDeviceManager::instance(this));
    qmlRegisterSingletonInstance("FlashTools", 1, 0, "FlashTools", FlashTools::instance());
}

} // namespace ADT
