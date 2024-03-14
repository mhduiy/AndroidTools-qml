#include "deivicelistviewmodel.h"

DeviceListviewModel::DeviceListviewModel(QObject *parent)
: QAbstractListModel(parent)
{

}

int DeviceListviewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_deviceInfos.size();
}

QVariant DeviceListviewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_deviceInfos.size())
        return QVariant();

    DeviceBaceInfo *info = m_deviceInfos[index.row()];
    switch (role) {
    case NameRole:
        return info->deviceName;
    case ipRole:
        return info->ip;
    case batteryRole:
        return info->battery;
    case isConnectedRole:
        return info->isConnected;
    case isWirelessRole:
        return info->isWireless;
    case isChargingRole:
        return info->isCharging;
    default:
        return QVariant();
    }
}

void DeviceListviewModel::appendRow(DeviceBaceInfo *info)
{
    if (info) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_deviceInfos.append(info);
        endInsertRows();

    }
}

void DeviceListviewModel::insertRow(int row, DeviceBaceInfo *info)
{
    if (info && row <= m_deviceInfos.size()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_deviceInfos.insert(row, info);
        endInsertRows();
    }
}

void DeviceListviewModel::removeRow(int row)
{
    if (row >= 0 && row < m_deviceInfos.size()) {
        beginRemoveRows(QModelIndex(), row, row);
        m_deviceInfos.remove(row);
        endRemoveRows();
    }
}

QHash<int, QByteArray> DeviceListviewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "deviceName";
    roles[ipRole] = "ip";
    roles[batteryRole] = "battery";
    roles[isConnectedRole] = "isConnected";
    roles[isWirelessRole] = "isWireless";
    roles[isChargingRole] = "isCharging";
    return roles;
}
