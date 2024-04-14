#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QQmlContext>
#include <QThread>
#include "cpp/adb/connectmanager.h"
#include "cpp/infoPageTool/infopagetool.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ADBTools::instance(&app);
    ADBInterface *interface = ADBInterface::instance(&app);

    // 连接管理放置到子线程
    QThread *thread = new QThread();
    ConnectManager::instance(&app)->moveToThread(thread);
    thread->start();
    // 开始检测设备连接
    ConnectManager::instance()->startCheckDevice();

    // 加载InfoPage的相关逻辑
    InfoPageTool::instance(&app);

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/qml/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    qWarning() << engine.rootObjects();

    return app.exec();
}
