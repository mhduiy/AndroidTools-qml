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
#include "cpp/controlPageTool/controlPageTool.h"
#include "cpp/appPageTool/appPagetool.h"
#include "cpp/flashPageTool/flashPageTool.h"
#include "cpp/imagePageTool/imagePageTool.h"
#include "cpp/settingPageTool/settingPageTools.h"
#include "cpp/utils/notificationcontroller.h"
#include "cpp/components/fpsitem.h"
#include "cpp/utils/globalsetting.h"
#include "cpp/app/appglobal.h"
#include "cpp/imagePageTool/scrcpy/ui/mirror/imageframeitem.h"
#include "cpp/utils/constants.h"
#include "cpp/adb/adblog.h"
#include "src/cpp/adb/devicehelper.h"
#include "src/cpp/adb/adbdevice.h"

#include <QProcess>
bool checkADB() {
    QProcess process;
    QString trueADBPath;

    if (QSysInfo::productType() == "windows") {
        process.start("powershell", {"-Command", "Get-Command adb | Select-Object -ExpandProperty Source"});
    } else {
        process.start("which", {"adb"});
    }

    process.waitForFinished();
    QString output = process.readAll().simplified();

    if (!output.isEmpty() && QFile::exists(output)) {
        trueADBPath = output;
    }

    if (!trueADBPath.isEmpty()) {
        qInfo() << "find adb:" << trueADBPath;
        qputenv("QTSCRCPY_ADB_PATH", trueADBPath.toLocal8Bit());
        return true;
    }

    qWarning() << "can not found adb!";
    return false;
}

void forceOpenGL()
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QSurfaceFormat format;
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
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

    qInfo() << "mainThread:" << QThread::currentThreadId();

    ADT::DeviceHelper::instance(qApp)->init();

    ADT::ControlPageTool::instance(&app);
    ADT::AppPageTool::instance(&app);
    ADT::FlashPageTool::instance(&app);
    ADT::ImagePageTool::instance(&app);
    ADT::SettingPageTools::instance(&app);
    
    NotificationController::instance(&app);


    qmlRegisterSingletonInstance("NotificationController", 1, 0, "NotificationController", NotificationController::instance());
    qmlRegisterType<FpsItem>( "FpsItem", 1, 0, "FpsItem");
    qmlRegisterType<ImageFrameItem>( "ImageFrameItem", 1, 0, "ImageFrameItem");
    qmlRegisterSingletonInstance("App", 1, 0, "App", App);
    qmlRegisterSingletonInstance("ConnectManager", 1, 0, "ConnectManager", ADT::ConnectManager::instance());
    qmlRegisterSingletonInstance("DeviceHelper", 1, 0, "DeviceHelper", ADT::DeviceHelper::instance());
    qmlRegisterSingletonInstance("ADBLog", 1, 0, "ADBLog", ADBLogModel::instance(&app));
    
    // 注册枚举类到QML
    qmlRegisterUncreatableMetaObject(ADT::staticMetaObject, "ADT", 1, 0, "ADT", "Access to enums & flags only");
    
    qInfo() << "核心模块加载完成，用时(ms):" << loaderTimer.elapsed();

    AppSettings->checkConfig("other", "useOpenGL", DEFAULT_USE_OPENGL);
    bool useOpenGL = GlobalSetting::instance()->readConfig("other", "useOpenGL").toBool();
    if (useOpenGL) {
        qInfo() << "force use OpenGL";
        forceOpenGL();
    }

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/qml/Main.qml");
    engine.load(url);
    qInfo() << "QML界面加载完成，用时(ms):" << loaderTimer.elapsed();

    return app.exec();
}
