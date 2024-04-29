#include "settingPageTools.h"
#include <QtQml>
#include <QtConcurrent/QtConcurrentRun>
#include <QNetworkAccessManager>
#include <QThread>
#include "src/cpp/utils/Notification.h"
#include "wallpaperhelper.h"
#include "../utils/globalsetting.h"

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
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_wallPaperInfo.append(info);
    endInsertRows();
}

void WallPaperModel::removeRow(const QString &code)
{
    return;
}

void WallPaperModel::setInfo(const WallPaperInfo &info)
{
    int _index = 0;
    for (auto &it : m_wallPaperInfo) {
        if (it.url == info.url) {
            it.isLoading = info.isLoading;
            it.title = info.title;
            emit dataChanged(index(_index, 0), index(_index, 0));
            break;
        }
        _index++;
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
    const QString url = m_wallPaperInfo.value(index).url;
    if (url.isEmpty()) {
        NotificationControl::instance()->send("设置失败了", NotificationControl::Error);
    }
    emit currentItemChanged(url);
    WallpaperHelper::instance()->setWallPaper(url);
    GlobalSetting::instance()->writeConfig("wallpaper", "url", url);
    GlobalSetting::instance()->writeConfig("wallpaper", "index", QString::number(index));
}

QHash<int, QByteArray> WallPaperModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[TitleRole] = "title";
    roles[LoadingRole] = "isloading";
    return roles;
}

SettingPageTools::SettingPageTools(QObject *parent)
    : QObject(parent)
    , m_wallpaperModel(new WallPaperModel)
    , m_bingWallpaperHander(new BingWallPaperHander)
{
    qmlRegisterSingletonInstance("WallPaperModel", 1, 0, "WallPaperModel", m_wallpaperModel);
    qmlRegisterSingletonInstance("WallpaperHelper", 1, 0, "WallpaperHelper", WallpaperHelper::instance(this));

    m_wallpaperModel->appendRow(WallPaperInfo("qrc:/res/backgroundImage.jpeg", "默认壁纸", false));
    // 设置默认壁纸

    // TODO 临时方案
    QString cutWallpaperUrl = GlobalSetting::instance()->readConfig("wallpaper", "url");
    WallpaperHelper::instance()->setWallPaper(cutWallpaperUrl);
//    GlobalSetting::instance()->readConfig("wallpaper", "index");
//    m_wallpaperModel->setCurrentIndex(0);

    connect(m_bingWallpaperHander, &BingWallPaperHander::workFinish, this, &SettingPageTools::onBingWallPaperWorkFinish);
    m_bingWallpaperHander->doWork();
}

SettingPageTools::~SettingPageTools()
{

}

void SettingPageTools::onBingWallPaperWorkFinish(const QString &url)
{
    if (url.isEmpty()) {
        qWarning() << "bing wallpaper url is empty, skiping ...";
        return;
    }
    m_wallpaperModel->appendRow(WallPaperInfo(url, "Bing 每日壁纸", false));
}
