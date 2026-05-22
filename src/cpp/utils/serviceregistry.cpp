#include "serviceregistry.h"
#include "taskthread.h"

#include "src/cpp/adb/adbtools.h"
#include "src/cpp/adb/connectmanager.h"
#include "src/cpp/adb/devicehelper.h"
#include "src/cpp/adb/adblog.h"
#include "src/cpp/utils/globalsetting.h"
#include "src/cpp/utils/notificationcontroller.h"
#include "src/cpp/utils/keyconvert.h"
#include "src/cpp/app/appglobal.h"
#include "src/cpp/settingPageTool/othersettingshandler.h"
#include "src/cpp/settingPageTool/wallpaperhelper.h"
#include "src/cpp/controlPageTool/controlPageTool.h"
#include "src/cpp/AppPageTool/AppPageTool.h"
#include "src/cpp/flashPageTool/flashPageTool.h"
#include "src/cpp/imagePageTool/imagePageTool.h"
#include "src/cpp/settingPageTool/settingPageTools.h"

#include <QDebug>

ServiceRegistry::ServiceRegistry(QObject *parent)
    : QObject(parent)
{
}

ServiceRegistry::~ServiceRegistry()
{
    qInfo() << "ServiceRegistry shutting down...";
}

void ServiceRegistry::initialize()
{
    qInfo() << "ServiceRegistry initializing...";

    m_taskThread = new TaskThread(this);

    initLeafServices();
    initADBInfrastructure();
    initScrcpyServices();
    initPageTools();

    qInfo() << "ServiceRegistry initialization complete.";
}

void ServiceRegistry::initLeafServices()
{
    GlobalSetting::instance(this);
    NotificationController::instance(this);
    ADBLogModel::instance(this);
    KeyConvert::instance(this);
    ADT::ADBTools::instance(this);
    AppGlobal::instance(this);

    OtherSettingsHandler::instance(this);
    WallpaperHelper::instance(this);
}

void ServiceRegistry::initADBInfrastructure()
{
    ADT::ConnectManager::instance(this);
    ADT::DeviceHelper::instance(this);
}

void ServiceRegistry::initScrcpyServices()
{
    ADT::ImagePageTool::instance(this);
}

void ServiceRegistry::initPageTools()
{
    ADT::ControlPageTool::instance(this);
    ADT::AppPageTool::instance(this);
    ADT::FlashPageTool::instance(this);
    ADT::SettingPageTools::instance(this);
}
