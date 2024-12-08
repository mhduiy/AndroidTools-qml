#pragma once

#include "SceneProviderRenderer.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QString>

//!
//! SceneProvider will act as a BackgroundRenderer on the MainThread, its child SceneProviderRenderer will do the actual
//! rendering on RenderThread

