#pragma once

#include <QQuickFramebufferObject>
#include <QTimer>

#include <QPointer>
#include "service/ResourceService.h"
#include "service/ServiceManager.h"

//!
//! A simple Framebuffer object which will render the saved framebuffer from SceneProvider
//! Scene will live on MainThread
//!

class Scene : public QQuickFramebufferObject {

Q_OBJECT

public:
    explicit Scene(QQuickItem *parent = nullptr);

    static void declareQml();

    virtual Renderer *createRenderer() const override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void mouseProcess(QMouseEvent *event);

private:
    QTimer m_timer;
    ResourceService* m_resourceService = nullptr;


};
