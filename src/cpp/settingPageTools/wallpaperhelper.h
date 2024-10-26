#ifndef WALLPAPERHELPER_H
#define WALLPAPERHELPER_H

#include <QObject>
#include "../utils/singleton.hpp"
#include <QTimer>

class WallpaperHelper : public QObject
{
    Q_OBJECT
    SINGLETON(WallpaperHelper)
    Q_PROPERTY(QString wallpaper READ getWallpaper WRITE setWallPaper NOTIFY wallpaperChanged)
    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(int blurRadius READ getBlurRadius WRITE setBlurRadius NOTIFY blurRadiusChanged)

public:
    void setWallPaper(const QString &url);
    QString getWallpaper() const;

    void setOpacity(const qreal &value);
    qreal getOpacity() const;

    void setBlurRadius(const int &value);
    int getBlurRadius() const;

    Q_INVOKABLE void requestAddCustomWallpaper();

private:
    void writeValueToConfig();

signals:
    void wallpaperChanged(const QString &url);
    void opacityChanged(const qreal &value);
    void blurRadiusChanged(const int value);
    void requestRefreshWallpaperList();

private:
    QString m_wallpaperUrl;
    qreal m_opacity;
    int m_blurRadius;

    QTimer *m_triggerTimer;
};

#endif // WALLPAPERHELPER_H
