#include "appDetailControl.h"
#include "src/cpp/adb/connectmanager.h"
#include "src/cpp/utils/notificationcontroller.h"
#include "src/cpp/utils/utils.hpp"
#include <QVariant>
#include <QStandardPaths>
#include <QDir>
#include <QMetaObject>
#include <QDebug>

namespace ADT {

AppDetailControl::AppDetailControl(QObject *parent) 
: QObject(parent)
{
    connect(this, &AppDetailControl::updateSoftDetailInfoFinish, this, &AppDetailControl::onUpdateSoftDetailInfoFinish, Qt::QueuedConnection);
}

AppDetailControl::~AppDetailControl()
{

}

void AppDetailControl::updateInfo(const QString &packageName)
{
    asyncOperator([packageName, this](){
        auto info = CONNECTMANAGER->cutADBDevice()->getAppDetailInfo(packageName);
        emit updateSoftDetailInfoFinish(info);
    });
}

void AppDetailControl::installApp(const QString &path, bool r, bool s, bool d, bool g)
{
    asyncOperator([path, r, s, d, g, this](){
        NotificationController::instance()->send("安装中", "请耐心等待", NotificationController::Info);
        if (!CONNECTMANAGER->cutADBDevice()->installApp(path, r, s, d, g)) {
            NotificationController::instance()->send("安装失败", "安装失败", NotificationController::Error);
            Q_EMIT requestUpdateSoftList();
        } else {
            NotificationController::instance()->send("安装成功", "安装成功", NotificationController::Info);
        }
    });
}

void AppDetailControl::clearData(const QString &packageName)
{
    asyncOperator([packageName](){
        if (!CONNECTMANAGER->cutADBDevice()->clearData(packageName)) {
            NotificationController::instance()->send("清除数据失败", "清除数据失败", NotificationController::Error);
        }
    });
}

void AppDetailControl::uninstallApp(const QString &packageName)
{
    asyncOperator([packageName, this](){
        if (!CONNECTMANAGER->cutADBDevice()->uninstallApp(packageName)) {
            NotificationController::instance()->send("卸载失败", "卸载失败", NotificationController::Error);
        } else {
            NotificationController::instance()->send("卸载成功", "卸载成功", NotificationController::Info);
            Q_EMIT requestUpdateSoftList();
        }
    });
}

void AppDetailControl::freezeApp(const QString &packageName)
{
    asyncOperator([packageName, this](){
        if (!CONNECTMANAGER->cutADBDevice()->freezeApp(packageName)) {
            NotificationController::instance()->send("冻结失败", "冻结失败", NotificationController::Error);
        } else {
            NotificationController::instance()->send("冻结成功", "冻结成功", NotificationController::Info);
            Q_EMIT requestUpdateSoftList();
        }
    });
}

void AppDetailControl::unfreezeApp(const QString &packageName)
{
    asyncOperator([packageName, this](){
        if (!CONNECTMANAGER->cutADBDevice()->unfreezeApp(packageName)) {
            NotificationController::instance()->send("解冻失败", "解冻失败", NotificationController::Error);
        } else {
            NotificationController::instance()->send("解冻成功", "解冻成功", NotificationController::Info);
            Q_EMIT requestUpdateSoftList();
        }
    });
}

void AppDetailControl::extractApp(const QString &packageName, const QString &targetPath)
{
    asyncOperator([packageName, targetPath](){
        NotificationController::instance()->send("提取中", "请耐心等待", NotificationController::Info);
        if (!CONNECTMANAGER->cutADBDevice()->extractApp(packageName, targetPath)) {
            NotificationController::instance()->send("提取失败", "提取失败", NotificationController::Error);
        } else {
            NotificationController::instance()->send("提取成功", "保存在" + targetPath, NotificationController::Info);
        }
    });
}

void AppDetailControl::stopApp(const QString &packageName)
{
    asyncOperator([packageName](){
        if (!CONNECTMANAGER->cutADBDevice()->stopApp(packageName)) {
            NotificationController::instance()->send("停止失败", "停止失败", NotificationController::Error);
        }
    });
}

void AppDetailControl::startApp(const QString &packageName)
{
    asyncOperator([packageName](){
        CONNECTMANAGER->cutADBDevice()->startApp(packageName);
        NotificationController::instance()->send("执行启动", "命令已发送给设备", NotificationController::Info);
    });
}

void AppDetailControl::startActivity(const QString &activity, const QStringList &args)
{
    asyncOperator([activity, args](){
        CONNECTMANAGER->cutADBDevice()->startActivity(activity, args);
        NotificationController::instance()->send("执行启动", "命令已发送给设备", NotificationController::Info);
    });
}

void AppDetailControl::onUpdateSoftDetailInfoFinish(const AppDetailInfo &info)
{
    m_info = info;
    emit valueChanged(m_info);
}

void AppDetailControl::setSoftListType(SoftListType type)
{
    m_softListType = type;
    emit softListTypeChanged(m_softListType);
}

} // namespace ADT