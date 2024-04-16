#include "detailinfocontrol.h"
#include "../adb/connectmanager.h"

DetailInfoControl::DetailInfoControl(QObject *parent) : QObject(parent)
{
    connect(ConnectManager::instance(), &ConnectManager::currentDeviceChanged, this, &DetailInfoControl::updateInfo);
}

void DetailInfoControl::updateInfo()
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
    emit valueChanged();
}
