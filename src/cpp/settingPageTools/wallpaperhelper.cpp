#include "wallpaperhelper.h"
#include <QDebug>

WallpaperHelper::WallpaperHelper(QObject *parent)
    :QObject(parent)
{

}

void WallpaperHelper::setWallPaper(const QString &url)
{
    qInfo() << "设置壁纸" << url;
    m_wallpaperUrl = url;
    emit wallpaperChanged(url);
}

QString WallpaperHelper::getWallpaper() const
{
    return m_wallpaperUrl;
}
