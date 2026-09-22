#ifndef SOFTLISTMODEL_H
#define SOFTLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "../adb/adbtypes.h"

namespace ADT {

enum SoftListModelRole {
    AppPackageRole = Qt::UserRole + 1,
    AppNameRole,
    AppVersionRole,
    AppVersionCodeRole,
    AppIsSystemRole,
    AppIsEnabledRole,
    AppFirstInstallTimeRole,
    AppLastUpdateTimeRole,
    AppTargetSdkRole,
    AppMinSdkRole,
    AppIdRole,
    AppPathRole,
    AppIconRole
};

class SoftListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SoftListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void appendRow(AppDetailInfo info);
    Q_INVOKABLE void setIcon(const QString &packageName, const QString &iconBase64);
    void clearData();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<AppDetailInfo> m_appListInfo;
};

} // namespace ADT

#endif // SOFTLISTMODEL_H
