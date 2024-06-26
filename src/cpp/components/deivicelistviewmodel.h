#ifndef DEIVICELISTVIEWMODEL_H
#define DEIVICELISTVIEWMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QList>
#include <qtmetamacros.h>
#include "../adb/adbinterface.h"


enum DeviceBaceRole {
    NameRole = Qt::UserRole + 1,
    ipRole,
    batteryRole,
    isConnectedRole,
    isWirelessRole,
    isChargingRole,

    deviceCodeRole
};

class DeviceListviewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DeviceListviewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void appendRow(DeviceBaceInfo info);
    Q_INVOKABLE void removeRow(const QString &code);
    Q_INVOKABLE void setInfo(const DeviceBaceInfo &info);
    QModelIndex index(int row, int column = 1, const QModelIndex &parent = QModelIndex()) const override;
    bool hasDeviceCode(const QString &code);

    Q_INVOKABLE void setCurrentIndex(int index);
    Q_INVOKABLE void requestDisConnect(const QString &deviceCode);

signals:
    void currentItemChanged(const QString &code);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<DeviceBaceInfo> m_deviceInfos;
};

#endif // DEIVICELISTVIEWMODEL_H
