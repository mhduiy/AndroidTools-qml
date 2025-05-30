#pragma once

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QtQml>
#include "../utils/defutils.hpp"
#include "../adb/devicecommandexecutor.h"

class BasePageTool : public QObject
{
    Q_OBJECT

public:
    explicit BasePageTool(QObject *parent = nullptr);
    virtual ~BasePageTool();

protected:
    // 通用的异步操作执行器
    template<typename T>
    QFuture<T> executeDeviceCommand(
        const QString& commandName,
        std::function<T(QSharedPointer<ADBDevice>)> command,
        const QString& progressMessage = "",
        const QString& successMessage = "",
        const QString& errorMessage = ""
    ) {
        return createDeviceCommand<T>(commandName, command)
            .withProgressMessage(progressMessage)
            .withSuccessMessage(successMessage)
            .withErrorMessage(errorMessage)
            .execute();
    }

    // 特化版本，用于无返回值的命令
    QFuture<void> executeDeviceCommand(
        const QString& commandName,
        std::function<void(QSharedPointer<ADBDevice>)> command,
        const QString& progressMessage = "",
        const QString& successMessage = "",
        const QString& errorMessage = ""
    ) {
        return VoidDeviceCommand(commandName, command)
            .withProgressMessage(progressMessage)
            .withSuccessMessage(successMessage)
            .withErrorMessage(errorMessage)
            .execute();
    }

    // 工具线程管理
    QThread* createWorkerThread(const QString& threadName);
    void destroyWorkerThread(QThread* thread);

    // QML 注册辅助函数
    template<typename T>
    void registerSingletonInstance(const QString& module, int major, int minor, const QString& name, T* instance) {
        qmlRegisterSingletonInstance(module.toUtf8().constData(), major, minor, name.toUtf8().constData(), instance);
    }

    // 定时器管理
    QTimer* createTimer(int interval, bool singleShot = false);

    // QML类型初始化 - 子类可以重写此方法
    virtual void initializeQmlTypes() {}

private:
    QList<QThread*> m_workerThreads;
    QList<QTimer*> m_timers;
};

// 宏定义简化 PageTool 的创建
#define DECLARE_PAGE_TOOL(ClassName) \
    class ClassName : public BasePageTool \
    { \
        Q_OBJECT \
        SINGLETON(ClassName) \
    public: \
        explicit ClassName(QObject *parent = nullptr); \
        ~ClassName(); \
    private: \
        void initializeQmlTypes() override; \
    };

// 宏定义简化构造函数的实现
#define IMPLEMENT_PAGE_TOOL_CONSTRUCTOR(ClassName) \
    ClassName::ClassName(QObject *parent) : BasePageTool(parent) \
    { \
        initializeQmlTypes(); \
    } \
    \
    ClassName::~ClassName() \
    { \
        qInfo() << #ClassName " destroyed"; \
    } 