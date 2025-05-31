#include "resolutioncontrol.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/utils.hpp"
#include <QDebug>

namespace ADT {

ResolutionControl::ResolutionControl(QObject *parent) : QObject(parent)
{
    // 监听当前设备的详细信息变化
    connect(ConnectManager::instance(), &ConnectManager::cutADBDeviceChanged, this, &ResolutionControl::onDeviceInfoChanged);
    
    // 初始更新一次
    onDeviceInfoChanged();
}

ResolutionControl::~ResolutionControl()
{
    qInfo() << "ResolutionControl destroyed";
}

void ResolutionControl::onDeviceInfoChanged()
{
    // auto device = ConnectManager::instance()->cutADBDevice();
    // if (device && device->detailInfo()) {
    //     auto detailInfo = device->detailInfo();
        
    //     // 解析分辨率信息
    //     QString resolutionStr = detailInfo->resolving;
    //     if (!resolutionStr.isEmpty() && resolutionStr.contains('x')) {
    //         auto sizeInfo = resolutionStr.split('x');
    //         if (sizeInfo.size() >= 2) {
    //             setResolutionWidth(sizeInfo.first().toUInt());
    //             setResolutionHeight(sizeInfo.last().toUInt());
    //         }
    //     }
        
    //     // 设置DPI
    //     setScreenDPI(detailInfo->dpi.toUInt());
    // }
}

void ResolutionControl::setResolutionWidth(quint16 width)
{
    if (m_scrennWidth != width) {
        m_scrennWidth = width;
        emit resolutionWidthChanged(width);
    }
}

void ResolutionControl::setResolutionHeight(quint16 height)
{
    if (m_screenHeight != height) {
        m_screenHeight = height;
        emit resolutionHeightChanged(height);
    }
}

void ResolutionControl::setScreenDPI(quint16 dpi)
{
    if (m_deviceDpi != dpi) {
        m_deviceDpi = dpi;
        emit screenDPIChanged(dpi);
    }
}

quint16 ResolutionControl::getResolutionWidth() const
{
    return m_scrennWidth;
}

quint16 ResolutionControl::getResolutionHeight() const
{
    return m_screenHeight;
}

quint16 ResolutionControl::getScreenDPI() const
{
    return m_deviceDpi;
}

void ResolutionControl::set(quint16 width, quint16 height, quint16 dpi)
{
    auto operatorFunc = [this, width, height, dpi](){
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        
        const QString cutDevice = device->code();
        QStringList args;
        
        if (width != this->m_scrennWidth || height != this->m_screenHeight) {
            args << "-s" << cutDevice << "shell"
                 << "wm" << "size" << QString("%1x%2").arg(width).arg(height);
            ADBTools::instance()->executeCommand(ADBTools::ADB, args);
        }

        if (dpi != this->m_deviceDpi) {
            args.clear();
            args << "-s" << cutDevice << "shell"
                 << "wm" << "density" << QString::number(dpi);
            ADBTools::instance()->executeCommand(ADBTools::ADB, args);
        }
    };

    asyncOperator(operatorFunc);
}

void ResolutionControl::restore()
{
    auto operatorFunc = [](){
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) return;
        
        const QString cutDevice = device->code();
        QStringList args;
        
        args << "-s" << cutDevice << "shell"
             << "wm" << "size" << "reset";
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);

        args.clear();
        args << "-s" << cutDevice << "shell"
             << "wm" << "density" << "reset";
        ADBTools::instance()->executeCommand(ADBTools::ADB, args);
    };

    asyncOperator(operatorFunc);
}

} // namespace ADT