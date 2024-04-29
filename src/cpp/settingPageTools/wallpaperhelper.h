#ifndef WALLPAPERHELPER_H
#define WALLPAPERHELPER_H

#include <QObject>
#include "../utils/singleton.hpp"

class WallpaperHelper : public QObject
{
    Q_OBJECT
    SINGLETON(WallpaperHelper)
    Q_PROPERTY(QString wallpaper READ getWallpaper WRITE setWallPaper NOTIFY wallpaperChanged)

public:
    void setWallPaper(const QString &url);
    QString getWallpaper() const;

signals:
    void wallpaperChanged(const QString &url);

private:
    QString m_wallpaperUrl;
};

#endif // WALLPAPERHELPER_H
