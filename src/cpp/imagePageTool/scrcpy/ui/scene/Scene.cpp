#include <QQmlEngine>

#include "Scene.h"
#include "SceneRenderer.h"

Scene::Scene(QQuickItem *parent) : QQuickFramebufferObject(parent) {

    m_timer.setInterval(16);
    connect(&m_timer, &QTimer::timeout, this, &Scene::update);
    m_timer.start();

    setTextureFollowsItemSize(true);

    m_resourceService = ServiceManager::getInstance().resourceService();
    setWidth(40);
    setHeight(60);

    setAcceptedMouseButtons(Qt::AllButtons);
}

void Scene::declareQml() {
    qmlRegisterType<Scene>("App", 1, 0, "Scene");
}

QQuickFramebufferObject::Renderer *Scene::createRenderer() const {
    return new SceneRenderer;
}

void Scene::mouseProcess(QMouseEvent *event) {
    auto device = qsc::IDeviceManage::getInstance().getDevice(m_resourceService->serial());
    if (!device) {
        return;
    }
    if (m_resourceService->orientation() == 0)
            emit device->mouseEvent(event, m_resourceService->frameSize(), m_resourceService->portraitSize());
    else
            emit device->mouseEvent(event, m_resourceService->frameSize(), m_resourceService->landscapeSize());
}

void Scene::mousePressEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void Scene::mouseReleaseEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void Scene::mouseMoveEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void Scene::mouseDoubleClickEvent(QMouseEvent *event) {
    mouseProcess(event);
}


void Scene::wheelEvent(QWheelEvent *) {

    // QWheelEvent wheelEvent(
    //         event->pos(), event->globalPosF(), event->pixelDelta(), event->angleDelta(), event->delta(),
    //         event->orientation(),
    //         event->buttons(), event->modifiers(), event->phase(), event->source(), event->inverted());

    // auto device = qsc::IDeviceManage::getInstance().getDevice(m_resourceService->serial());
    // if (!device) {
    //     return;
    // }
    // if (m_resourceService->orientation() == 0)
    //         emit device->wheelEvent(&wheelEvent, m_resourceService->frameSize(), m_resourceService->portraitSize());
    // else
    //         emit device->wheelEvent(&wheelEvent, m_resourceService->frameSize(), m_resourceService->landscapeSize());
}
