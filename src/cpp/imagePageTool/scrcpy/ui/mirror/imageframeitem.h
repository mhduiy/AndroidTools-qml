//imageframeitem.h
#pragma once

#include <QQuickItem>
#include <QImage>
#include <QSGTexture>

class ImageFrameItem : public QQuickItem
{
    Q_OBJECT
    // QML_ELEMENT

    Q_PROPERTY(QImage imageFrame READ imageFrame WRITE setImageFrame NOTIFY imageFrameChanged)
    Q_PROPERTY(bool hasAlphaChannel READ hasAlphaChannel WRITE setHasAlphaChannel NOTIFY hasAlphaChannelChanged)

    QImage m_imageFrame;
    bool m_hasAlphaChannel = false;
    QSGTexture *m_texture = nullptr;
    QImage m_lastImageFrame;

public:
    ImageFrameItem(QQuickItem *parent = nullptr);
    QImage imageFrame() const;
    bool hasAlphaChannel() const;
    void setImageFrame(const QImage& newImageFrame);
    void setHasAlphaChannel(bool newHasAlphaChannel);

signals:
    void imageFrameChanged();
    void hasAlphaChannelChanged();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};
