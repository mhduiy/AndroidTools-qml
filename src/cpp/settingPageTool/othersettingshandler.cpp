#include "othersettingshandler.h"
#include "../utils/globalsetting.h"
#include "src/cpp/utils/constants.h"

static auto constexpr Other_Section = "other";
static auto constexpr Wrapper_opacity_key = "wrapperOpacity";
static auto constexpr Use_OpenGL_Key = "useOpenGL";
static auto constexpr Device_Refresh_Interval_Key = "deviceRefreshInterval";

OtherSettingsHandler::OtherSettingsHandler(QObject *parent)
    : QObject(parent)
{
    double configWrapperOpacity = AppSettings->readConfig(Other_Section, Wrapper_opacity_key, DEFAULT_WRAPPER_OPACITY).toDouble();
    setWrapperOpacity(configWrapperOpacity);

    bool useOpenGL = AppSettings->readConfig(Other_Section, Use_OpenGL_Key, DEFAULT_USE_OPENGL).toBool();
    setUseOpenGL(useOpenGL);
    
    int deviceRefreshInterval = AppSettings->readConfig(Other_Section, Device_Refresh_Interval_Key, DEFAULT_DEVICE_REFRESH_INTERVAL).toInt();
    setDeviceRefreshInterval(deviceRefreshInterval);
}

void OtherSettingsHandler::setWrapperOpacity(double value)
{
    if (m_wrapperOpacity != value) {
        m_wrapperOpacity = value;
        AppSettings->writeConfig(Other_Section, Wrapper_opacity_key, value);
        Q_EMIT wrapperOpacityChanged(value);
    }
}

void OtherSettingsHandler::setUseOpenGL(bool value)
{
    if (m_useOpenGL != value) {
        m_useOpenGL = value;
        AppSettings->writeConfig(Other_Section, Use_OpenGL_Key, value);
        Q_EMIT useOpenGLChanged(value);
    }
}

void OtherSettingsHandler::setDeviceRefreshInterval(int value)
{
    if (m_deviceRefreshInterval != value) {
        m_deviceRefreshInterval = value;
        AppSettings->writeConfig(Other_Section, Device_Refresh_Interval_Key, value);
        Q_EMIT deviceRefreshIntervalChanged(value);
    }
}
