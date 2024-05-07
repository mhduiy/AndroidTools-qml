#include "appPagetool.h"
#include "../adb/connectmanager.h"
#include "appDetailControl.h"
#include <QtQml>

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
    connect(ConnectManager::instance(), &ConnectManager::currentDeviceChanged, m_appHandle, &AppInfoHandle::updateListInfo);
    connect(ConnectManager::instance(), &ConnectManager::currentDeviceChanged, this, [this](){m_softListModel->clearData();});
    connect(m_appHandle, &AppInfoHandle::updateListFinish, this, &AppPageTool::updateAppListInfo);
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
    auto infos = ADBInterface::instance()->getSoftListInfo(ConnectManager::instance()->currentDeviceCode());
    m_info = infos;
    emit updateListFinish();
}
QList<AppListInfo> AppInfoHandle::getInfo()
{
    return m_info;
}
