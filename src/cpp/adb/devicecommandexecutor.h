#pragma once

#include <QObject>
#include <QFuture>
#include <QPromise>
#include <QtConcurrent>
#include <functional>
#include "connectmanager.h"
#include "../utils/notificationcontroller.h"

template<typename T>
class DeviceCommand
{
public:
    using CommandFunction = std::function<T(QSharedPointer<ADBDevice>)>;
    using SuccessCallback = std::function<void(const T&)>;
    using ErrorCallback = std::function<void(const QString&)>;

    DeviceCommand(const QString& name, CommandFunction func)
        : m_name(name), m_function(func) {}

    DeviceCommand& withSuccessMessage(const QString& message) {
        m_successMessage = message;
        return *this;
    }

    DeviceCommand& withErrorMessage(const QString& message) {
        m_errorMessage = message;
        return *this;
    }

    DeviceCommand& onSuccess(SuccessCallback callback) {
        m_successCallback = callback;
        return *this;
    }

    DeviceCommand& onError(ErrorCallback callback) {
        m_errorCallback = callback;
        return *this;
    }

    DeviceCommand& withProgressMessage(const QString& message) {
        m_progressMessage = message;
        return *this;
    }

    QFuture<T> execute() {
        auto device = ConnectManager::instance()->cutADBDevice();
        
        if (!device) {
            NotificationController::instance()->send(
                "执行失败", 
                "当前无设备连接", 
                NotificationController::Error
            );
            
            QPromise<T> promise;
            promise.start();
            if (m_errorCallback) {
                m_errorCallback("当前无设备连接");
            }
            promise.finish();
            return promise.future();
        }

        if (!m_progressMessage.isEmpty()) {
            NotificationController::instance()->send(
                "执行中", 
                m_progressMessage, 
                NotificationController::Info
            );
        }

        return QtConcurrent::run([this, device]() -> T {
            try {
                QSharedPointer<ADBDevice> devicePtr(device, [](ADBDevice*){});
                T result = m_function(devicePtr);
                
                QMetaObject::invokeMethod(QCoreApplication::instance(), [this, result]() {
                    if (!m_successMessage.isEmpty()) {
                        NotificationController::instance()->send(
                            "执行成功", 
                            m_successMessage, 
                            NotificationController::Info
                        );
                    }
                    
                    if (m_successCallback) {
                        m_successCallback(result);
                    }
                }, Qt::QueuedConnection);
                
                return result;
            } catch (const std::exception& e) {
                QMetaObject::invokeMethod(QCoreApplication::instance(), [this, e]() {
                    QString errorMsg = QString::fromStdString(e.what());
                    if (!m_errorMessage.isEmpty()) {
                        NotificationController::instance()->send(
                            "执行失败", 
                            m_errorMessage + ": " + errorMsg, 
                            NotificationController::Error
                        );
                    }
                    
                    if (m_errorCallback) {
                        m_errorCallback(errorMsg);
                    }
                }, Qt::QueuedConnection);
                
                throw;
            }
        });
    }

private:
    QString m_name;
    CommandFunction m_function;
    QString m_successMessage;
    QString m_errorMessage;
    QString m_progressMessage;
    SuccessCallback m_successCallback;
    ErrorCallback m_errorCallback;
};

// 便捷的创建函数
template<typename T>
DeviceCommand<T> createDeviceCommand(const QString& name, typename DeviceCommand<T>::CommandFunction func) {
    return DeviceCommand<T>(name, func);
}

// 特化版本，用于无返回值的命令
using VoidDeviceCommand = DeviceCommand<void>;

template<>
class DeviceCommand<void>
{
public:
    using CommandFunction = std::function<void(QSharedPointer<ADBDevice>)>;
    using SuccessCallback = std::function<void()>;
    using ErrorCallback = std::function<void(const QString&)>;

    DeviceCommand(const QString& name, CommandFunction func)
        : m_name(name), m_function(func) {}

    DeviceCommand& withSuccessMessage(const QString& message) {
        m_successMessage = message;
        return *this;
    }

    DeviceCommand& withErrorMessage(const QString& message) {
        m_errorMessage = message;
        return *this;
    }

    DeviceCommand& onSuccess(SuccessCallback callback) {
        m_successCallback = callback;
        return *this;
    }

    DeviceCommand& onError(ErrorCallback callback) {
        m_errorCallback = callback;
        return *this;
    }

    DeviceCommand& withProgressMessage(const QString& message) {
        m_progressMessage = message;
        return *this;
    }

    QFuture<void> execute()
    {
        auto device = ConnectManager::instance()->cutADBDevice();
        
        if (!device) {
            NotificationController::instance()->send(
                "执行失败", 
                "当前无设备连接", 
                NotificationController::Error
            );
            
            if (m_errorCallback) {
                m_errorCallback("当前无设备连接");
            }
            
            QPromise<void> promise;
            promise.start();
            promise.finish();
            return promise.future();
        }

        if (!m_progressMessage.isEmpty()) {
            NotificationController::instance()->send(
                "执行中", 
                m_progressMessage, 
                NotificationController::Info
            );
        }

        return QtConcurrent::run([this, device]() {
            try {
                // 将原始指针包装为智能指针给CommandFunction使用
                QSharedPointer<ADBDevice> devicePtr(device, [](ADBDevice*){});
                m_function(devicePtr);
                
                QMetaObject::invokeMethod(QCoreApplication::instance(), [this]() {
                    if (!m_successMessage.isEmpty()) {
                        NotificationController::instance()->send(
                            "执行成功", 
                            m_successMessage, 
                            NotificationController::Info
                        );
                    }
                    
                    if (m_successCallback) {
                        m_successCallback();
                    }
                }, Qt::QueuedConnection);
                
            } catch (const std::exception& e) {
                QMetaObject::invokeMethod(QCoreApplication::instance(), [this, e]() {
                    QString errorMsg = QString::fromStdString(e.what());
                    if (!m_errorMessage.isEmpty()) {
                        NotificationController::instance()->send(
                            "执行失败", 
                            m_errorMessage + ": " + errorMsg, 
                            NotificationController::Error
                        );
                    }
                    
                    if (m_errorCallback) {
                        m_errorCallback(errorMsg);
                    }
                }, Qt::QueuedConnection);
                
                throw;
            }
        });
    }

private:
    QString m_name;
    CommandFunction m_function;
    QString m_successMessage;
    QString m_errorMessage;
    QString m_progressMessage;
    SuccessCallback m_successCallback;
    ErrorCallback m_errorCallback;
}; 