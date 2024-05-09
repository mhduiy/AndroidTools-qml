#ifndef FLASHPAGETOOL_H
#define FLASHPAGETOOL_H

#include <QObject>
#include <QThread>
#include "fastbootdevicemanager.h"
#include "../utils/singleton.hpp"
#include <QAbstractListModel>

enum FlashLinkRole {
    FlashUrlRole = Qt::UserRole + 1,
    FlashTitleRole,
    FlashDescriptionRole,
};

struct FlashLinKInfo
{
    QString url;
    QString title;
    QString description;

    FlashLinKInfo(const QString &_url, const QString &_title, const QString &_description)
    : url(_url)
    , title(_title)
    , description(_description)
    {

    }
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
};

#endif
