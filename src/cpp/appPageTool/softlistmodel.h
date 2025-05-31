#ifndef SOFTLISTMODEL_H
#define SOFTLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "../adb/adbdevice.h"

namespace ADT {

enum SoftListModelRole {
    AppNameRole = Qt::UserRole + 1,
    AppStateRole,
    AppVersionCodeRole,
};

class SoftListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SoftListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void appendRow(AppListInfo info);
    Q_INVOKABLE void removeRow(const QString &packageName);
    Q_INVOKABLE void setInfo(const AppListInfo &info);
    QModelIndex index(int row, int column = 1, const QModelIndex &parent = QModelIndex()) const override;
    bool hasPackage(const QString &packageName);
    void clearData();

    Q_INVOKABLE void setCurrentIndex(int index);

signals:
    void currentItemChanged(const QString &code);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<AppListInfo> m_appListInfo;
};

} // namespace ADT

#endif // SOFTLISTMODEL_H
