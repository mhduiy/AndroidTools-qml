#ifndef DEVICECONTROL_H
#define DEVICECONTROL_H

#include <QObject>
#include <QHash>
#include "src/cpp/utils/defutils.hpp"
#include "src/cpp/adb/adbtools.h"
#include "src/cpp/adb/adbdevice.h"

namespace ADT {

class DeviceControl : public QObject
{
    Q_OBJECT
    SINGLETON(DeviceControl)
public:
    Q_INVOKABLE void control(ControlType controlType, int controlItem);

private:
    ADBTools *m_adbtools;
};

} // namespace ADT

#endif // DEVICECONTROL_H
