#include "devicecommandexecutor.h"
#include "connectmanager.h"
#include <QCoreApplication>
#include <QMetaObject>
#include <QDebug>

ADBCommandExecutor::ADBCommandExecutor(QObject *parent)
    : QObject(parent)
{
}

ADBCommandExecutor& ADBCommandExecutor::withArgs(const QStringList& args)
{
    m_args = args;
    return *this;
}

ADBCommandExecutor& ADBCommandExecutor::withWriteData(const QString& writeStr)
{
    m_writeData = writeStr;
    return *this;
}

ADBCommandExecutor& ADBCommandExecutor::withTimeout(int timeout)
{
    m_timeout = timeout;
    return *this;
}

ADBCommandExecutor& ADBCommandExecutor::withSuccessMessage(const QString& message)
{
    m_successMessage = message;
    return *this;
}

ADBCommandExecutor& ADBCommandExecutor::withFailureMessage(const QString& message)
{
    m_failureMessage = message;
    return *this;
}

ADBCommandExecutor& ADBCommandExecutor::withProgressMessage(const QString& message)
{
    m_progressMessage = message;
    return *this;
}

ADBCommandExecutor& ADBCommandExecutor::onSuccess(SuccessCallback callback)
{
    m_successCallback = callback;
    return *this;
}

ADBCommandExecutor& ADBCommandExecutor::onFailure(FailureCallback callback)
{
    m_failureCallback = callback;
    return *this;
}

QFuture<ADBCommandResult> ADBCommandExecutor::execute()
{
    return QtConcurrent::run([this]() -> ADBCommandResult {
        // 获取当前连接的设备
        auto device = ConnectManager::instance()->cutADBDevice();
        
        ADBCommandResult result;
        
        if (!device) {
            result.success = false;
            result.errorOutput = "当前无设备连接";
            result.exitCode = -1;
            result.command = m_args.join(" ");
            
            // 发送失败通知
            QMetaObject::invokeMethod(QCoreApplication::instance(), [this, result]() {
                sendNotification("执行失败", "当前无设备连接", NotificationController::Error);
                if (m_failureCallback) {
                    m_failureCallback(result);
                }
            }, Qt::QueuedConnection);
            
            return result;
        }
        
        // 显示进度消息
        if (!m_progressMessage.isEmpty()) {
            QMetaObject::invokeMethod(QCoreApplication::instance(), [this]() {
                sendNotification("执行中", m_progressMessage, NotificationController::Info);
            }, Qt::QueuedConnection);
        }
        
        // 使用优化后的executeCommandDetailed方法
        result = device->executeCommandDetailed(m_args, m_writeData, m_timeout);
        
        // 判断命令是否成功
        bool commandSuccess = isCommandSuccessful(result);
        
        // 更新成功状态（如果需要额外的成功判断逻辑）
        if (!result.success && commandSuccess) {
            result.success = commandSuccess;
        }
        
        // 处理结果
        QMetaObject::invokeMethod(QCoreApplication::instance(), [this, result]() {
            if (result.isSuccess()) {
                if (!m_successMessage.isEmpty()) {
                    sendNotification("执行成功", m_successMessage, NotificationController::Info);
                }
                if (m_successCallback) {
                    m_successCallback(result);
                }
            } else {
                QString errorMsg = m_failureMessage.isEmpty() ? 
                    QString("命令执行失败: %1").arg(result.getAllOutput()) : 
                    m_failureMessage;
                sendNotification("执行失败", errorMsg, NotificationController::Error);
                if (m_failureCallback) {
                    m_failureCallback(result);
                }
            }
        }, Qt::QueuedConnection);
        
        return result;
    });
}

ADBCommandResult ADBCommandExecutor::executeSync()
{
    auto future = execute();
    future.waitForFinished();
    return future.result();
}

QFuture<ADBCommandResult> ADBCommandExecutor::executeCommand(
    const QStringList& args,
    const QString& successMsg,
    const QString& failureMsg,
    SuccessCallback onSuccess,
    FailureCallback onFailure)
{
    auto executor = new ADBCommandExecutor();
    executor->withArgs(args);
    
    if (!successMsg.isEmpty()) {
        executor->withSuccessMessage(successMsg);
    }
    if (!failureMsg.isEmpty()) {
        executor->withFailureMessage(failureMsg);
    }
    if (onSuccess) {
        executor->onSuccess(onSuccess);
    }
    if (onFailure) {
        executor->onFailure(onFailure);
    }
    
    auto future = executor->execute();
    
    // 自动删除executor对象
    executor->deleteLater();
    
    return future;
}

bool ADBCommandExecutor::isCommandSuccessful(const ADBCommandResult& result) const
{
    // 如果已经成功且退出码为0，直接返回true
    if (result.isSuccess()) {
        return true;
    }
    
    // 对于shell命令，通常只要没有明显错误就认为成功
    if (m_args.contains("shell")) {
        return result.exitCode == 0;
    }
    
    // 默认根据退出码判断
    return result.exitCode == 0;
}

