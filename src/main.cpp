#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include "cpp/components/deivicelistviewmodel.h"
#include <QQmlContext>
#include "cpp/adb/adbtools.h"
#include "cpp/adb/adbinterface.h"
#include <QThread>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ADBTools::instance(&app);
    ADBInterface interface;

    QThread *thread = new QThread();
    interface.moveToThread(thread);
    thread->start();

    interface.startRefreshDevice();

    qWarning() << "ADB: " << interface.adbVersion() << QThread::currentThreadId();

    DeviceListviewModel deviceListviewModel;
    deviceListviewModel.appendRow(new DeviceBaceInfo("XIAOMI 8 Lite", "", 100, true, false, true));
    deviceListviewModel.appendRow(new DeviceBaceInfo("OPPO R11 PLUS", "192.168.0.2", 10, true, true, false));
    deviceListviewModel.appendRow(new DeviceBaceInfo("HUAWEI Nova11SE", "", 10, false, false, false));
    deviceListviewModel.appendRow(new DeviceBaceInfo("VIVO Y77", "", 10, false, false, false));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("deviceListviewModel", &deviceListviewModel);
    const QUrl url("qrc:/qml/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
