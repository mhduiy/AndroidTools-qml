#pragma once

#include <QQuickItem>

class FpsItem : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(int fps WRITE setFps READ getFps NOTIFY fpsChanged)
public:
    FpsItem();
    void setFps(int);
    [[nodiscard]] inline int getFps() const { return m_fps; };

signals:
    void fpsChanged(int);

private:
    int _frameCount = 0;
    int m_fps = 0;
};