#ifndef ADBPROCESS_H
#define ADBPROCESS_H

#include <QObject>

class AdbProcessImpl;
namespace qsc {

class AdbProcess : public QObject
{
    Q_OBJECT

public:
    enum ADB_EXEC_RESULT
    {
        AER_SUCCESS_START,        // Start successfully
        AER_ERROR_START,          // Startup failed
        AER_SUCCESS_EXEC,         // execution succeed
        AER_ERROR_EXEC,           // Execution failed
        AER_ERROR_MISSING_BINARY, // file not found
    };

    explicit AdbProcess(QObject *parent = nullptr);
    virtual ~AdbProcess();

    static void setAdbPath(const QString& adbPath);

    void execute(const QString &serial, const QStringList &args);
    void forward(const QString &serial, quint16 localPort, const QString &deviceSocketName);
    void forwardRemove(const QString &serial, quint16 localPort);
    void reverse(const QString &serial, const QString &deviceSocketName, quint16 localPort);
    void reverseRemove(const QString &serial, const QString &deviceSocketName);
    void push(const QString &serial, const QString &local, const QString &remote);
    void install(const QString &serial, const QString &local);
    void removePath(const QString &serial, const QString &path);
    bool isRuning();
    void setShowTouchesEnabled(const QString &serial, bool enabled);
    void kill();
    QStringList arguments();
    QStringList getDevicesSerialFromStdOut();
    QString getDeviceIPFromStdOut();
    QString getDeviceIPByIpFromStdOut();
    QString getStdOut();
    QString getErrorOut();

signals:
    void adbProcessResult(ADB_EXEC_RESULT processResult);

private:
    AdbProcessImpl* m_adbImpl = nullptr;
};

}
#endif // ADBPROCESS_H
