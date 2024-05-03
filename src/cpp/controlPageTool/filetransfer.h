#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include "../utils/singleton.hpp"

class FileTransfer : public QObject
{
    Q_OBJECT
    SINGLETON(FileTransfer)
public:
    Q_INVOKABLE void transmission(const QString &source, const QString &targetDir);
};

#endif // FILETRANSFER_H
