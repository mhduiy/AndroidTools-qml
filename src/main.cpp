#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QElapsedTimer>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

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
#ifdef Q_OS_WIN
    const QString executable = "adb.exe";
#else
    const QString executable = "adb";
#endif
    const QString appDir = QCoreApplication::applicationDirPath();
    QString adbPath = QStandardPaths::findExecutable(executable);
    const QStringList bundledPaths = {
        appDir + "/tools/" + executable,
        appDir + "/../Resources/tools/" + executable,
        appDir + "/../lib/android-tools/tools/" + executable,
        appDir + "/lib/" + executable,
        appDir + "/../../../lib/" + executable
    };

    for (const QString &path : bundledPaths) {
        if (adbPath.isEmpty() && QFileInfo::exists(path)) {
            adbPath = QDir::cleanPath(path);
        }
    }

    if (adbPath.isEmpty()) {
        qWarning() << "can not find adb";
        return false;
    }

    const QString path = QFileInfo(adbPath).absolutePath()
        + QDir::listSeparator() + QString::fromLocal8Bit(qgetenv("PATH"));
    qputenv("PATH", path.toLocal8Bit());
    qputenv("QTSCRCPY_ADB_PATH", adbPath.toLocal8Bit());
    return true;
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
    const QString appDir = QCoreApplication::applicationDirPath();
    engine.addImportPath(appDir + "/qml");
    engine.addImportPath(appDir + "/../Resources/qml");
    engine.addImportPath(appDir + "/../share/android-tools/qml");
    engine.addImportPath(appDir + "/../../../qml");
    const QUrl url("qrc:/qml2/Main.qml");
    engine.load(url);
    qInfo() << "QML界面加载完成，用时(ms):" << loaderTimer.elapsed();

    return app.exec();
}
