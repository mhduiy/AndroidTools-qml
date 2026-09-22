#include "appglobal.h"
#include "src/cpp/utils/constants.h"
#include "src/cpp/utils/globalsetting.h"

static auto constexpr Other_Section = "other";
static auto constexpr Theme_Type_Key = "themeType";

AppGlobal::AppGlobal(QObject *parent)
    : QObject(parent)
{
    m_themeType = (ThemeType)AppSettings->readConfig(Other_Section, Theme_Type_Key, DEFAULT_APP_THEME_TYPE).toInt();
}

void AppGlobal::setThemeType(ThemeType type)
{
    if (m_themeType != type) {
        m_themeType = type;
        AppSettings->writeConfig(Other_Section, Theme_Type_Key, type);
        Q_EMIT themeTypeChanged();
    }
}
