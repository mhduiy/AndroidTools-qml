#pragma once

#include <QQuickFramebufferObject>
#include <QQuickItem>
#include <memory>

#include "core/include/QtScrcpyCore.h"
#include "service/ResourceService.h"

class MirrorScene : public QQuickFramebufferObject
{

Q_OBJECT

public:
    MirrorScene(QQuickItem* parent = nullptr);

    void timerEvent(QTimerEvent* event) override;
    static void declareQml();

    bool infoDirty() const
    {
        return m_infoChanged;
    }
    void makeInfoDirty(bool dirty)
    {
        m_infoChanged = dirty;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void mouseProcess(QMouseEvent *event);

signals:
    void cppGenerateEvents(QString request, QString data);

public:
    Renderer* createRenderer() const override;

    ResourceService* m_resourceService = nullptr;
    bool m_infoChanged = false;
};