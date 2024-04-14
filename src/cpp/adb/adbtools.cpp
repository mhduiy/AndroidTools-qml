#include "adbtools.h"
#include <QMutex>
#include <QProcess>

#define PREFIX ""

#ifdef Q_OS_WIN

#define ADBPATH "adb.exe"
#define FASTBOOTPATH "fastboot.exe"

#elif defined(Q_OS_LINUX)

#define ADBPATH "adb"
#define FASTBOOTPATH "fastboot"

#elif defined(Q_OS_LINUX)

#endif




ADBTools::ADBTools(QObject *parent) : QObject(parent)
{

}

QString ADBTools::executeCommand(APP app, const QStringList &args)
{
    QProcess pro;
    if (app == APP::ADB) {
        pro.start(ADBPATH, args);
    } else if (app == APP::FASTBOOT){
        pro.start(ADBPATH, args);
    }
    pro.waitForFinished(3000);    //命令执行超过3秒强制提出

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
