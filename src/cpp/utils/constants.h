#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QHash>
#include <QDir>
#include <QApplication>
#include <QStandardPaths>

const static QString WALLPAPERCACHEJSONPATH = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "AndroidTools" + QDir::separator() + "wallpaperCache.json";

// default values
const static double DEFAULT_WALLPAPER_OPACITY = 0.6;
const static QString DEFAULT_WALLPAPER = "";
const static int DEFAULT_WALLPAPER_BLUR_RADIUS = 50;
const static bool DEFAULT_USE_OPENGL = false; 
const static double DEFAULT_WRAPPER_OPACITY = 0.4;
const static int DEFAULT_APP_THEME_TYPE = 0;
const static int DEFAULT_DEVICE_REFRESH_INTERVAL = 5000;

class constants
{
public:
    constants();
};

#endif // CONSTANTS_H
