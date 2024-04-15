#include "cutactivitycontrol.h"
#include <QDebug>
#include "../adb/connectmanager.h"
#include "../adb/adbinterface.h"

CutActivityControl::CutActivityControl(QObject *parent) : QObject(parent)
{
    m_identifier = "-";
    m_cutPackageName = "-";
    m_cutActivity = "-";

    connect(ConnectManager::instance(), &ConnectManager::deviceRefreshFinish, this, &CutActivityControl::updateInfo);
}

void CutActivityControl::updateInfo()
{
    const DeviceCutActivityInfo &&info = ConnectManager::instance()->getDeviceCutActivityInfo();
    m_identifier = info.windowCode;
    m_cutPackageName = info.cutPackage;
    m_cutActivity = info.cutActivity;
    emit valueChanged();
}

void CutActivityControl::killCutActivity()
{
    ADBInterface::instance()->killActivity(m_cutPackageName);
}
