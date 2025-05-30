#include "basepagetool.h"
#include <QDebug>

BasePageTool::BasePageTool(QObject *parent)
    : QObject(parent)
{
    qDebug() << "BasePageTool constructed";
}

BasePageTool::~BasePageTool()
{
    // 清理所有工作线程
    for (auto thread : m_workerThreads) {
        destroyWorkerThread(thread);
    }
    m_workerThreads.clear();

    // 清理所有定时器
    for (auto timer : m_timers) {
        timer->stop();
        timer->deleteLater();
    }
    m_timers.clear();

    qDebug() << "BasePageTool destroyed";
}

QThread* BasePageTool::createWorkerThread(const QString& threadName)
{
    QThread* thread = new QThread(this);
    thread->setObjectName(threadName);
    m_workerThreads.append(thread);
    
    qDebug() << "Created worker thread:" << threadName;
    return thread;
}

void BasePageTool::destroyWorkerThread(QThread* thread)
{
    if (thread && m_workerThreads.contains(thread)) {
        QString threadName = thread->objectName();
        qInfo() << "Destroying worker thread:" << threadName;
        
        thread->quit();
        if (!thread->wait(5000)) {
            qWarning() << "Thread" << threadName << "did not exit gracefully, forcing termination";
            thread->terminate();
            thread->wait(1000);
        }
        
        m_workerThreads.removeAll(thread);
        thread->deleteLater();
        
        qInfo() << "Worker thread destroyed:" << threadName;
    }
}

QTimer* BasePageTool::createTimer(int interval, bool singleShot)
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(interval);
    timer->setSingleShot(singleShot);
    m_timers.append(timer);
    
    return timer;
} 