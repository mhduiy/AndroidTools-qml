#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QElapsedTimer>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QProcess>

#include "cpp/adb/connectmanager.h"
#include "cpp/adb/adblog.h"
#include "cpp/adb/adbdevice.h"
#include "src/cpp/adb/devicehelper.h"
#include "cpp/components/fpsitem.h"
#include "cpp/components/SystemInfoProvider.h"
#include "cpp/utils/globalsetting.h"
#include "cpp/utils/constants.h"
#include "cpp/utils/notificationcontroller.h"
#include "cpp/utils/serviceregistry.h"
#include "cpp/app/appglobal.h"
#include "cpp/imagePageTool/scrcpy/ui/mirror/imageframeitem.h"

bool checkADB()
{
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

    QQuickStyle::setStyle("Fusion");
    QElapsedTimer loaderTimer;
    loaderTimer.start();

    qInfo() << "mainThread:" << QThread::currentThreadId();

    ServiceRegistry registry(&app);
    registry.initialize();

    ADT::CONNECTMANAGER->startADBServer([&]() {
        QMetaObject::invokeMethod(ADT::CONNECTMANAGER, "startCheckDevice", Qt::QueuedConnection);
    });

    qmlRegisterSingletonInstance("NotificationController", 1, 0, "NotificationController", NotificationController::instance());
    qmlRegisterType<FpsItem>("FpsItem", 1, 0, "FpsItem");
    qmlRegisterType<ImageFrameItem>("ImageFrameItem", 1, 0, "ImageFrameItem");
    qmlRegisterSingletonInstance("SystemInfo", 1, 0, "SystemInfo", SystemInfoProvider::instance());
    qmlRegisterSingletonInstance("App", 1, 0, "App", App);
    qmlRegisterSingletonInstance("ConnectManager", 1, 0, "ConnectManager", ADT::ConnectManager::instance());
    qmlRegisterSingletonInstance("DeviceHelper", 1, 0, "DeviceHelper", ADT::DeviceHelper::instance());
    qmlRegisterSingletonInstance("ADBLog", 1, 0, "ADBLog", ADBLogModel::instance(&app));

    qmlRegisterUncreatableMetaObject(ADT::staticMetaObject, "ADT", 1, 0, "ADT", "Access to enums & flags only");

    qInfo() << "核心模块加载完成，用时(ms):" << loaderTimer.elapsed();

    AppSettings->checkConfig("other", "useOpenGL", DEFAULT_USE_OPENGL);
    bool useOpenGL = GlobalSetting::instance()->readConfig("other", "useOpenGL").toBool();
    if (useOpenGL) {
        qInfo() << "force use OpenGL";
        forceOpenGL();
    }

    QQmlApplicationEngine engine;
    // TODO: Make FluentUI QML import path cross-platform (CMake install step)
#ifdef Q_OS_MACOS
    engine.addImportPath("/opt/homebrew/qml");
#endif
    const QUrl url("qrc:/qml2/Main.qml");
    engine.load(url);
    qInfo() << "QML界面加载完成，用时(ms):" << loaderTimer.elapsed();

    return app.exec();
}
