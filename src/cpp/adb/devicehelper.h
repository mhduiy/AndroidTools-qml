#pragma once

#include <QObject>
#include "adbdevice.h"

#include "src/cpp/utils/defutils.hpp"

namespace ADT {

class DeviceHelper : public QObject
{
    SINGLETON(DeviceHelper)
    DECLARE_PROPERTY(QList<ADBDevice*>, adbDeviceList)
    
    Q_OBJECT

public:
    void init();

    Q_INVOKABLE void requestDisConnectDevice(const QString &deviceCode, ConnectStatus type = C_ADB);

private:
    void initConnectManager();
};

} // namespace ADT