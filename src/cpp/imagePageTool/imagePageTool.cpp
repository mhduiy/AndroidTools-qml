#include "imagePageTool.h"
#include <qobject.h>
#include "ui/util/config.h"
#include "service/ServiceManager.h"
#include "ui/mirror/MirrorScene.h"
#include "service/WebSocketService.h"

ImagePageTool::ImagePageTool(QObject *parent)
: QObject(parent)
{
    QString str = QCoreApplication::applicationDirPath();
    int index = str.indexOf("/.output");
    QString leftSide = str.left(index);

    Config::getInstance().setProjectPath(leftSide);

    resourceService = new ResourceService(this);
    ServiceManager::getInstance().setResourceService(resourceService); // Register service to our C++ singleton

    m_broadcast = new Broadcast(); // UDP broadcast to clients can find server data
    m_broadcast->start();

    new WebSocketService(8097, this);

    ResourceService::declareQml();
    MirrorScene::declareQml();

    qmlRegisterSingletonInstance("Resource", 1, 0, "Resource", resourceService);
}