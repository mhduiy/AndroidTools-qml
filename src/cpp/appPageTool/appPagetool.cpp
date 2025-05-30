#include "appPagetool.h"
#include "../adb/connectmanager.h"
#include "appDetailControl.h"
#include <QtQml>
#include <QDebug>

AppPageTool::AppPageTool(QObject *parent)
: QObject(parent)
, m_softListModel(new SoftListModel(this))
, m_appHandle(new AppInfoHandle)
, m_appHandleThread(new QThread(this))
{
    qmlRegisterSingletonInstance("SoftListModel", 1, 0, "SoftListModel", m_softListModel);
    qmlRegisterSingletonInstance("AppDetailControl", 1, 0, "AppDetailControl", AppDetailControl::instance(this));

    m_appHandle->moveToThread(m_appHandleThread);
    m_appHandleThread->start();
    connect(AppDetailControl::instance(), &AppDetailControl::requestUpdateSoftList, m_appHandle, &AppInfoHandle::updateListInfo);
    connect(ConnectManager::instance(), &ConnectManager::cutADBDeviceChanged, m_appHandle, &AppInfoHandle::updateListInfo);
    connect(ConnectManager::instance(), &ConnectManager::cutADBDeviceChanged, this, [this](){m_softListModel->clearData();});
    connect(m_appHandle, &AppInfoHandle::updateListFinish, this, &AppPageTool::updateAppListInfo);
}

AppPageTool::~AppPageTool()
{
    qInfo() << "AppPageTool Thread exiting";
    m_appHandleThread->quit();
    m_appHandleThread->wait();
    qInfo() << "AppPageTool Thread exited";
}

void AppPageTool::updateAppListInfo()
{
    m_softListModel->clearData();
    for (const auto &info : m_appHandle->getInfo()) {
        m_softListModel->appendRow(info);
    }
}

AppInfoHandle::AppInfoHandle(QObject *parent) : QObject(parent)
{
    
}

void AppInfoHandle::updateListInfo()
{
    auto device = ConnectManager::instance()->cutADBDevice();
    if (!device) {
        m_info.clear();
        emit updateListFinish();
        return;
    }
    
    auto infos = device->getSoftListInfo();
    m_info = infos;
    emit updateListFinish();
}

QList<AppListInfo> AppInfoHandle::getInfo()
{
    return m_info;
}
