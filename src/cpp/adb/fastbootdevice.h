#pragma once

#include "device.h"

class FastbootDevice : public Device
{
    Q_OBJECT
public:
    explicit FastbootDevice(const QString &code, QObject *parent = nullptr);

    // 断开设备连接
    void requestDisConnect() Q_DECL_OVERRIDE;
};