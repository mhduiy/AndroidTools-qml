#pragma once

#include "SceneProviderRenderer.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QString>

//!
//! SceneProvider will act as a BackgroundRenderer on the MainThread, its child SceneProviderRenderer will do the actual
//! rendering on RenderThread

class SceneProvider : public QQuickItem {
    Q_OBJECT
public:
    explicit SceneProvider(QQuickItem* parent = nullptr);

    static void declareQml();

protected:
    virtual void releaseResources() override;

private slots:
    void onWindowChanged(QQuickWindow* window);

signals:
    void cppGenerateEvents(QString request, QString data);

private:
    SceneProviderRenderer* m_sceneProviderRenderer = nullptr;
    ResourceService* m_resourceService = nullptr;

    QMetaObject::Connection m_connectionInitialized;
};
