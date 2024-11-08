#pragma once

#include <QObject>
#include <QTimer>
#include "../utils/singleton.hpp"

class RealTimeInfoWorker;

class RealTimeInfoHelper : public QObject
{
    Q_OBJECT
    SINGLETON(RealTimeInfoHelper)
    Q_PROPERTY(double cpuTmp READ cpuTmp WRITE setCpuTmp NOTIFY cpuTmpChanged)
    Q_PROPERTY(int cpuUse READ cpuUse WRITE setCpuUse NOTIFY cpuUseChanged)
    Q_PROPERTY(int memUse READ memUse WRITE setMemUse NOTIFY memUseChanged)
    Q_PROPERTY(int disUse READ disUse WRITE setDisUse NOTIFY disUseChanged)
    Q_PROPERTY(double btyTmp READ btyTmp WRITE setBtyTmp NOTIFY btyTmpChanged)
public:
    ~RealTimeInfoHelper();

    inline double cpuTmp() const { return m_cpuTmp; }
    inline int cpuUse() const { return m_cpuUse; }
    inline int memUse() const { return m_memUse; }
    inline int disUse() const { return m_disUse; }
    inline double btyTmp() const { return m_btyTmp; }


public slots:
    void setCpuTmp(double value);
    void setCpuUse(int value);
    void setMemUse(int value);
    void setDisUse(int value);
    void setBtyTmp(double value);

signals:
    void cpuTmpChanged(double value);
    void cpuUseChanged(int value);
    void memUseChanged(int value);
    void disUseChanged(int value);
    void btyTmpChanged(double value);

private:
    RealTimeInfoWorker *m_woker;
    QThread *m_workerThread;

    double m_cpuTmp;
    int m_cpuUse;
    int m_memUse;
    int m_disUse;
    double m_btyTmp;
};

class RealTimeInfoWorker : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeInfoWorker(RealTimeInfoHelper *helper, QObject *parent = nullptr);
    void startRealTimeCheck();
    void stopRealTimeCheck();

private slots:
    void onTimeOut();

private:
    void refreshMemUse();

private:
    QTimer *m_timer;
    RealTimeInfoHelper *m_helper;
};
