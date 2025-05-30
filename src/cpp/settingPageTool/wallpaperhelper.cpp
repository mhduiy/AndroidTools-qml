#include "wallpaperhelper.h"
#include <QDebug>
#include "../utils/globalsetting.h"
#include <QFileDialog>
#include <QApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "../utils/constants.h"
#include <QUrl>

WallpaperHelper::WallpaperHelper(QObject *parent)
    : QObject(parent)
    , m_triggerTimer(new QTimer(this))
{
    GlobalSetting::instance()->checkConfig("wallpaper", "url", "qrc:/res/backgroundImage.jpeg");
    GlobalSetting::instance()->checkConfig("wallpaper", "opacity", DEFAULT_WALLPAPER_OPACITY);
    GlobalSetting::instance()->checkConfig("wallpaper", "blurRadius", DEFAULT_WALLPAPER_BLUR_RADIUS);
    // 获取壁纸配置
    QString cutWallpaperUrl = GlobalSetting::instance()->readConfig("wallpaper", "url").toString();
    if (cutWallpaperUrl.isEmpty()) {
        setWallPaper("qrc:/res/backgroundImage.jpeg");
    } else {
        setWallPaper(cutWallpaperUrl);
    }

    qreal wallpaperOpacity =  GlobalSetting::instance()->readConfig("wallpaper", "opacity", DEFAULT_WALLPAPER_OPACITY).toReal();
    setOpacity(wallpaperOpacity);
    int wallpaperBlurRadius = GlobalSetting::instance()->readConfig("wallpaper", "blurRadius", DEFAULT_WALLPAPER_BLUR_RADIUS).toInt();
    setBlurRadius(wallpaperBlurRadius);

    connect(m_triggerTimer, &QTimer::timeout, this, &WallpaperHelper::writeValueToConfig);
    m_triggerTimer->setSingleShot(true);
}

void WallpaperHelper::setWallPaper(const QString &url)
{
    m_wallpaperUrl = url;
    emit wallpaperChanged(url);
}

QString WallpaperHelper::getWallpaper() const
{
    return m_wallpaperUrl;
}

void WallpaperHelper::setOpacity(const qreal &value)
{
    m_opacity = value;
    emit opacityChanged(value);
    m_triggerTimer->start(100);
}

qreal WallpaperHelper::getOpacity() const
{
    return m_opacity;
}

void WallpaperHelper::setBlurRadius(const int &value)
{
    m_blurRadius = value;
    emit blurRadiusChanged(value);
    m_triggerTimer->start(100);
}

int WallpaperHelper::getBlurRadius() const
{
    return m_blurRadius;
}

void WallpaperHelper::writeValueToConfig()
{
    GlobalSetting::instance()->writeConfig("wallpaper", "opacity", m_opacity);
    GlobalSetting::instance()->writeConfig("wallpaper", "blurRadius", m_blurRadius);
}

void WallpaperHelper::requestAddCustomWallpaper()
{
    QStringList filePaths = QFileDialog::getOpenFileNames();
    QFile cacheFile(WALLPAPERCACHEJSONPATH);
    qWarning() << WALLPAPERCACHEJSONPATH;
    if (!cacheFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }
    QJsonDocument cacheDoc = QJsonDocument::fromJson(cacheFile.readAll());
    QJsonArray cacheArray = cacheDoc.array();

    for (const QString &filePath : filePaths) {
        QJsonObject wallpaperJson;
        wallpaperJson.insert("url", QUrl::fromLocalFile(filePath).toString());
        cacheArray.append(wallpaperJson);
    }
    QJsonDocument tempDoc(cacheArray);
    cacheFile.seek(0);
    cacheFile.write(tempDoc.toJson());
    cacheFile.close();

    emit requestRefreshWallpaperList();
}
