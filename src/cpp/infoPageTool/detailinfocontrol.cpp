#include "detailinfocontrol.h"
#include "../adb/connectmanager.h"
#include <QThread>

DetailInfoControl::DetailInfoControl(QObject *parent) : QObject(parent)
{
    m_thread = new QThread(this);
    m_helper = new DetailInfoUpdateHelper();
    m_helper->moveToThread(m_thread);
    m_thread->start();
    connect(ConnectManager::instance(), &ConnectManager::currentDeviceChanged, m_helper, &DetailInfoUpdateHelper::updateInfo);
    connect(m_helper, &DetailInfoUpdateHelper::updateFinish, this, &DetailInfoControl::updateInfo);
}

DetailInfoControl::~DetailInfoControl()
{
    qInfo() << "DetailInfoControl Thread exiting";
    m_thread->quit();
    m_thread->wait();
    qInfo() << "DetailInfoControl Thread exited";
}

void DetailInfoControl::updateInfo()
{
    m_info = m_helper->m_info;
    emit valueChanged();
}

DetailInfoUpdateHelper::DetailInfoUpdateHelper(QObject *parent) : QObject(parent)
{

}

void DetailInfoUpdateHelper::updateInfo()
{
    m_info.clear();
    auto infos = ADBInterface::instance()->getDeviceDetailInfo(ConnectManager::instance()->currentDeviceCode());
    m_info.append(infos.manufacturer);
    m_info.append(infos.brand);
    m_info.append(infos.model);
    m_info.append(infos.deviceName);
    m_info.append(infos.deviceCode);
    m_info.append(infos.systemInfo);
    m_info.append(infos.androidVersion);
    m_info.append(infos.resolving);
    m_info.append(infos.dpi);
    m_info.append(infos.macAddr);
    m_info.append(infos.ipAddr);
    m_info.append(infos.cpuInfo);
    m_info.append(infos.memory);
    m_info.append(infos.sdkVersion);
    m_info.append(infos.serialNumber);
    emit updateFinish();
}
