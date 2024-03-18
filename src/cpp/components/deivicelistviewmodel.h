#ifndef DEIVICELISTVIEWMODEL_H
#define DEIVICELISTVIEWMODEL_H

#include <QAbstractListModel>
#include "../adb/adbinterface.h"


enum DeviceBaceRole {
    NameRole = Qt::UserRole + 1,
    ipRole,
    batteryRole,
    isConnectedRole,
    isWirelessRole,
    isChargingRole
};

class DeviceListviewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DeviceListviewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void appendRow(DeviceBaceInfo *info);
    Q_INVOKABLE void insertRow(int row, DeviceBaceInfo *info);
    Q_INVOKABLE void removeRow(int row);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<DeviceBaceInfo *> m_deviceInfos;
};

#endif // DEIVICELISTVIEWMODEL_H
