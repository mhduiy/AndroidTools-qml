#include "imagePageTool.h"
#include <qobject.h>
#include "src/cpp/adb/connectmanager.h"
#include "src/cpp/utils/notificationcontroller.h"
#include "src/cpp/utils/utils.hpp"
#include "ui/util/config.h"
#include "service/ServiceManager.h"
#include "ui/mirror/MirrorScene.h"
#include "service/WebSocketService.h"
#include "src/cpp/utils/constants.h"

ImageDetailTools::ImageDetailTools(QObject *parent)
    : QObject(parent)
{

}

void ImageDetailTools::shotScreen(const QString &outPath)
{
    auto device = ConnectManager::instance()->cutADBDevice();
    if (!device) {
        NotificationController::instance()->send("截图失败", "当前无设备连接", NotificationController::Error, 3000);
        return;
    }

    auto func = [device, outPath](){
        NotificationController::instance()->send("命令已发送", "正在截图...", NotificationController::Info, 5000);
        device->shotScreen(outPath);
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

    checkScrcpyServer();

    qmlRegisterSingletonInstance("Resource", 1, 0, "Resource", resourceService);
    qmlRegisterSingletonInstance("ImageDetailTools", 1, 0, "ImageDetailTools", ImageDetailTools::instance(this));
}

void ImagePageTool::checkScrcpyServer()
{
    if (!QFile::exists(SCRCPYSERVERPATH)) {
        // 确保目标目录存在
        QFileInfo fileInfo(SCRCPYSERVERPATH);
        QDir dir = fileInfo.dir();
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QFile resourceFile(":/res/scrcpy-server");
        if (!resourceFile.exists()) {
            qWarning() << "Resource file does not exist!";
            return;
        }
        if (!resourceFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open resource file:" << resourceFile.error();
            return;
        }
        QFile targetFile(SCRCPYSERVERPATH);
        if (!targetFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Cannot open target file:" << targetFile.error();
            return;
        }
        QByteArray data = resourceFile.readAll();
        if (targetFile.write(data) == -1) {
            qWarning() << "Failed to write file:" << targetFile.error();
            return;
        }
        resourceFile.close();
        targetFile.close();    
    }
}
