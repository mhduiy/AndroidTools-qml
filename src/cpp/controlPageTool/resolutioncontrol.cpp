#include "resolutioncontrol.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/utils.hpp"

ResolutionControl::ResolutionControl(QObject *parent) : QObject(parent)
{
    m_thread = new QThread(this);
    auto *m_detailInfohelper = new DetailInfoUpdateHelper();
    m_detailInfohelper->moveToThread(m_thread);
    m_thread->start();
    connect(ConnectManager::instance(), &ConnectManager::currentDeviceChanged, m_detailInfohelper, &DetailInfoUpdateHelper::updateInfo);
    connect(m_detailInfohelper, &DetailInfoUpdateHelper::updateFinish, this, [this, m_detailInfohelper](){
        auto infos = m_detailInfohelper->getInfo();
        auto sizeInfo = infos.value(7).toString().split('x');
        setResolutionWidth(sizeInfo.first().toUInt());
        setResolutionHeight(sizeInfo.last().toUInt());
        setScreenDPI(infos.value(8).toUInt());
    });
}

ResolutionControl::~ResolutionControl()
{
    qInfo() << "ResolutionControl Thread exiting";
    m_thread->quit();
    m_thread->wait();
    qInfo() << "ResolutionControl Thread exited";
}

void ResolutionControl::setResolutionWidth(quint16 width)
{
    m_scrennWidth = width;
    emit resolutionWidthChanged(width);
}

void ResolutionControl::setResolutionHeight(quint16 height)
{
    m_screenHeight = height;
    emit resolutionHeightChanged(height);
}

void ResolutionControl::setScreenDPI(quint16 dpi)
{
    m_deviceDpi = dpi;
    emit screenDPIChanged(dpi);
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
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
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
        const QString cutDevice = ConnectManager::instance()->currentDeviceCode();
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
