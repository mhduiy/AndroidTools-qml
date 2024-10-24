#include "adbtools.h"
#include <QMutex>
#include <QProcess>
#include <QDebug>
#include <QMutexLocker>

#define PREFIX ""

#ifdef Q_OS_WIN

#define ADBPATH "adb.exe"
#define FASTBOOTPATH "fastboot.exe"

#elif defined(Q_OS_LINUX)

#define ADBPATH "adb"
#define FASTBOOTPATH "fastboot"

#elif defined(Q_OS_LINUX)

#endif




ADBTools::ADBTools(QObject *parent)
: QObject(parent)
, m_mutex(new QMutex)
{

}

QString ADBTools::executeCommand(APP app, const QStringList &args, const QString &writeStr, const int timeout)
{
//    QMutexLocker locker(m_mutex);
    QProcess pro;
    if (app == APP::ADB) {
        pro.start(ADBPATH, args);
    } else if (app == APP::FASTBOOT){
        pro.start(FASTBOOTPATH, args);
    }

    if (!pro.waitForStarted()) {
        qWarning() << "Failed to start program.";
        return {};
    }

    m_isRunning.storeRelease(m_isRunning.loadRelaxed() + 1);

    if (!writeStr.isEmpty()) {
        pro.write(writeStr.toLocal8Bit());
        pro.closeWriteChannel(); // 关闭写入通道，表示数据写入完毕
    }

    pro.waitForFinished(timeout);    //命令执行超过3秒强制提出
    m_isRunning.storeRelease(m_isRunning.loadRelaxed() - 1);

    QString ret = pro.readAllStandardOutput();
    if(ret.isEmpty()) {
        ret = pro.readAllStandardError();
    }
    return ret;
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