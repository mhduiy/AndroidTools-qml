#include "adbcontrol.h"
#include <QtConcurrent/QtConcurrentRun>
#include "../adb/adbinterface.h"
#include "../utils/notificationcontroller.h"
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
        NotificationController::instance()->send("配对失败", "ip和端口不能为空");
        return;
    }
    if (pairCode.isEmpty()) {
        NotificationController::instance()->send("配对失败", "请输入配对码");
        return;
    }
    auto retStr = ADBTools::instance()->executeCommand(ADBTools::ADB, {"pair", ipAndPort}, pairCode).simplified();
    if (retStr.contains("Success")) {
        NotificationController::instance()->send("配对成功", "请进行下一步");
    } else {
        NotificationController::instance()->send("配对失败", "配对失败，请检查信息是否填写正确", NotificationController::Error);
    }
}

void ADBControl::connectDevice(const QString ipAndPort)
{
    if (ipAndPort.isEmpty()) {
        NotificationController::instance()->send("配对失败", "ip和端口不能为空", NotificationController::Warning);
        return;
    }
    auto retStr = ADBTools::instance()->executeCommand(ADBTools::ADB, {"connect", ipAndPort}).simplified();
    NotificationController::instance()->send("返回信息", retStr);
    if (!retStr.contains("connected")) {
        NotificationController::instance()->send("连接失败", "请检查信息是否填写正确", NotificationController::Error);
    }
}

void ADBControl::updateADBVersion()
{
    m_adbVersion = ADBInterface::instance()->adbVersion();
    emit valueChanged();
}
