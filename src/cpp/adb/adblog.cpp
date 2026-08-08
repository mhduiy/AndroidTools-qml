#include "adblog.h"
#include <QDateTime>
#include <QThread>

static const int MAX_COUNT = 300;

ADBLogModel::ADBLogModel(QObject *parent)
: QAbstractListModel(parent)
{

}

int ADBLogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_logInfo.size();
}

QVariant ADBLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_logInfo.size())
        return QVariant();

    const ADBLog &info = m_logInfo[index.row()];
    switch (role) {
    case ADBLogTypeRole:
        return info.type;
    case ADBLogInfoRole:
        return info.log;
    default:
        return QVariant();
    }
}

QModelIndex ADBLogModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}

QHash<int, QByteArray> ADBLogModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ADBLogTypeRole] = "type";
    roles[ADBLogInfoRole] = "log";
    return roles;
}

void ADBLogModel::commitLog(ADBLogType type, const QString &logMeg)
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, [this, type, logMeg]() { commitLog(type, logMeg); }, Qt::QueuedConnection);
        return;
    }

    if (m_logInfo.size() == MAX_COUNT) {
        beginRemoveRows(QModelIndex(), 0, 0);
        m_logInfo.removeFirst();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), m_logInfo.size(), m_logInfo.size());
    const QString formatLog = QString("[%1] %2").arg(QDateTime::currentDateTime().toString("HH:mm:ss"), logMeg.trimmed());
    m_logInfo.append({.type = type, .log = formatLog});
    endInsertRows();
}

void ADBLogModel::clear()
{
    if (m_logInfo.isEmpty()) {
        return;
    }
    beginResetModel();
    m_logInfo.clear();
    endResetModel();
}
