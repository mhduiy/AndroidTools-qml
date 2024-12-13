#include "fpsitem.h"

#include <QTimer>
#include <QQuickWindow>

FpsItem::FpsItem() {
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] {
        setFps(_frameCount);
        _frameCount = 0;
    });
    connect(this, &QQuickItem::windowChanged, this, [this] {
        if (window()) {
            connect(window(), &QQuickWindow::afterRendering, this, [this] { _frameCount++; }, Qt::DirectConnection);
        }
    });
    timer->start(1000);
}

void FpsItem::setFps(int fps)
{
    m_fps = fps;
    Q_EMIT fpsChanged(fps);
}
