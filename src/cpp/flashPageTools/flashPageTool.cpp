#include "flashPageTool.h"
#include <QtQml>

FlashPageTool::FlashPageTool(QObject *parent)
    : QObject(parent)
    , m_fastBootDeviceManager(FastBootDeviceManager::instance(this))
{
    m_flashLinkModel = new FlashLinkModel(this);
    qmlRegisterSingletonInstance("FastBootDeviceManager", 1, 0, "FastBootDeviceManager", m_fastBootDeviceManager);
    qmlRegisterSingletonInstance("FlashLinkModel", 1, 0, "FlashLinkModel", m_flashLinkModel);
}

FlashLinkModel::FlashLinkModel(QObject *parent)
    : QAbstractListModel(parent)
{
    appendRow(FlashLinKInfo("https://www.baidu.com", "百度", "百度一下，你就知道"));
}

int FlashLinkModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_deviceInfos.size();
}

QVariant FlashLinkModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_deviceInfos.size())
        return QVariant();

    const FlashLinKInfo &info = m_deviceInfos[index.row()];
    switch (role) {
    case FlashUrlRole:
        return info.url;
    case FlashTitleRole:
        return info.title;
    case FlashDescriptionRole:
        return info.description;
    default:
        return QVariant();
    }
}

void FlashLinkModel::appendRow(FlashLinKInfo info)
{
    if (info.url.isEmpty()) return;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_deviceInfos.append(info);
    endInsertRows();
}

QModelIndex FlashLinkModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}

void FlashLinkModel::goLink(int index)
{
    return;
}

QHash<int, QByteArray> FlashLinkModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FlashUrlRole] = "url";
    roles[FlashTitleRole] = "title";
    roles[FlashDescriptionRole] = "description";
    return roles;
}
