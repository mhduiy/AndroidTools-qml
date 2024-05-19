#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <QObject>

#include "adbprocess.h"

class FileHandler : public QObject
{
    Q_OBJECT
public:
    enum FILE_HANDLER_RESULT
    {
        FAR_IS_RUNNING,   // executing
        FAR_SUCCESS_EXEC, // execution succeed
        FAR_ERROR_EXEC,   // Execution failed
    };

    FileHandler(QObject *parent = nullptr);
    virtual ~FileHandler();

    const QString &getDevicePath();

public slots:
    void onPushFileRequest(const QString &serial, const QString &file, const QString &devicePath = "");
    void onInstallApkRequest(const QString &serial, const QString &apkFile);

protected:
    void onAdbProcessResult(qsc::AdbProcess* adb, bool isApk, qsc::AdbProcess::ADB_EXEC_RESULT processResult);

signals:
    void fileHandlerResult(FILE_HANDLER_RESULT processResult, bool isApk = false);
};

#endif // FILEHANDLER_H
