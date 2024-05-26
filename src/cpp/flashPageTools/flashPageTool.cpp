#include "flashPageTool.h"
#include "src/cpp/flashPageTools/flashtools.h"
#include <QtQml>

FlashPageTool::FlashPageTool(QObject *parent)
    : QObject(parent)
    , m_fastBootDeviceManager(FastBootDeviceManager::instance(this))
    , m_flashLinkModel(new FlashLinkModel(this))
    , m_flashUrlHandle(new FlashUrlHandle(this))
{
    qmlRegisterSingletonInstance("FastBootDeviceManager", 1, 0, "FastBootDeviceManager", m_fastBootDeviceManager);
    qmlRegisterSingletonInstance("FlashLinkModel", 1, 0, "FlashLinkModel", m_flashLinkModel);
    qmlRegisterSingletonInstance("FlashTools", 1, 0, "FlashTools", FlashTools::instance());

    connect(m_flashUrlHandle, &FlashUrlHandle::workFinish, this, [this](){
        auto infos = m_flashUrlHandle->getInfo();
        m_flashLinkModel->clearData();
        for (auto &info : infos) {
            m_flashLinkModel->appendRow(info);
        }
    });
    m_flashUrlHandle->doWork();
}

FlashLinkModel::FlashLinkModel(QObject *parent)
    : QAbstractListModel(parent)
{
    
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

void FlashLinkModel::clearData()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_deviceInfos.clear();
    endRemoveRows();
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
