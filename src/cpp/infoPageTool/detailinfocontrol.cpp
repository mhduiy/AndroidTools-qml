#include "detailinfocontrol.h"
#include "../adb/connectmanager.h"

DetailInfoControl::DetailInfoControl(QObject *parent) : QObject(parent)
{
    connect(ConnectManager::instance(), &ConnectManager::currentDeviceChanged, this, &DetailInfoControl::updateInfo);
}

void DetailInfoControl::updateInfo()
{

}
