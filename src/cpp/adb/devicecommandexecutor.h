#pragma once

#include <QObject>
#include <QFuture>
#include <QPromise>
#include <QtConcurrent>
#include <functional>
#include "adbdevice.h"
#include "../utils/notificationcontroller.h"

// ADB命令执行器
class ADBCommandExecutor : public QObject
{
    Q_OBJECT

public:
    using SuccessCallback = std::function<void(const ADBCommandResult&)>;
    using FailureCallback = std::function<void(const ADBCommandResult&)>;

    explicit ADBCommandExecutor(QObject *parent = nullptr);

    // 配置命令参数和执行选项
    ADBCommandExecutor& withArgs(const QStringList& args);
    ADBCommandExecutor& withWriteData(const QString& writeStr);
    ADBCommandExecutor& withTimeout(int timeout);
    
    // 配置消息
    ADBCommandExecutor& withSuccessMessage(const QString& message);
    ADBCommandExecutor& withFailureMessage(const QString& message);
    ADBCommandExecutor& withProgressMessage(const QString& message);
    
    // 配置回调
    ADBCommandExecutor& onSuccess(SuccessCallback callback);
    ADBCommandExecutor& onFailure(FailureCallback callback);
    
    // 执行命令
    QFuture<ADBCommandResult> execute();
    
    // 便捷方法：同步执行
    ADBCommandResult executeSync();
    
    // 静态便捷方法
    static QFuture<ADBCommandResult> executeCommand(
        const QStringList& args,
        const QString& successMsg = "",
        const QString& failureMsg = "",
        SuccessCallback onSuccess = nullptr,
        FailureCallback onFailure = nullptr
    );

private:
    // 判断命令是否成功
    bool isCommandSuccessful(const ADBCommandResult& result) const;
    
    // 发送通知
    void sendNotification(const QString& title, const QString& message, 
                         NotificationController::NotificationType type) const;

private:
    QStringList m_args;
    QString m_writeData;
    int m_timeout = 3000;
    
    QString m_successMessage;
    QString m_failureMessage;
    QString m_progressMessage;
    
    SuccessCallback m_successCallback;
    FailureCallback m_failureCallback;
};

// 便捷的创建函数
inline ADBCommandExecutor* createADBCommand() {
    return new ADBCommandExecutor();
}

// 宏定义用于快速创建命令
#define ADB_COMMAND() (new ADBCommandExecutor())

// 预定义的常用命令构建器
namespace ADBCommands {
    // 安装APK
    ADBCommandExecutor* installApp(const QString& apkPath, bool replace = false);
    
    // 卸载应用
    ADBCommandExecutor* uninstallApp(const QString& packageName);
    
    // 启动应用
    ADBCommandExecutor* startApp(const QString& packageName);
    
    // 停止应用
    ADBCommandExecutor* stopApp(const QString& packageName);
    
    // 推送文件
    ADBCommandExecutor* pushFile(const QString& localPath, const QString& remotePath);
    
    // 拉取文件
    ADBCommandExecutor* pullFile(const QString& remotePath, const QString& localPath);
    
    // 执行shell命令
    ADBCommandExecutor* shell(const QString& command);
    
    // 重启设备
    ADBCommandExecutor* reboot(const QString& mode = "");
    
    // 按键事件
    ADBCommandExecutor* keyEvent(int keyCode);
    
    // 截图
    ADBCommandExecutor* screenshot(const QString& outputPath = "");
    
    // 清除应用数据
    ADBCommandExecutor* clearAppData(const QString& packageName);
    
    // 获取设备属性
    ADBCommandExecutor* getDeviceProp(const QString& property);
    
    // 启动Activity
    ADBCommandExecutor* startActivity(const QString& activity, const QStringList& extras = {});
}
