//imageframeitem.h
#pragma once

#include <QQmlEngine>
#include <QQuickItem>
#include <QImage>
#include <QtQml>
#include <QSGTexture>

class ImageFrameItem : public QQuickItem
{
    Q_OBJECT
    // QML_ELEMENT

    Q_PROPERTY(QImage imageFrame MEMBER m_imageFrame WRITE setImageFrame NOTIFY imageFrameChanged)
    Q_PROPERTY(bool hasAlphaChannel MEMBER m_hasAlphaChannel WRITE setHasAlphaChannel NOTIFY hasAlphaChannelChanged)

    QImage m_imageFrame;
    bool m_hasAlphaChannel = false;
    QSGTexture *m_texture = nullptr;
    QImage m_lastImageFrame;

public:
    ImageFrameItem(QQuickItem *parent = nullptr);
    void setImageFrame(const QImage& newImageFrame);
    void setHasAlphaChannel(bool newHasAlphaChannel);

signals:
    void imageFrameChanged();
    void hasAlphaChannelChanged();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};
