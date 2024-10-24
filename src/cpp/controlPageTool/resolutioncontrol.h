#ifndef RESOLUTIONCONTROL_H
#define RESOLUTIONCONTROL_H

#include <QObject>
#include "../utils/singleton.hpp"
#include "../infoPageTool/detailinfocontrol.h"

class ResolutionControl : public QObject
{
    Q_OBJECT
    SINGLETON(ResolutionControl)
    Q_PROPERTY(quint16 screenWidth READ getResolutionWidth WRITE setResolutionWidth NOTIFY resolutionWidthChanged)
    Q_PROPERTY(quint16 screenHeight READ getResolutionHeight WRITE setResolutionHeight NOTIFY resolutionHeightChanged)
    Q_PROPERTY(quint16 dpi READ getScreenDPI WRITE setScreenDPI NOTIFY screenDPIChanged)

public:
    ~ResolutionControl();
    void setResolutionWidth(quint16 width);
    void setResolutionHeight(quint16 height);
    void setScreenDPI(quint16 dpi);

    quint16 getResolutionWidth() const;
    quint16 getResolutionHeight() const;
    quint16 getScreenDPI() const;

    Q_INVOKABLE void set(quint16 width, quint16 height, quint16 dpi);
    Q_INVOKABLE void restore();

signals:
    void resolutionWidthChanged(quint16);
    void resolutionHeightChanged(quint16);
    void screenDPIChanged(quint16);

private:
    quint16 m_screenHeight;
    quint16 m_scrennWidth;
    quint16 m_deviceDpi;
    DetailInfoUpdateHelper *m_detailInfohelper;
    QThread *m_thread;
};

#endif // RESOLUTIONCONTROL_H
