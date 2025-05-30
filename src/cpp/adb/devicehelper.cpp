#include "devicehelper.h"
#include "connectmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QCoreApplication>

DeviceHelper::DeviceHelper(QObject *parent)
    : QObject(parent)
    , m_connectionManagerThread(new QThread(this))
{

}

void DeviceHelper::init()
{
    initConnectManager();
    CONNECTMANAGER->startCheckDevice();
}


void DeviceHelper::initConnectManager()
{
    // 不移动ConnectManager到子线程，保持在主线程中
    // CONNECTMANAGER->moveToThread(m_connectionManagerThread);
    // m_connectionManagerThread->start();
    
    // 开始检测设备连接
    CONNECTMANAGER->startCheckDevice();

    connect(CONNECTMANAGER, &ConnectManager::deviceConnected, this, [this](QSharedPointer<Device> device) {
        if (device->connectStatus() == C_ADB) {
            auto adbDevice = device.dynamicCast<ADBDevice>();
            if (adbDevice) {
                // 将原始指针添加到列表
                auto newList = m_adbDeviceList;
                newList.append(adbDevice.get());
                setadbDeviceList(newList);
            }
        }
    });

    connect(CONNECTMANAGER, &ConnectManager::deviceDisconnected, this, [this](QSharedPointer<Device> device) {
        if (device->connectStatus() == C_ADB) {
            auto newList = m_adbDeviceList;
            for (int i = 0; i < newList.size(); ++i) {
                // 比较原始指针
                if (newList[i] == device.get()) {
                    newList.removeAt(i);
                    setadbDeviceList(newList);
                    break;
                }
            }
        }
    });
    
    // 同步当前已连接的设备
    auto existingDevices = CONNECTMANAGER->devices(C_ADB);
    QList<ADBDevice*> deviceList;
    for (const auto &device : existingDevices) {
        if (device->connectStatus() == C_ADB) {
            auto adbDevice = device.dynamicCast<ADBDevice>();
            if (adbDevice) {
                deviceList.append(adbDevice.get());
            }
        }
    }
    
    if (deviceList.size() > 0) {
        setadbDeviceList(deviceList);
    }
}

void DeviceHelper::requestDisConnectDevice(const QString &deviceCode, ConnectStatus)
{
    if (deviceCode.isEmpty()) {
        return;
    }

    auto &&device = CONNECTMANAGER->devices(C_ADB);
    for (const auto &d : device) {
        if (d->code() == deviceCode) {
            d->requestDisConnect();
            break;
        }
    }
}
