#pragma once

#include <QObject>
#include <QThreadPool>
#include <QRunnable>
#include <functional>

class TaskThread : public QObject
{
    Q_OBJECT
public:
    explicit TaskThread(QObject *parent = nullptr)
        : QObject(parent)
    {
        m_pool.setMaxThreadCount(2);
    }

    void postTask(std::function<void()> task)
    {
        class FnRunnable : public QRunnable
        {
        public:
            FnRunnable(std::function<void()> fn) : m_fn(std::move(fn)) {}
            void run() override { m_fn(); }
        private:
            std::function<void()> m_fn;
        };
        m_pool.start(new FnRunnable(std::move(task)));
    }

private:
    QThreadPool m_pool;
};
