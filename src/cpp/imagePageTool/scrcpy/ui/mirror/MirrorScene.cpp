#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQuickFramebufferObject>

#include "QtScrcpyCore.h"
#include "MirrorRenderer.h"
#include "MirrorScene.h"
#include "service/ServiceManager.h"


class MirrorItemRender : public QQuickFramebufferObject::Renderer {

public:
    MirrorItemRender();

    void render() override;

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

    void synchronize(QQuickFramebufferObject *) override;

private:
    MirrorRenderer m_render;
    QQuickWindow *m_window = nullptr;
};

MirrorItemRender::MirrorItemRender() : m_render(0) {
    m_render.init();
}

void MirrorItemRender::render() {
    m_window->beginExternalCommands();
    m_render.paint();
    m_window->endExternalCommands();
}

QOpenGLFramebufferObject *MirrorItemRender::createFramebufferObject(const QSize &size) {
    Q_UNUSED(size);
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    m_render.resize(size.width(), size.height());
    return new QOpenGLFramebufferObject(size, format);
}

void MirrorItemRender::synchronize(QQuickFramebufferObject *item) {

    MirrorScene *pItem = qobject_cast<MirrorScene *>(item);

    if (pItem) {
        if (!m_window) {
            m_window = pItem->window();
        }

        if (pItem->infoDirty()) {
            //m_render.updateTextureInfo(pItem->videoWidth(), pItem->videoHeght(), pItem->videoFormat());
            pItem->makeInfoDirty(false);
        }
    }
}

MirrorScene::MirrorScene(QQuickItem *parent) : QQuickFramebufferObject(parent) {

    m_resourceService = ServiceManager::getInstance().resourceService();
    connect(m_resourceService, &ResourceService::cppGenerateEvents, this, &MirrorScene::cppGenerateEvents); //Interacting C++ to Qml

    //Update the interface at a frame rate of 60 frames per second
    startTimer(1000 / 60);
    setAcceptedMouseButtons(Qt::AllButtons);
}

void MirrorScene::declareQml() {
    qmlRegisterType<MirrorScene>("MirrorScene", 1, 0, "MirrorScene");
}

void MirrorScene::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    update();
}

QQuickFramebufferObject::Renderer *MirrorScene::createRenderer() const {
    return new MirrorItemRender;
}

void MirrorScene::mouseProcess(QMouseEvent *event) {
    auto device = qsc::IDeviceManage::getInstance().getDevice(m_resourceService->serial());
    if (!device) {
        return;
    }
    if (m_resourceService->orientation() == 0) {
        emit device->mouseEvent(event, m_resourceService->frameSize(), m_resourceService->portraitSize());
    }
    else
        emit device->mouseEvent(event, m_resourceService->frameSize(), m_resourceService->landscapeSize());
}

void MirrorScene::mousePressEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::mouseReleaseEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::mouseMoveEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::mouseDoubleClickEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::wheelEvent(QWheelEvent *event) {

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