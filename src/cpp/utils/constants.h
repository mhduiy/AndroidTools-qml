#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QHash>
#include <QDir>
#include <QApplication>
#include <QStandardPaths>

const QString WALLPAPERCACHEJSONPATH = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "AndroidTools" + QDir::separator() + "wallpaperCache.json";
class constants
{
public:
    constants();
};

#endif // CONSTANTS_H
