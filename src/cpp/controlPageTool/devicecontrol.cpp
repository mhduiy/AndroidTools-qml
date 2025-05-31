#include <QDebug>
#include "devicecontrol.h"

#include "src/cpp/adb/connectmanager.h"
#include "src/cpp/utils/notificationcontroller.h"
#include "src/cpp/utils/utils.hpp"

namespace ADT {

DeviceControl::DeviceControl(QObject *parent) : QObject(parent)
{
    m_adbtools = ADBTools::instance();
}

void DeviceControl::control(ControlType controlType, int controlItem)
{
    asyncOperator([controlType, controlItem, this](){
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) {
            NotificationController::instance()->send("执行失败", "当前无设备连接", NotificationController::Error);
            return;
        }

        device->control(controlType, controlItem);
    });
}

} // namespace ADT