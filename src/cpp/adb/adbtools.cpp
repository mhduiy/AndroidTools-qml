#include "adbtools.h"
#include "adbdevice.h" // 包含 ADBCommandResult 定义
#include <QMutex>
#include <QProcess>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include "adblog.h"

#define PREFIX ""

#ifdef Q_OS_WIN

#define ADBPATH "adb.exe"
#define FASTBOOTPATH "fastboot.exe"

#elif defined(Q_OS_LINUX)

#define ADBPATH "adb"
#define FASTBOOTPATH "fastboot"

#else

#define ADBPATH "adb"
#define FASTBOOTPATH "fastboot"

#endif

namespace ADT {

ADBTools::ADBTools(QObject *parent)
: QObject(parent)
, m_mutex(new QMutex)
{

}

CommandResult ADBTools::executeCommandDetailed(APP app, const QStringList &args, const QString &writeStr, const int timeout)
{
    QElapsedTimer timer;
    timer.start();
    
    QProcess pro;
    QString programName;
    
    if (app == APP::ADB) {
        programName = ADBPATH;
        pro.start(ADBPATH, args);
    } else if (app == APP::FASTBOOT){
        programName = FASTBOOTPATH;
        pro.start(FASTBOOTPATH, args);
    }

    // 构建命令字符串用于调试
    QString commandStr = QString("%1 %2").arg(programName, args.join(" "));
    
    if (!pro.waitForStarted()) {
        qWarning() << "Failed to start program:" << commandStr;
        return ADT::CommandResult(false, -1, "", "Failed to start program", commandStr, timer.elapsed());
    }

    m_isRunning.storeRelease(m_isRunning.loadRelaxed() + 1);

    if (!writeStr.isEmpty()) {
        pro.write(writeStr.toLocal8Bit());
        pro.closeWriteChannel(); // 关闭写入通道，表示数据写入完毕
    }

    bool finished = pro.waitForFinished(timeout);
    m_isRunning.storeRelease(m_isRunning.loadRelaxed() - 1);
    
    int executionTime = timer.elapsed();
    int exitCode = pro.exitCode();
    QString output = pro.readAllStandardOutput();
    QString errorOutput = pro.readAllStandardError();
    
    // 判断命令是否成功
    bool success = finished && (exitCode == 0);
    
    // 如果命令超时，记录错误
    if (!finished) {
        pro.kill();
        pro.waitForFinished(1000);
        errorOutput = QString("Command timeout after %1ms").arg(timeout);
        success = false;
        exitCode = -1;
    }
    
    // 记录日志
    if (!output.isEmpty()) {
        ADBLogModel::instance()->commitLog(ADBLogType::Info, output);
    }
    if (!errorOutput.isEmpty()) {
        ADBLogModel::instance()->commitLog(ADBLogType::Error, errorOutput);
    }
    
    return ADT::CommandResult(success, exitCode, output, errorOutput, commandStr, executionTime);
}

QString ADBTools::executeCommand(APP app, const QStringList &args, const QString &writeStr, const int timeout)
{
    // 使用新的详细方法，但只返回输出内容以保持向后兼容
    CommandResult result = executeCommandDetailed(app, args, writeStr, timeout);
    return result.getAllOutput();
}

ADBTools *ADBTools::instance(QObject *parent)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    static ADBTools* instance = nullptr;
    if (instance == nullptr) {
        instance = new ADBTools(parent);
    }
    return instance;
}

bool ADBTools::isRunning()
{
    return m_isRunning.loadRelaxed();
}

bool ADBTools::startService()
{
    return executeCommandDetailed(APP::ADB, {"start-server"}).success;
}

bool ADBTools::killService()
{
    return executeCommandDetailed(APP::ADB, {"kill-server"}).success;
}

} // namespace ADT