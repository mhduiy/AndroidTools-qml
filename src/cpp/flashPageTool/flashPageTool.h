#ifndef FLASHPAGETOOL_H
#define FLASHPAGETOOL_H

#include <QObject>
#include <QThread>
#include "fastbootdevicemanager.h"
#include "../utils/defutils.hpp"
#include <QAbstractListModel>
#include "flashUrlHandle.h"

namespace ADT {

enum FlashLinkRole {
    FlashUrlRole = Qt::UserRole + 1,
    FlashTitleRole,
    FlashDescriptionRole,
};

class FlashLinkModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FlashLinkModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void appendRow(FlashLinKInfo info);
    QModelIndex index(int row, int column = 1, const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void goLink(int index);
    void clearData();

signals:
    void currentItemChanged(const QString &code);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<FlashLinKInfo> m_deviceInfos;
};


class FlashPageTool : public QObject
{
    Q_OBJECT
    SINGLETON(FlashPageTool)
public:

private:
    QThread *m_deviceCheckThread;
    FastBootDeviceManager *m_fastBootDeviceManager;
    FlashLinkModel *m_flashLinkModel;
    FlashUrlHandle *m_flashUrlHandle;
};

} // namespace ADT

#endif