void ADBCommandExecutor::sendNotification(const QString& title, const QString& message, 
                                        NotificationController::NotificationType type) const
{
    NotificationController::instance()->send(title, message, type);
}

// 预定义命令实现
namespace ADBCommands {

ADBCommandExecutor* installApp(const QString& apkPath, bool replace)
{
    QStringList args = {"install"};
    if (replace) {
        args << "-r";
    }
    args << apkPath;
    
    auto executor = ADB_COMMAND();
    return &executor->withArgs(args)
        .withSuccessMessage("应用安装成功")
        .withFailureMessage("应用安装失败")
        .withProgressMessage("正在安装应用...")
        .withTimeout(60000); // 安装可能需要更长时间
}

ADBCommandExecutor* uninstallApp(const QString& packageName)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"uninstall", packageName})
        .withSuccessMessage("应用卸载成功")
        .withFailureMessage("应用卸载失败")
        .withProgressMessage("正在卸载应用...");
}

ADBCommandExecutor* startApp(const QString& packageName)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"shell", "monkey", "-p", packageName, "-c", "android.intent.category.LAUNCHER", "1"})
        .withSuccessMessage("应用启动成功")
        .withFailureMessage("应用启动失败")
        .withProgressMessage("正在启动应用...");
}

ADBCommandExecutor* stopApp(const QString& packageName)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"shell", "am", "force-stop", packageName})
        .withSuccessMessage("应用已停止")
        .withFailureMessage("停止应用失败")
        .withProgressMessage("正在停止应用...");
}

ADBCommandExecutor* pushFile(const QString& localPath, const QString& remotePath)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"push", localPath, remotePath})
        .withSuccessMessage("文件推送成功")
        .withFailureMessage("文件推送失败")
        .withProgressMessage("正在推送文件...")
        .withTimeout(30000); // 文件传输可能需要更长时间
}

ADBCommandExecutor* pullFile(const QString& remotePath, const QString& localPath)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"pull", remotePath, localPath})
        .withSuccessMessage("文件拉取成功")
        .withFailureMessage("文件拉取失败")
        .withProgressMessage("正在拉取文件...")
        .withTimeout(30000); // 文件传输可能需要更长时间
}

ADBCommandExecutor* shell(const QString& command)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"shell", command})
        .withProgressMessage("正在执行Shell命令...");
}

ADBCommandExecutor* reboot(const QString& mode)
{
    QStringList args = {"reboot"};
    if (!mode.isEmpty()) {
        args << mode;
    }
    
    auto executor = ADB_COMMAND();
    return &executor->withArgs(args)
        .withSuccessMessage("设备重启中...")
        .withFailureMessage("设备重启失败")
        .withTimeout(10000);
}

ADBCommandExecutor* keyEvent(int keyCode)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"shell", "input", "keyevent", QString::number(keyCode)})
        .withProgressMessage("正在发送按键事件...");
}

ADBCommandExecutor* screenshot(const QString& outputPath)
{
    QStringList args = {"exec-out", "screencap", "-p"};
    
    auto executor = ADB_COMMAND();
    executor->withArgs(args)
        .withSuccessMessage("截图成功")
        .withFailureMessage("截图失败")
        .withProgressMessage("正在截图...");
    
    if (!outputPath.isEmpty()) {
        // 如果指定了输出路径，需要额外处理保存文件的逻辑
        executor->onSuccess([outputPath](const ADBCommandResult& result) {
            // 这里可以添加保存截图文件的逻辑
            // 由于截图是二进制数据，可能需要特殊处理
            qDebug() << "截图数据大小:" << result.output.size() << "字节";
        });
    }
    
    return executor;
}

ADBCommandExecutor* clearAppData(const QString& packageName)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"shell", "pm", "clear", packageName})
        .withSuccessMessage("应用数据清除成功")
        .withFailureMessage("清除应用数据失败")
        .withProgressMessage("正在清除应用数据...");
}

ADBCommandExecutor* getDeviceProp(const QString& property)
{
    auto executor = ADB_COMMAND();
    return &executor->withArgs({"shell", "getprop", property})
        .withProgressMessage(QString("正在获取设备属性: %1").arg(property));
}

ADBCommandExecutor* startActivity(const QString& activity, const QStringList& extras)
{
    QStringList args = {"shell", "am", "start", "-n", activity};
    args.append(extras);
    
    auto executor = ADB_COMMAND();
    return &executor->withArgs(args)
        .withSuccessMessage("Activity启动成功")
        .withFailureMessage("Activity启动失败")
        .withProgressMessage("正在启动Activity...");
}

} // namespace ADBCommands 