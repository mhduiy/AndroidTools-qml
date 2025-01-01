#include "realtimeinfohelper.h"
#include "../adb/connectmanager.h"

#include <QThread>
#include <QDebug>


RealTimeInfoHelper::RealTimeInfoHelper(QObject *parent)
: QObject(parent)
, m_woker(new RealTimeInfoWorker(this))
, m_workerThread(new QThread(this))
{
    m_woker->moveToThread(m_workerThread);
    m_workerThread->start();
    QMetaObject::invokeMethod(m_woker, "startRealTimeCheck", Qt::QueuedConnection);
}

RealTimeInfoHelper::~RealTimeInfoHelper()
{
    qInfo() << "RealTimeInfo Thread exiting";
    m_workerThread->quit();
    m_workerThread->wait();
    qInfo() << "RealTimeInfo Thread exited";
}

void RealTimeInfoHelper::setCpuTmp(double value)
{
    if (m_cpuTmp != value) {
        m_cpuTmp = value;
        Q_EMIT cpuTmpChanged(value);
    }
}

void RealTimeInfoHelper::setCpuUse(int value)
{
    if (m_cpuUse != value) {
        m_cpuUse = value;
        Q_EMIT cpuUseChanged(value);
    }
}

void RealTimeInfoHelper::setMemUse(int value)
{
    if (m_memUse != value) {
        m_memUse = value;
        Q_EMIT memUseChanged(value);
    }
}

void RealTimeInfoHelper::setDisUse(int value)
{
    if (m_disUse != value) {
        m_disUse = value;
        Q_EMIT disUseChanged(value);
    }
}

void RealTimeInfoHelper::setBtyTmp(double value)
{
    if (m_btyTmp != value) {
        m_btyTmp = value;
        Q_EMIT btyTmpChanged(value);
    }
}

RealTimeInfoWorker::RealTimeInfoWorker(RealTimeInfoHelper *helper, QObject *parent)
: QObject(parent)
, m_timer(new QTimer(this))
, m_helper(helper)
{
    m_timer->setInterval(2000);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &RealTimeInfoWorker::onTimeOut);
}

void RealTimeInfoWorker::startRealTimeCheck()
{
    m_timer->start();
}

void RealTimeInfoWorker::stopRealTimeCheck()
{
    m_timer->stop();
}

void RealTimeInfoWorker::onTimeOut()
{
    refreshMemUse();
}

void RealTimeInfoWorker::refreshMemUse()
{
    auto cutDevice = ConnectManager::instance()->currentDeviceCode();
    if (cutDevice.isEmpty()) {
        return;
    }
    QStringList args;
    args << "-s" << cutDevice << "shell" << "cat /proc/meminfo";
    QStringList ret = ADBTools::instance()->executeCommand(ADBTools::ADB, args).split('\n');
    int memTotal = 0;
    int memAvailable = 0;
    for (auto &lineInfo : ret) {
        lineInfo = lineInfo.simplified();
        if (lineInfo.startsWith("MemTotal")) {
            memTotal = lineInfo.split(' ').value(1).toInt();
        } else if (lineInfo.startsWith("MemAvailable")) {
            memAvailable = lineInfo.split(' ').value(1).toInt();
            break;
        }
    }
    int precent = memAvailable * 100 / memTotal;
    QMetaObject::invokeMethod(m_helper, "setMemUse", Qt::QueuedConnection, Q_ARG(int, 100 - precent));
}
