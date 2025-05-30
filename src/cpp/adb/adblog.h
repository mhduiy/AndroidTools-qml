#pragma once

#include <QAbstractListModel>
#include <QMutex>

#include "src/cpp/utils/defutils.hpp"
enum ADBLogType {
    Info = 0,
    Error
};

enum ADBLogRole {
    ADBLogTypeRole = Qt::UserRole + 1,
    ADBLogInfoRole
};

struct ADBLog {
    ADBLogType type;
    QString log;
};

class ADBLogModel: public QAbstractListModel
{
    Q_OBJECT
    SINGLETON(ADBLogModel)
public:
    Q_ENUM(ADBLogType);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column = 1, const QModelIndex &parent = QModelIndex()) const override;
    void commitLog(ADBLogType type, const QString &logMeg);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    void arrangeLog();

private:
    QList<ADBLog *> m_logInfo;
    QMutex m_commitMutex;
};
