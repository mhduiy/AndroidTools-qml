#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include <qthread.h>
#include <qtmetamacros.h>
#include "../utils/defutils.hpp"

class FileTransferHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileTransferHandler(QObject *parent = nullptr);
public slots:
    void transmission(QString deviceCode, QString source, QString targetDir);

signals:
    void workFinish();
};

class FileTransfer : public QObject
{
    Q_OBJECT
    SINGLETON(FileTransfer)
public:
    ~FileTransfer();
    Q_INVOKABLE void transmission(const QString &source, const QString &targetDir);
private:
    FileTransferHandler *m_handler;
    QThread *m_handleThread;
};

#endif // FILETRANSFER_H
