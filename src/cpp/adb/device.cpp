#include "device.h"

Device::Device(const QString &code, QObject *parent) : QObject(parent)
{
    setcode(code);
}
