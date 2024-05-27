#include "deivicelistviewmodel.h"
#include "src/cpp/adb/adbinterface.h"

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

    const DeviceBaceInfo &info = m_deviceInfos[index.row()];
    switch (role) {
    case NameRole:
        return info.deviceName;
    case ipRole:
        return info.ip;
    case batteryRole:
        return info.battery;
    case isConnectedRole:
        return info.isConnected;
    case isWirelessRole:
        return info.isWireless;
    case isChargingRole:
        return info.isCharging;
    case deviceCodeRole:
        return info.deviceCode;
    default:
        return QVariant();
    }
}

void DeviceListviewModel::appendRow(DeviceBaceInfo info)
{
    if (info.deviceCode.isEmpty()) return;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_deviceInfos.append(info);
    endInsertRows();
}

void DeviceListviewModel::removeRow(const QString &code)
{
    if (code.isEmpty()) return;
    for (int i = 0; i < m_deviceInfos.size(); i++) {
        if (m_deviceInfos[i].deviceCode == code) {
            beginRemoveRows(QModelIndex(), i, i);
            m_deviceInfos.remove(i);
            endRemoveRows();
        }
    }
}

void DeviceListviewModel::setInfo(const DeviceBaceInfo &info)
{
    int _index = 0;
    for (auto &it : m_deviceInfos) {
        if (it.deviceCode == info.deviceCode) {
            it.deviceName = info.deviceName;
            it.battery = info.battery;
            it.ip = info.ip;
            it.isCharging = info.isCharging;
            it.isConnected = info.isConnected;
            it.isWireless = info.isWireless;
            emit dataChanged(index(_index, 0), index(_index, 0));
            break;
        }
        _index++;
    }
}

QModelIndex DeviceListviewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    // 在这里根据行号和列号创建索引
    return createIndex(row, column);
}

bool DeviceListviewModel::hasDeviceCode(const QString &code)
{
    for (const auto &info : m_deviceInfos) {
        if (info.deviceCode == code) {
            return true;
        }
    }
    return false;
}

void DeviceListviewModel::setCurrentIndex(int index)
{
    emit currentItemChanged(m_deviceInfos.value(index).deviceCode);
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
    roles[isChargingRole] = "isCharging";
    roles[deviceCodeRole] = "deviceCode";
    return roles;
}

void DeviceListviewModel::requestDisConnect(const QString &deviceCode)
{
    for (auto &it : m_deviceInfos) {
        if (it.deviceCode == deviceCode) {
            ADBInterface::instance()->disConnectDeivce(deviceCode);
            return;
        }
    }
}