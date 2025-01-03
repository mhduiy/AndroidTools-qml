#pragma once

#include <QObject>
#include <qobject.h>
#include <qtmetamacros.h>
#include "../utils/singleton.hpp"

#include "service/ResourceService.h"

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
public:


private:
    ResourceService *resourceService = nullptr;
};