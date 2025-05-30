#include "appHelper.h"
#include "../adb/connectmanager.h"
#include "../adb/adbtools.h"
#include "../adb/devicecommandexecutor.h"
#include "../utils/notificationcontroller.h"
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QUrl>
#include <QFile>

AppHelper::AppHelper(QObject *parent) : QObject(parent)
{

}

void AppHelper::installApp(const QString &path, bool r, bool s, bool d, bool g)
{
    QString filePath;
    QUrl url(path);
    if (url.isLocalFile()) {
        filePath = url.toLocalFile();
    } else {
        filePath = path;
    }

    if (!QFile::exists(filePath)) {
        NotificationController::instance()->send("安装失败", "安装文件不合法", NotificationController::Error);
        return;
    }

    // 使用新的命令执行框架
    createDeviceCommand<bool>("installApp", [filePath, r, s, d, g](QSharedPointer<ADBDevice> device) -> bool {
        return device->installApp(filePath, r, s, d, g);
    })
    .withProgressMessage("开始安装，请耐心等待")
    .withSuccessMessage("安装执行完成")
    .withErrorMessage("安装过程中出现错误")
    .onSuccess([this](bool success) {
        if (success) {
            emit requestUpdateSoftList();
        }
    })
    .execute();
}

void AppHelper::clearData(const QString &packageName)
{
    VoidDeviceCommand("clearData", [packageName](QSharedPointer<ADBDevice> device) {
        if (!device->clearData(packageName)) {
            throw std::runtime_error("数据清除失败");
        }
    })
    .withSuccessMessage("数据清除完成")
    .withErrorMessage("数据清除失败")
    .execute();
}

void AppHelper::uninstallApp(const QString &packageName)
{
    VoidDeviceCommand("uninstallApp", [packageName](QSharedPointer<ADBDevice> device) {
        if (!device->uninstallApp(packageName)) {
            throw std::runtime_error("卸载失败");
        }
    })
    .withSuccessMessage("卸载完成")
    .withErrorMessage("卸载失败")
    .onSuccess([this]() {
        emit requestUpdateSoftList();
    })
    .execute();
}

void AppHelper::freezeApp(const QString &packageName)
{
    VoidDeviceCommand("freezeApp", [packageName](QSharedPointer<ADBDevice> device) {
        if (!device->freezeApp(packageName)) {
            throw std::runtime_error("冻结失败");
        }
    })
    .withSuccessMessage("冻结成功")
    .withErrorMessage("冻结失败")
    .onSuccess([this]() {
        emit requestUpdateSoftList();
    })
    .execute();
}

void AppHelper::unfreezeApp(const QString &packageName)
{
    VoidDeviceCommand("unfreezeApp", [packageName](QSharedPointer<ADBDevice> device) {
        if (!device->unfreezeApp(packageName)) {
            throw std::runtime_error("解冻失败");
        }
    })
    .withSuccessMessage("解冻完成")
    .withErrorMessage("解冻失败")
    .onSuccess([this]() {
        emit requestUpdateSoftList();
    })
    .execute();
}

void AppHelper::extractApp(const QString &packagePath, const QString &targetPath, const QString &packageName)
{
    qWarning() << "sub thread" << QThread::currentThreadId();
    
    QString tar = targetPath;
    if (targetPath.isEmpty()) {
        tar = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QDir::separator() + packageName + ".apk";
    }
    
    if (packagePath.isEmpty()) {
        qWarning() << "packagePath is empty, return , packageName: " << packageName;
        return;
    }
    
    VoidDeviceCommand("extractApp", [packageName, tar](QSharedPointer<ADBDevice> device) {
        if (!device->extractApp(packageName, tar)) {
            throw std::runtime_error("提取过程中出现错误");
        }
    })
    .withSuccessMessage("提取完成，默认保存在文档目录")
    .withErrorMessage("提取失败")
    .execute();
}

void AppHelper::stopApp(const QString &packageName)
{
    VoidDeviceCommand("stopApp", [packageName](QSharedPointer<ADBDevice> device) {
        device->killActivity(packageName);
    })
    .withSuccessMessage("指令已发送，执行完成")
    .execute();
}

void AppHelper::startApp(const QString &packageName)
{
    VoidDeviceCommand("startApp", [packageName](QSharedPointer<ADBDevice> device) {
        device->startApp(packageName);
    })
    .withSuccessMessage("指令已发送，执行启动")
    .execute();
}

void AppHelper::startActivity(const QString &activity, const QStringList &args)
{
    VoidDeviceCommand("startActivity", [activity, args](QSharedPointer<ADBDevice> device) {
        device->startActivity(activity, args);
    })
    .withSuccessMessage("指令已发送，启动活动")
    .execute();
}

void AppHelper::updateDetailInfo(QString packageName)
{
    createDeviceCommand<AppDetailInfo>("updateDetailInfo", [packageName](QSharedPointer<ADBDevice> device) -> AppDetailInfo {
        return device->getAppDetailInfo(packageName);
    })
    .onSuccess([this](const AppDetailInfo& info) {
        this->m_info = info;
        qWarning() << this->m_info.versionName;
        emit updateFinish();
    })
    .execute();
}
