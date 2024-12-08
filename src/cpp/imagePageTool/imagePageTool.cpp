#include "imagePageTool.h"
#include <qobject.h>
#include "src/cpp/adb/adbinterface.h"
#include "src/cpp/adb/connectmanager.h"
#include "src/cpp/utils/notificationcontroller.h"
#include "src/cpp/utils/utils.hpp"
#include "ui/util/config.h"
#include "service/ServiceManager.h"
#include "ui/mirror/MirrorScene.h"
#include "service/WebSocketService.h"

ImageDetailTools::ImageDetailTools(QObject *parent)
    : QObject(parent)
{

}

void ImageDetailTools::shotScreen(const QString &outPath)
{
    const QString &deviceCode =  ConnectManager::instance()->currentDeviceCode();
    if (deviceCode.isEmpty()) {
        NotificationController::instance()->send("截图失败", "当前无设备连接", NotificationController::Error, 3000);
        return;
    }

    auto func = [deviceCode, outPath](){
        NotificationController::instance()->send("命令已发送", "正在截图...", NotificationController::Info, 5000);
        ADBInterface::instance()->shotScreen(deviceCode, outPath);
    };

    asyncOperator(func);
}

ImagePageTool::ImagePageTool(QObject *parent)
: QObject(parent)
{
    QString str = QCoreApplication::applicationDirPath();
    int index = str.indexOf("/.output");
    QString leftSide = str.left(index);

    Config::getInstance().setProjectPath(leftSide);

    resourceService = new ResourceService(this);
    ServiceManager::getInstance().setResourceService(resourceService); // Register service to our C++ singleton

    new WebSocketService(8097, this);

    ResourceService::declareQml();
    MirrorScene::declareQml();
    Config::declareQml();

    qmlRegisterSingletonInstance("Resource", 1, 0, "Resource", resourceService);
    qmlRegisterSingletonInstance("ImageDetailTools", 1, 0, "ImageDetailTools", ImageDetailTools::instance(this));
}