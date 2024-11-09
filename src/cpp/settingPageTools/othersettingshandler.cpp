#include "othersettingshandler.h"
#include "../utils/globalsetting.h"

OtherSettingsHandler::OtherSettingsHandler(QObject *parent)
    : QObject(parent)
    , m_triggerTimer(new QTimer(this))
{
    m_triggerTimer->setInterval(300);
    m_triggerTimer->setSingleShot(true);
    GlobalSetting::instance()->checkConfig("other", "wrapperOpacity", 0.3);
    double configWrapperOpacity = GlobalSetting::instance()->readConfig("other", "wrapperOpacity").toDouble();
    setWrapperOpacity(configWrapperOpacity);
    connect(m_triggerTimer, &QTimer::timeout, this, &OtherSettingsHandler::syncConfig);

}

void OtherSettingsHandler::setWrapperOpacity(double value)
{
    if (m_wrapperOpacity != value) {
        m_wrapperOpacity = value;
        Q_EMIT wrapperOpacityChanged(value);
        m_triggerTimer->start();
    }
}

void OtherSettingsHandler::syncConfig()
{
    GlobalSetting::instance()->writeConfig("other", "wrapperOpacity", m_wrapperOpacity);
}
