#pragma once

#include <QObject>
#include <qobject.h>
#include <qtmetamacros.h>
#include "../utils/singleton.hpp"

#include "service/ResourceService.h"
#include "service/udp/Broadcast.h"

class ImagePageTool : public QObject
{
    Q_OBJECT
    SINGLETON(ImagePageTool)
public:


private:
    ResourceService *resourceService = nullptr;
    Broadcast *m_broadcast = nullptr;
};