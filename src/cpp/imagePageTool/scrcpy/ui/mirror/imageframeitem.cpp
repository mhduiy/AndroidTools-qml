#include "imageframeitem.h"
#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

ImageFrameItem::ImageFrameItem(QQuickItem *parent) : QQuickItem(parent)
{
    setFlags(QQuickItem::ItemHasContents);
}

QSGNode *ImageFrameItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    auto node = dynamic_cast<QSGSimpleTextureNode *>(oldNode);
    if (!node) node = new QSGSimpleTextureNode();

    auto alphaTextureOption = m_hasAlphaChannel ? QQuickWindow::TextureHasAlphaChannel : QQuickWindow::TextureIsOpaque;

    if (!m_texture || m_imageFrame != m_lastImageFrame) {
        if (m_texture) {
            delete m_texture;
        }
        m_texture = window()->createTextureFromImage(m_imageFrame, alphaTextureOption);
        m_lastImageFrame = m_imageFrame;
    }
    node->setTexture(m_texture);
    node->setOwnsTexture(false);
    node->setRect(boundingRect());
    node->markDirty(QSGNode::DirtyForceUpdate);

    return node;
}

void ImageFrameItem::setImageFrame(const QImage& newImageFrame)
{
    if (newImageFrame == m_imageFrame) return;
    QMetaObject::invokeMethod(this, [this, newImageFrame]() {
        m_imageFrame = newImageFrame;
        emit imageFrameChanged();
        update();
    }, Qt::QueuedConnection);
}

void ImageFrameItem::setHasAlphaChannel(bool newHasAlphaChannel)
{
    if (m_hasAlphaChannel == newHasAlphaChannel) return;
    QMetaObject::invokeMethod(this, [this, newHasAlphaChannel]() {
        m_hasAlphaChannel = newHasAlphaChannel;
        emit hasAlphaChannelChanged();
        update();
    }, Qt::QueuedConnection);
}