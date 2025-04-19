#include "adblog.h"
#include <QMutexLocker>
#include <QDateTime>

static const int MAX_COUNT = 100;

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

    const ADBLog *info = m_logInfo[index.row()];
    switch (role) {
    case ADBLogTypeRole:
        return info->type;
    case ADBLogInfoRole:
        return info->log;
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
    QMutexLocker locker(&m_commitMutex);
    beginInsertRows(QModelIndex(), m_logInfo.size(), m_logInfo.size());
    const QString formatLog = QString("[%1][%2]: %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(type).arg(logMeg);
    m_logInfo.append(new ADBLog{.type = type, .log = formatLog});
    endInsertRows();
    qDebug() << QString("adblog[%1] ").arg(type) << logMeg; 
    arrangeLog();
}

void ADBLogModel::arrangeLog()
{
    if (int deleteCount = m_logInfo.size() - MAX_COUNT; deleteCount > 0) {
        for (int i = 0; i < deleteCount; i++) {
            beginRemoveRows(QModelIndex(), 0, 0);
            auto first = m_logInfo.takeFirst();
            endRemoveRows();
            delete first;
        }
    }
}
