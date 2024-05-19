#include "ServiceManager.h"

#include <QThread>

ResourceService* ServiceManager::resourceService() const {
    return m_resourceService;
}

SceneProviderRenderer* ServiceManager::renderer() const {
    return m_renderers.value(QThread::currentThread(), nullptr);
}

void ServiceManager::setResourceService(ResourceService* resourceService) {
    m_resourceService = resourceService;
}

void ServiceManager::addRenderer(SceneProviderRenderer* renderer) {
    m_renderers.insert(QThread::currentThread(), renderer);
}
