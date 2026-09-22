#ifndef RESOLUTIONCONTROL_H
#define RESOLUTIONCONTROL_H

#include <QObject>
#include "../utils/defutils.hpp"

namespace ADT {

class ResolutionControl : public QObject
{
    Q_OBJECT
    SINGLETON(ResolutionControl)
public:
    ~ResolutionControl();

    Q_INVOKABLE void set(quint16 width, quint16 height, quint16 dpi);
    Q_INVOKABLE void restore();

private:
    quint16 m_screenHeight = 0;
    quint16 m_scrennWidth = 0;
    quint16 m_deviceDpi = 0;
};

} // namespace ADT

#endif // RESOLUTIONCONTROL_H
