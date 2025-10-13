#include "softlistmodel.h"

namespace ADT {

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

    const AppDetailInfo &info = m_appListInfo[index.row()];
    switch (role) {
    case AppPackageRole:
        return info.packageName;
    case AppNameRole:
        return info.appName;
    case AppVersionRole:
        return info.versionName;
    case AppVersionCodeRole:
        return info.versionCode;
    case AppIsSystemRole:
        return info.isSystemApp;
    case AppIsEnabledRole:
        return info.isEnabled;
    case AppFirstInstallTimeRole:
        return info.firstInstallTime;
    case AppLastUpdateTimeRole:
        return info.lastUpdateTime;
    case AppTargetSdkRole:
        return info.targetsdk;
    case AppMinSdkRole:
        return info.minsdk;
    case AppIdRole:
        return info.appid;
    case AppPathRole:
        return info.path;
    case AppIconRole:
        return info.iconBase64;

    default:
        return QVariant();
    }
}

void SoftListModel::appendRow(AppDetailInfo info)
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

void SoftListModel::setIcon(const QString &packageName, const QString &iconBase64)
{
    if (packageName.isEmpty()) return;
    for (int i = 0; i < m_appListInfo.size(); i++) {
        if (m_appListInfo[i].packageName == packageName) {
            m_appListInfo[i].iconBase64 = iconBase64;
            emit dataChanged(index(i, 0), index(i, 0));
            break;
        }
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
    roles[AppPackageRole] = "packageName";
    roles[AppNameRole] = "appName";
    roles[AppVersionRole] = "versionName";
    roles[AppVersionCodeRole] = "versionCode";
    roles[AppIsSystemRole] = "isSystemApp";
    roles[AppIsEnabledRole] = "isEnabled";
    roles[AppFirstInstallTimeRole] = "firstInstallTime";
    roles[AppLastUpdateTimeRole] = "lastUpdateTime";
    roles[AppTargetSdkRole] = "targetSdk";
    roles[AppMinSdkRole] = "minSdk";
    roles[AppIdRole] = "appId";
    roles[AppPathRole] = "path";
    roles[AppIconRole] = "icon";
    return roles;
}

} // namespace ADT