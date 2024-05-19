#include "SceneProvider.h"
#include "service/ServiceManager.h"

#include <QDebug>
#include <QQmlEngine>
#include <QRunnable>
#include <QThread>

namespace {
    constexpr int SceneWidth  = 400;
    constexpr int SceneHeight = 900;
} // namespace


class CleanupJob : public QRunnable {
    Q_DISABLE_COPY(CleanupJob)
public:
    explicit CleanupJob(SceneProviderRenderer* sceneProviderRenderer) : m_sceneProviderRenderer(sceneProviderRenderer) {}
    void run() override { delete m_sceneProviderRenderer; }

private:
    SceneProviderRenderer* m_sceneProviderRenderer;
};

SceneProvider::SceneProvider(QQuickItem* parent) : QQuickItem(parent) {

    m_resourceService = ServiceManager::getInstance().resourceService();
    connect(m_resourceService, &ResourceService::cppGenerateEvents, this, &SceneProvider::cppGenerateEvents); //Interacting C++ to Qml

    connect(this, &QQuickItem::windowChanged, this, &SceneProvider::onWindowChanged);

}

void SceneProvider::declareQml() {
    qmlRegisterType<SceneProvider>("App", 1, 0, "SceneProvider");
}

void SceneProvider::releaseResources() {
    window()->scheduleRenderJob(new CleanupJob(m_sceneProviderRenderer), QQuickWindow::BeforeSynchronizingStage);
    m_sceneProviderRenderer = nullptr;
}

void SceneProvider::onWindowChanged(QQuickWindow* window) {

    m_connectionInitialized = connect(window, &QQuickWindow::sceneGraphInitialized, [this, window] {
        disconnect(m_connectionInitialized);

        // We are currenthly in a render thread
        m_sceneProviderRenderer = new SceneProviderRenderer();
        m_sceneProviderRenderer->init(window, {SceneWidth, SceneHeight});

        // Make sure we are connecting dirrectly so the signals won't get queued
        connect(window,
                &QQuickWindow::beforeSynchronizing,
                m_sceneProviderRenderer,
                &SceneProviderRenderer::synchronize,
                Qt::DirectConnection);

        connect(window, //
                &QQuickWindow::beforeRendering,
                m_sceneProviderRenderer,
                &SceneProviderRenderer::render,
                Qt::DirectConnection);

        connect(window,
                &QQuickWindow::sceneGraphInvalidated,
                m_sceneProviderRenderer,
                &SceneProviderRenderer::cleanup,
                Qt::DirectConnection);
    });
}
