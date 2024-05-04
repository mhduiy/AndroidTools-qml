#include "softlistmodel.h"

SoftListModel::SoftListModel(QObject *parent) : QAbstractListModel(parent)
{

}

int SoftListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_appListInfo.size();
}

QVariant SoftListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_appListInfo.size())
        return QVariant();

    const AppListInfo &info = m_appListInfo[index.row()];
    switch (role) {
    case AppNameRole:
        return info.packageName;
    case AppStateRole:
        return info.state;
    case AppVersionCodeRole:
        return info.versionCode;
    default:
        return QVariant();
    }
}

void SoftListModel::appendRow(AppListInfo info)
{
    if (info.packageName.isEmpty()) return;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_appListInfo.append(info);
    endInsertRows();
}

void SoftListModel::removeRow(const QString &packageName)
{
    if (packageName.isEmpty()) return;
    for (int i = 0; i < m_appListInfo.size(); i++) {
        if (m_appListInfo[i].packageName == packageName) {
            beginRemoveRows(QModelIndex(), i, i);
            m_appListInfo.remove(i);
            endRemoveRows();
        }
    }
}

void SoftListModel::setInfo(const AppListInfo &info)
{
    int _index = 0;
    for (auto &it : m_appListInfo) {
        if (it.packageName == info.packageName) {
            it.state = info.state;
            it.versionCode = info.versionCode;
            emit dataChanged(index(_index, 0), index(_index, 0));
            break;
        }
        _index++;
    }
}

QModelIndex SoftListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    // 在这里根据行号和列号创建索引
    return createIndex(row, column);
}

bool SoftListModel::hasPackage(const QString &packageName)
{
    for (const auto &info : m_appListInfo) {
        if (info.packageName == packageName) {
            return true;
        }
    }
    return false;
}

void SoftListModel::clearData()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_appListInfo.clear();
    endRemoveRows();
}

void SoftListModel::setCurrentIndex(int index)
{
    emit currentItemChanged(m_appListInfo.value(index).packageName);
}

QHash<int, QByteArray> SoftListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AppNameRole] = "packageName";
    roles[AppStateRole] = "state";
    roles[AppVersionCodeRole] = "versionCode";
    return roles;
}
