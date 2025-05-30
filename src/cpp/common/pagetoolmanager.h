#pragma once

#include <QObject>
#include <QHash>
#include <QQmlApplicationEngine>
#include "../utils/defutils.hpp"

class BasePageTool;

/**
 * 页面工具管理器
 * 统一管理所有PageTool的生命周期和QML注册
 */
class PageToolManager : public QObject
{
    Q_OBJECT
    SINGLETON(PageToolManager)

public:
    // PageTool信息结构
    struct PageToolInfo {
        QString name;
        QString qmlModule;
        int majorVersion;
        int minorVersion;
        std::function<BasePageTool*()> factory;
        BasePageTool* instance;
        
        PageToolInfo() : majorVersion(1), minorVersion(0), instance(nullptr) {}
    };

    // 注册PageTool
    template<typename T>
    void registerPageTool(const QString& name, const QString& qmlModule = "", int major = 1, int minor = 0) {
        PageToolInfo info;
        info.name = name;
        info.qmlModule = qmlModule.isEmpty() ? name : qmlModule;
        info.majorVersion = major;
        info.minorVersion = minor;
        info.factory = []() -> BasePageTool* {
            return T::instance();
        };
        info.instance = nullptr;
        
        m_pageTools[name] = info;
        qDebug() << "Registered PageTool:" << name;
    }

    // 初始化所有PageTool
    void initializeAll();
    
    // 获取PageTool实例
    template<typename T>
    T* getPageTool(const QString& name) {
        return qobject_cast<T*>(getPageToolInstance(name));
    }
    
    // 销毁所有PageTool
    void destroyAll();
    
    // 注册到QML引擎
    void registerToQml(QQmlApplicationEngine* engine);

private:
    BasePageTool* getPageToolInstance(const QString& name);
    
    QHash<QString, PageToolInfo> m_pageTools;
    QQmlApplicationEngine* m_qmlEngine;
}; 