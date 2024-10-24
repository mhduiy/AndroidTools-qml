#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QQmlContext>
#include <QThread>
#include <QQuickItem>
#include "cpp/adb/connectmanager.h"
#include "cpp/infoPageTool/infopagetool.h"
#include "cpp/controlPageTool/controlPageTool.h"
#include "cpp/appPageTools/appPagetool.h"
#include "cpp/flashPageTools/flashPageTool.h"
#include "cpp/imagePageTool/imagePageTool.h"
#include "cpp/settingPageTools/settingPageTools.h"
#include "cpp/utils/notificationcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef Q_OS_LINUX
    qunsetenv("http_proxy");
    qunsetenv("https_proxy");
#endif

    ADBTools::instance(&app);
    ADBInterface *interface = ADBInterface::instance(&app);
    interface->startADBService();

    // 连接管理放置到子线程
    QThread *thread = new QThread(&app);
    ConnectManager::instance()->moveToThread(thread);
    thread->start();
    // 开始检测设备连接
    ConnectManager::instance()->startCheckDevice();

    // 加载InfoPage的相关逻辑
    InfoPageTool::instance(&app);
    // 加载ControlPage相关逻辑
    ControlPageTool::instance(&app);
    // 加载AppPage的相关逻辑
    AppPageTool::instance(&app);
    // 加载FlashPage相关逻辑
    FlashPageTool::instance(&app);
    // 加载ImagePage相关逻辑
    ImagePageTool::instance(&app);
    // 加载SettingPage的相关逻辑
    SettingPageTools::instance(&app);

    // 注册通知控制管理
    NotificationController::instance(&app);
    qmlRegisterSingletonInstance("NotificationController", 1, 0, "NotificationController", NotificationController::instance());

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/qml/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    QObject::connect(qApp, &QApplication::aboutToQuit, qApp, [thread]() {
        ConnectManager::instance()->stopCheckDevice();
        qInfo() << "等待设备管理线程结束";
        thread->quit();
        thread->wait();
        qInfo() << "设备管理线程结束";
    });

    return app.exec();
}
