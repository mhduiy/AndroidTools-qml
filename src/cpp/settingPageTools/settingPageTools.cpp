#include "settingPageTools.h"
#include <QtQml>

WallPaperModel::WallPaperModel(QObject *parent)
    : QAbstractListModel(parent)
{
    appendRow(WallPaperInfo("qrc:/res/backgroundImage.jpeg", "默认壁纸", false));
    appendRow(WallPaperInfo("https://cn.bing.com/th?id=OHR.GuadalupeTexas_ZH-CN3911419948_UHD.jpg&rf=LaDigue_UHD.jpg&pid=hp&w=3840&h=2160&rs=1&c=4", "Bing 每日壁纸", false));
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
    return;
}

QHash<int, QByteArray> WallPaperModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[TitleRole] = "title";
    roles[LoadingRole] = "isloading";
    return roles;
}

SettingPageTools::SettingPageTools(QObject *parent) : QObject(parent)
{
    m_wallpaperModel = new WallPaperModel();
    qmlRegisterSingletonInstance("WallPaperModel", 1, 0, "WallPaperModel", m_wallpaperModel);
}
