
#include <QQuickView>
#include <QQmlContext>
#include <QFileInfo>

#include "service/ServiceManager.h"

#include "MainWindow.h"
#include "ui/util/config.h"
#include "ui/mirror/MirrorScene.h"

MainWindow::MainWindow(QQuickView *parent) : QQuickView(parent = nullptr) {

    QString str = QCoreApplication::applicationDirPath();
    int index = str.indexOf("/.output");
    QString leftSide = str.left(index);

    Config::getInstance().setProjectPath(leftSide);

    resourceService = new ResourceService(this);
    ServiceManager::getInstance().setResourceService(resourceService); // Register service to our C++ singleton

    m_broadcast = new Broadcast(); // UDP broadcast to clients can find server data
    m_broadcast->start();

    new WebSocketService(8097, this);

    // Declare/Register all used custom QML elements
    //Scene::declareQml();
    //SceneProvider::declareQml();

    ResourceService::declareQml();
    MirrorScene::declareQml();

    QQmlContext *rootContext;
    rootContext = this->rootContext();
    rootContext->setContextProperty("resource", resourceService); // Also set it to QML root context

    setKeyboardGrabEnabled(true);
    setSource(QUrl("/home/mhduiy/Dev/QmlScrcpy/res/qml/main.qml"));
    setResizeMode(QQuickView::SizeRootObjectToView);
}

MainWindow::~MainWindow() {
    qsc::IDeviceManage::getInstance().disconnectAllDevice();
    this->destroy();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    auto device = qsc::IDeviceManage::getInstance().getDevice(resourceService->serial());
    if (!device) {
        return;
    }

    if (resourceService->orientation() == 0) {
        emit device->keyEvent(event, resourceService->frameSize(), resourceService->portraitSize());
    } else {
        emit device->keyEvent(event, resourceService->frameSize(), resourceService->landscapeSize());
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {

    auto device = qsc::IDeviceManage::getInstance().getDevice(resourceService->serial());
    if (!device) {
        return;
    }

    if (resourceService->orientation() == 0) {
        emit device->keyEvent(event, resourceService->frameSize(), resourceService->portraitSize());
    } else {
        emit device->keyEvent(event, resourceService->frameSize(), resourceService->landscapeSize());
    }
}

