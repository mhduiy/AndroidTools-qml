#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QQmlContext>
#include <QThread>
#include <QQuickItem>
#include <QElapsedTimer>
#include <QStyleFactory>
#include <QQuickStyle>
#include <QQuickWindow>

#include "cpp/adb/connectmanager.h"
#include "cpp/infoPageTool/infopagetool.h"
#include "cpp/controlPageTool/controlPageTool.h"
#include "cpp/appPageTools/appPagetool.h"
#include "cpp/flashPageTools/flashPageTool.h"
#include "cpp/imagePageTool/imagePageTool.h"
#include "cpp/settingPageTools/settingPageTools.h"
#include "cpp/utils/notificationcontroller.h"

bool checkADB() {
    QProcess process;
    if (QSysInfo::productType() == "windows") {
        process.start("where", {"adb"});
    } else {
        process.start("which", {"adb"});
    }
    process.waitForFinished();
    QString output = process.readAll().simplified();
    
    if (!output.isEmpty() && QFile::exists(output)) {
        qInfo() << "find adb: " << output;

        qputenv("QTSCRCPY_ADB_PATH", output.toLocal8Bit());

        return true;
    }

    qWarning() << "can not fount adb!";
    return false;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef Q_OS_LINUX
    qunsetenv("http_proxy");
    qunsetenv("https_proxy");
#endif

    if (!checkADB()) {
        return 0;
    }

    QApplication::setStyle(QStyleFactory::create("fusion"));    //应用Qt原生样式
    QQuickStyle::setStyle("Fusion");
    QElapsedTimer loaderTimer;
    loaderTimer.start();

    ADBTools::instance(&app);
    ADBInterface *interface = ADBInterface::instance(&app);
    interface->startADBService();

    qInfo() << "mainThread: " << QThread::currentThreadId();

    // 连接管理放置到子线程
    QThread *thread = new QThread(&app);
    ConnectManager::instance()->moveToThread(thread);
    thread->start();
    // 开始检测设备连接
    ConnectManager::instance()->startCheckDevice();

    qInfo() << "load1" << loaderTimer.elapsed();
    // 加载InfoPage的相关逻辑
    InfoPageTool::instance(&app);
    qInfo() << "load2" << loaderTimer.elapsed();
    // 加载ControlPage相关逻辑
    ControlPageTool::instance(&app);
    qInfo() << "load3" << loaderTimer.elapsed();
    // 加载AppPage的相关逻辑
    AppPageTool::instance(&app);
    qInfo() << "load4" << loaderTimer.elapsed();
    // 加载FlashPage相关逻辑
    FlashPageTool::instance(&app);
    qInfo() << "load5" << loaderTimer.elapsed();
    // 加载ImagePage相关逻辑
    ImagePageTool::instance(&app);
    qInfo() << "load6" << loaderTimer.elapsed();
    // 加载SettingPage的相关逻辑
    SettingPageTools::instance(&app);
    qInfo() << "load7" << loaderTimer.elapsed();

    // 注册通知控制管理
    NotificationController::instance(&app);
    qmlRegisterSingletonInstance("NotificationController", 1, 0, "NotificationController", NotificationController::instance());
    qInfo() << "load8" << loaderTimer.elapsed();

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/qml/Main.qml");
    engine.load(url);
    qInfo() << "load9" << loaderTimer.elapsed();

    QObject::connect(qApp, &QApplication::aboutToQuit, qApp, [thread]() {
        ConnectManager::instance()->stopCheckDevice();
        qInfo() << "等待设备管理线程结束";
        thread->quit();
        thread->wait();
        qInfo() << "设备管理线程结束";
    });

    return app.exec();
}
