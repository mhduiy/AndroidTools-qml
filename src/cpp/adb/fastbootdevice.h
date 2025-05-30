#pragma once

#include "device.h"

class FastbootDevice : public Device
{
    Q_OBJECT
public:
    explicit FastbootDevice(QObject *parent = nullptr);

    // 断开设备连接
    void requestDisConnect() Q_DECL_OVERRIDE;

    void startBoot(const QString &bootImage);
    void flash(const QString &partName, const QString &imagePath);
    void clear(const QString &partName);


    QString executeCommand(const QString &command, const QStringList &args = QStringList(), const QString &workDir = QString());
private:

};