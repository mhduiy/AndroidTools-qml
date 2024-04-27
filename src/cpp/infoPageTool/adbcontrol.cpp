#include "adbcontrol.h"
#include <QtConcurrent/QtConcurrentRun>
#include "../adb/adbinterface.h"
#include "../utils/Notification.h"
#include <QDebug>

ADBControl::ADBControl(QObject *parent) : QObject(parent)
{
    m_adbVersion = ADBInterface::instance()->adbVersion();
    connect(ADBInterface::instance(), &ADBInterface::adbStarted, this, &ADBControl::updateADBVersion);
}

void ADBControl::restartADB()
{
    (void)QtConcurrent::run([](){
        ADBInterface::instance()->restartADBService();
    });
}

void ADBControl::pairDevice(const QString ipAndPort, const QString &pairCode)
{
    if (ipAndPort.isEmpty()) {
        NotificationControl::instance()->send("ip和端口不能为空");
        return;
    }
    if (pairCode.isEmpty()) {
        NotificationControl::instance()->send("请输入配对码");
        return;
    }
    auto retStr = ADBTools::instance()->executeCommand(ADBTools::ADB, {"pair", ipAndPort}, pairCode).simplified();
    if (retStr.contains("Success")) {
        NotificationControl::instance()->send("配对成功");
    } else {
        NotificationControl::instance()->send("配对失败，请检查信息是否填写正确", NotificationControl::Error);
    }
}

void ADBControl::connectDevice(const QString ipAndPort)
{
    if (ipAndPort.isEmpty()) {
        NotificationControl::instance()->send("ip和端口不能为空", NotificationControl::Warning);
        return;
    }
    auto retStr = ADBTools::instance()->executeCommand(ADBTools::ADB, {"connect", ipAndPort}).simplified();
    NotificationControl::instance()->send(retStr);
    if (!retStr.contains("connected")) {
        NotificationControl::instance()->send("连接失败，请检查信息是否填写正确", NotificationControl::Error);
    }
}

void ADBControl::updateADBVersion()
{
    m_adbVersion = ADBInterface::instance()->adbVersion();
    emit valueChanged();
}
