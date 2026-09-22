#pragma once

#include <QQuickItem>
#include <QImage>
#include "service/ResourceService.h"

class MirrorScene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image NOTIFY imageChanged)
public:
    explicit MirrorScene(QQuickItem* parent = nullptr);
    static void declareQml();

    QImage image() const;
    Q_INVOKABLE void onYuvFrame(int width, int height, uint8_t *dataY, uint8_t *dataU, uint8_t *dataV, int linesizeY, int linesizeU, int linesizeV);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void mouseProcess(QMouseEvent *event);

signals:
    void cppGenerateEvents(QString request, QString data);
    void imageChanged();

public:
    ResourceService* m_resourceService = nullptr;
    QImage m_image;
};