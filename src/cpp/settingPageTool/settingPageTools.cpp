#include "settingPageTools.h"
#include <QtQml>
#include "wallpaperhelper.h"
#include "../utils/globalsetting.h"
#include "../utils/constants.h"
#include "othersettingshandler.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace ADT {

static auto constexpr Wallpaper_Section = "wallpaper";
static auto constexpr Url_Key = "url";
static auto constexpr Index_key = "index";

WallPaperModel::WallPaperModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int WallPaperModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_wallPaperInfo.size();
}

QVariant WallPaperModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_wallPaperInfo.size())
        return QVariant();

    const WallPaperInfo &info = m_wallPaperInfo[index.row()];
    switch (role) {
    case UrlRole:
        return info.url;
    case TitleRole:
        return info.title;
    case LoadingRole:
        return info.isLoading;
    default:
        return QVariant();
    }
}

void WallPaperModel::appendRow(WallPaperInfo info)
{
    const int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_wallPaperInfo.append(info);
    endInsertRows();
    if (info.url == WallpaperHelper::instance()->getWallpaper()) {
        m_currentIndex = row;
        emit currentIndexChanged(row);
    }
}

QModelIndex WallPaperModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}

void WallPaperModel::setCurrentIndex(int index)
{
    if (index < 0 || index >= m_wallPaperInfo.size() || index == m_currentIndex) {
        return;
    }
    const QString url = m_wallPaperInfo[index].url;
    m_currentIndex = index;
    emit currentItemChanged(url);
    emit currentIndexChanged(index);
    WallpaperHelper::instance()->setWallPaper(url);
    AppSettings->writeConfig(Wallpaper_Section, Url_Key, url);
    AppSettings->writeConfig(Wallpaper_Section, Index_key, index);
}

QHash<int, QByteArray> WallPaperModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[TitleRole] = "title";
    roles[LoadingRole] = "isloading";
    return roles;
}

void WallPaperModel::clearData()
{
    beginResetModel();
    m_wallPaperInfo.clear();
    m_currentIndex = -1;
    endResetModel();
    emit currentIndexChanged(-1);
}

SettingPageTools::SettingPageTools(QObject *parent)
    : QObject(parent)
    , m_wallpaperModel(new WallPaperModel)
{
    qmlRegisterSingletonInstance("WallPaperModel", 1, 0, "WallPaperModel", m_wallpaperModel);
    qmlRegisterSingletonInstance("WallpaperHelper", 1, 0, "WallpaperHelper", WallpaperHelper::instance(this));
    qmlRegisterSingletonInstance("OtherSettingsHandler", 1, 0, "OtherSettingsHandler", OtherSettingsHandler::instance(this));

    connect(WallpaperHelper::instance(), &WallpaperHelper::requestRefreshWallpaperList, this, &SettingPageTools::onRequestRefreshWallpaperList);
    onRequestRefreshWallpaperList();
}

SettingPageTools::~SettingPageTools()
{

}

void SettingPageTools::onRequestRefreshWallpaperList()
{
    m_wallpaperModel->clearData();
    m_wallpaperModel->appendRow(WallPaperInfo("qrc:/res/backgroundImage.jpeg", "默认壁纸", false));

    QFile cacheFile(WALLPAPERCACHEJSONPATH);
    if (!cacheFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(cacheFile.readAll());
    if (doc.isNull()) {
        return;
    }
    QJsonArray array = doc.array();
    for (auto it : array) {
        QJsonObject imageObj = it.toObject();
        QString fileNameUrl = imageObj["url"].toString();
        QUrl url = QUrl(fileNameUrl);
        if (!QFile::exists(url.toLocalFile())) {
            continue;
        }
        m_wallpaperModel->appendRow(WallPaperInfo(url.toString(), "自定义壁纸", true));
    }
    cacheFile.close();
}

} // namespace ADT