#pragma once

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

    ResourceService* resourceService() const;
    void setResourceService(ResourceService* resourceService);

private:
    ServiceManager()                                  = default;
    ~ServiceManager()                                 = default;
    ServiceManager(const ServiceManager&)             = delete;
    ServiceManager& operator=(const ServiceManager&)  = delete;
    ServiceManager(const ServiceManager&&)            = delete;
    ServiceManager& operator=(const ServiceManager&&) = delete;

private:
    ResourceService* m_resourceService = nullptr;
};
