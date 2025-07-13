#pragma once

#include <QObject>
#include <QAtomicInt>
#include <QMutex>

#define ADBTOOL ADBTools::instance()

namespace ADT {

// 前向声明 ADBCommandResult
struct CommandResult;

class ADBTools : public QObject
{
    Q_OBJECT
public:
    enum APP{
        ADB,
        FASTBOOT
    };

    QString executeCommand(APP app = ADB, const QStringList &args = QStringList(), const QString &writeStr = "", const int timeout = 3000);
    
    // 新增方法，返回详细执行结果
    CommandResult executeCommandDetailed(APP app = ADB, const QStringList &args = QStringList(), const QString &writeStr = "", const int timeout = 3000);
    
    bool isRunning();

    static ADBTools *instance(QObject *parent = nullptr);

    bool startService();
    bool killService();
private:
    explicit ADBTools(QObject *parent = nullptr);
    ADBTools() = delete;
    QMutex *m_mutex;
    QAtomicInt m_isRunning = 0;
};

} // namespace ADT