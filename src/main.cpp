#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include "cpp/components/deivicelistviewmodel.h"
#include <QQmlContext>
#include "cpp/adb/adbtools.h"
#include "cpp/adb/adbinterface.h"
#include <QThread>

#include "cpp/infoPageTool/batteryControl.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ADBTools::instance(&app);
    ADBInterface *interface = ADBInterface::instance(&app);
    BatteryControl::instance(&app);

    QThread *thread = new QThread();
    interface->moveToThread(thread);
    thread->start();

    interface->startRefreshDevice();

    qWarning() << "ADB: " << interface->adbVersion() << QThread::currentThreadId();

    DeviceListviewModel deviceListviewModel;

    QObject::connect(interface, &ADBInterface::deviceDisconnected, [&deviceListviewModel, &interface](const QString &code){
        deviceListviewModel.removeRow(code);
    });

    QObject::connect(interface, &ADBInterface::deviceStatusUpdateFinish, [&deviceListviewModel, &interface](){
        auto devices = interface->getDeviceCodeSet();
        for (const QString &deviceCode : devices) {
            BatteryControl::instance()->updateBatteryInfo(*interface->getDeviceBatteryInfo(deviceCode).data());
            if (deviceListviewModel.hasDeviceCode(deviceCode)) {
                deviceListviewModel.setInfo(*interface->getDeviceBaceInfo(deviceCode));
            } else { // 是一个新设备
                deviceListviewModel.appendRow(*interface->getDeviceBaceInfo(deviceCode));
            }
        }
    });

    qmlRegisterSingletonInstance("BatteryControl", 1, 0, "BatteryControl", BatteryControl::instance());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("deviceListviewModel", &deviceListviewModel);
    const QUrl url("qrc:/qml/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
