#include "resolutioncontrol.h"
#include "../adb/adbtools.h"
#include "../adb/connectmanager.h"
#include "../utils/utils.hpp"

namespace ADT {

ResolutionControl::ResolutionControl(QObject *parent) : QObject(parent)
{
}

ResolutionControl::~ResolutionControl()
{
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
