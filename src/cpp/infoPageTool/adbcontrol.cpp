#include "adbcontrol.h"
#include <QtConcurrent/QtConcurrentRun>
#include "../adb/adbinterface.h"

ADBControl::ADBControl(QObject *parent) : QObject(parent)
{
    m_adbVersion = ADBInterface::instance()->adbVersion();
    connect(ADBInterface::instance(), &ADBInterface::adbStarted, this, &ADBControl::updateADBVersion);
}

void ADBControl::restartADB()
{
    (void)QtConcurrent::run([](){
        ADBInterface::instance()->restartADBService();
    });
}

void ADBControl::updateADBVersion()
{
    m_adbVersion = ADBInterface::instance()->adbVersion();
    emit valueChanged();
}
