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

