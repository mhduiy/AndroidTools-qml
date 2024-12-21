#include "othersettingshandler.h"
#include "../utils/globalsetting.h"

static auto constexpr Other_Section = "other";
static auto constexpr Wrapper_opacity_key = "wrapperOpacity";
static auto constexpr Use_OpenGL_Key = "useOpenGL";

OtherSettingsHandler::OtherSettingsHandler(QObject *parent)
    : QObject(parent)
{
    double configWrapperOpacity = AppSettings->readConfig(Other_Section, Wrapper_opacity_key, 0.3).toDouble();
    setWrapperOpacity(configWrapperOpacity);

    bool useOpenGL = AppSettings->readConfig(Other_Section, Use_OpenGL_Key, false).toBool();
    setUseOpenGL(useOpenGL);
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