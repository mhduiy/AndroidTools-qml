#ifndef DEVICECONTROL_H
#define DEVICECONTROL_H

#include <QObject>
#include <QHash>
#include "../utils/defutils.hpp"
#include "../adb/adbtools.h"
#include "src/cpp/adb/adbdevice.h"

class DeviceControl : public QObject
{
    Q_OBJECT
    SINGLETON(DeviceControl)

public:

    const static QHash<int, QString> MusicControlArgsMap;
    const static QHash<int, QString> KeyControlArgsMap;
    const static QHash<int, QString> BoardControlArgsMap;
public:
    Q_INVOKABLE void control(ControlType controlType, int controlItem);

private:
    ADBTools *m_adbtools;
};

#endif // DEVICECONTROL_H
