#pragma once

#include <QObject>
#include <QTimer>
#include "../utils/defutils.hpp"

class OtherSettingsHandler : public QObject
{
    Q_OBJECT
    SINGLETON(OtherSettingsHandler)
    Q_PROPERTY(double wrapperOpacity READ wrapperOpacity WRITE setWrapperOpacity NOTIFY wrapperOpacityChanged)
    Q_PROPERTY(bool useOpenGL READ useOpenGL WRITE setUseOpenGL NOTIFY useOpenGLChanged)
    Q_PROPERTY(int deviceRefreshInterval READ deviceRefreshInterval WRITE setDeviceRefreshInterval NOTIFY deviceRefreshIntervalChanged)
public:
    double wrapperOpacity() const { return m_wrapperOpacity; }
    void setWrapperOpacity(double value);

    bool useOpenGL() const { return m_useOpenGL; }
    void setUseOpenGL(bool value);
    
    int deviceRefreshInterval() const { return m_deviceRefreshInterval; }
    void setDeviceRefreshInterval(int value);

signals:
    void wrapperOpacityChanged(double value);
    void useOpenGLChanged(bool value);
    void deviceRefreshIntervalChanged(int value);

private:
    double m_wrapperOpacity;
    bool m_useOpenGL;
    int m_deviceRefreshInterval;
};
