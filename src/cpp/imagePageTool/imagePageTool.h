#pragma once

#include <QObject>
#include <qobject.h>
#include <qtmetamacros.h>
#include "../utils/defutils.hpp"

#include "service/ResourceService.h"

namespace ADT {

class ImageDetailTools : public QObject
{
    Q_OBJECT
    SINGLETON(ImageDetailTools)
public:
    Q_INVOKABLE void shotScreen(const QString &outPath);

private:
    ResourceService *resourceService = nullptr;
};

class ImagePageTool : public QObject
{
    Q_OBJECT
    SINGLETON(ImagePageTool)
private:
    void checkScrcpyServer();

private:
    ResourceService *resourceService = nullptr;
};

} // namespace ADT