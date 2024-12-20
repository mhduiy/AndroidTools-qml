#include "othersettingshandler.h"
#include "../utils/globalsetting.h"

OtherSettingsHandler::OtherSettingsHandler(QObject *parent)
    : QObject(parent)
    , m_triggerTimer(new QTimer(this))
{
    m_triggerTimer->setInterval(300);
    m_triggerTimer->setSingleShot(true);
    connect(m_triggerTimer, &QTimer::timeout, this, &OtherSettingsHandler::syncConfig);

    GlobalSetting::instance()->checkConfig("other", "wrapperOpacity", 0.3);
    double configWrapperOpacity = GlobalSetting::instance()->readConfig("other", "wrapperOpacity").toDouble();

    GlobalSetting::instance()->checkConfig("other", "useOpenGL", QVariant::fromValue(false));
    bool useOpenGL = GlobalSetting::instance()->readConfig("other", "useOpenGL").toBool();

    setWrapperOpacity(configWrapperOpacity);
    setUseOpenGL(useOpenGL);
}

void OtherSettingsHandler::setWrapperOpacity(double value)
{
    if (m_wrapperOpacity != value) {
        m_wrapperOpacity = value;
        Q_EMIT wrapperOpacityChanged(value);
        m_triggerTimer->start();
    }
}

void OtherSettingsHandler::setUseOpenGL(bool value)
{
    qWarning() << "=========";
    if (m_useOpenGL != value) {
        m_useOpenGL = value;
        Q_EMIT useOpenGLChanged(value);
        m_triggerTimer->start();
    }
}

void OtherSettingsHandler::syncConfig()
{
    GlobalSetting::instance()->writeConfig("other", "wrapperOpacity", m_wrapperOpacity);
    GlobalSetting::instance()->writeConfig("other", "useOpenGL", m_useOpenGL);
}
