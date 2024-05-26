#include "wallpaperhelper.h"
#include <QDebug>
#include <qcontainerfwd.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qlogging.h>
#include <qtmetamacros.h>
#include <qtypes.h>
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
    GlobalSetting::instance()->checkConfig("wallpaper", "opacity", 0.2);
    GlobalSetting::instance()->checkConfig("wallpaper", "blurRadius", 10);
    // 获取壁纸配置
    QString cutWallpaperUrl = GlobalSetting::instance()->readConfig("wallpaper", "url").toString();
    if (cutWallpaperUrl.isEmpty()) {
        setWallPaper("qrc:/res/backgroundImage.jpeg");
    } else {
        setWallPaper(cutWallpaperUrl);
    }

    qreal wallpaperOpacity =  GlobalSetting::instance()->readConfig("wallpaper", "opacity", 0.2).toReal();
    setOpacity(wallpaperOpacity);
    int wallpaperBlurRadius = GlobalSetting::instance()->readConfig("wallpaper", "blurRadius", 10).toInt();
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
    qWarning() << "---";
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
        qWarning() << filePath;
        wallpaperJson.insert("url", QUrl::fromLocalFile(filePath).toString());
        cacheArray.append(wallpaperJson);
    }
    QJsonDocument tempDoc(cacheArray);
    cacheFile.seek(0);
    cacheFile.write(tempDoc.toJson());
    cacheFile.close();

    emit requestRefreshWallpaperList();
}
