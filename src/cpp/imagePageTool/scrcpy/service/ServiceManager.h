#pragma once

#include "ui/sceneprovider/SceneProviderRenderer.h"

#include "ResourceService.h"

//!
//! This class acts only as a object holder accessible from the whole application
//!
class ServiceManager {

public:
    static ServiceManager& getInstance() {
        static ServiceManager instance;
        return instance;
    }

    static ServiceManager* getInstancePtr() { return &ServiceManager::getInstance(); }

    ResourceService* resourceService() const;
    SceneProviderRenderer* renderer() const;


    void addRenderer(SceneProviderRenderer* renderer);
    void setResourceService(ResourceService* resourceService);

private:
    ServiceManager()                                  = default;
    ~ServiceManager()                                 = default;
    ServiceManager(const ServiceManager&)             = delete;
    ServiceManager& operator=(const ServiceManager&)  = delete;
    ServiceManager(const ServiceManager&&)            = delete;
    ServiceManager& operator=(const ServiceManager&&) = delete;

    void generateObjects(QThread* thread);

private:
    ResourceService* m_resourceService = nullptr;
    QMap<QThread*, SceneProviderRenderer*> m_renderers;
};
