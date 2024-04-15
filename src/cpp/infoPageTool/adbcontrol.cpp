#include "adbcontrol.h"
#include "../adb/adbinterface.h"

ADBControl::ADBControl(QObject *parent) : QObject(parent)
{
    m_adbVersion = ADBInterface::instance()->adbVersion();
    connect(ADBInterface::instance(), &ADBInterface::adbStarted, this, &ADBControl::updateADBVersion);
}

void ADBControl::restartADB()
{
    ADBInterface::instance()->restartADBService();
}

void ADBControl::updateADBVersion()
{
    m_adbVersion = ADBInterface::instance()->adbVersion();
    emit valueChanged();
}
