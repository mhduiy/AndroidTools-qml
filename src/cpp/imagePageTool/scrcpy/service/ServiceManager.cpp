#include "ServiceManager.h"

ResourceService* ServiceManager::resourceService() const {
    return m_resourceService;
}

void ServiceManager::setResourceService(ResourceService* resourceService) {
    m_resourceService = resourceService;
}
