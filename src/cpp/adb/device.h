#pragma once

#include <QObject>
#include <QSharedPointer>

#include "src/cpp/utils/defutils.hpp"

enum ConnectStatus {
    C_ADB, C_Fastboot, C_DisConnected, C_Unknown
};

// 设备唯一code

class Device : public QObject
{
    Q_OBJECT
    // 设备唯一代码
    DECLARE_PROPERTY(QString, code)
    // 连接状态
    DECLARE_PROPERTY(ConnectStatus, connectStatus)

    // 设备基本信息
    // 设备名称
    DECLARE_PROPERTY(QString, deviceName)
    // 设备IP地址
    DECLARE_PROPERTY(QString, ip)
    // 是否连接
    DECLARE_PROPERTY(bool, isConnected)
    // 是否无线连接
    DECLARE_PROPERTY(bool, isWireless)
    // 是否充电
    DECLARE_PROPERTY(bool, isCharging)

public:
    explicit Device(const QString &code, QObject *parent = nullptr);
    virtual ~Device() = default;

    // 请求断开连接
    virtual void requestDisConnect() = 0;

    bool operator==(const Device &other) const
    {
        return code() == other.code();
    }

protected:
};
