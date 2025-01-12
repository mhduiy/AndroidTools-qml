#include "appglobal.h"
#include "src/cpp/utils/globalsetting.h"
#include <QDebug>

static auto constexpr Other_Section = "other";
static auto constexpr Theme_Type_Key = "themeType";

const QColor SuggestBackColor =  QColor(0, 125, 200, 0.1 * 255);
const QColor SuggestHoverColor =  QColor(0, 125, 200, 0.3 * 255);
const QColor SuggestClickedColor = QColor(0, 125, 200, 0.8 * 255);
const QColor OrdinaryBackColor = QColor(0, 0, 0, 0.1 * 255);
const QColor OrdinaryHoverColor = QColor(0, 0, 0, 0.3 * 255);
const QColor OrdinaryClickedColor =  QColor(0, 0, 0, 0.8 * 255);
const QColor WarningBackColor = QColor(215, 80, 80, 0.1 * 255);
const QColor WarningHoverColor = QColor(215, 80, 80, 0.3 * 255);;
const QColor WarningClickedColor = QColor(215, 80, 80, 0.8 * 255);

const QColor LightTextColor = OrdinaryClickedColor;
const QColor DarkTextColor = QColor(255, 255, 255, 0.8 * 255);

const QColor LightBase = Qt::white;
const QColor DarkBase = Qt::black;

const QColor LightMid = QColor(255, 255, 255, 0.4 * 255);
const QColor DarkMid = QColor(0, 0, 0, 0.4 * 255);

const QColor LightBorderColor = QColor(0, 0, 0, 0.2 * 255);
const QColor DarkBorderColor = QColor(255, 255, 255, 0.2 * 255);

AppGlobal::AppGlobal(QObject *parent)
    : QObject(parent)
{
    m_themeType = (ThemeType)AppSettings->readConfig(Other_Section, Theme_Type_Key, ThemeType::Light).toInt();
    reload();
}

QColor AppGlobal::adjustColorOpacity(const QColor &color, float opacity)
{
    QColor _color(color);
    _color.setAlpha(opacity * 255);
    return _color;
}

void AppGlobal::reload()
{
    m_windowTextColor = m_themeType == ThemeType::Light ? LightTextColor : DarkTextColor;
    m_windowTextAssitColor = adjustColorOpacity(m_windowTextColor, 0.5);
    m_baseColor = m_themeType == ThemeType::Light ? LightBase : DarkBase;
    m_midColor = m_themeType == ThemeType::Light ? LightMid : DarkMid;
    m_borderColor = m_themeType == ThemeType::Light ? LightBorderColor : DarkBorderColor;

    m_suggestColorWeight1 = m_themeType == ThemeType::Light ? SuggestBackColor : SuggestBackColor;
    m_suggestColorWeight2 = m_themeType == ThemeType::Light ? SuggestHoverColor : SuggestHoverColor;
    m_suggestColorWeight3 = m_themeType == ThemeType::Light ? SuggestClickedColor : SuggestClickedColor;
    m_warningColorWeight1 = m_themeType == ThemeType::Light ? WarningBackColor : WarningBackColor;
    m_warningColorWeight2 = m_themeType == ThemeType::Light ? WarningHoverColor : WarningHoverColor;
    m_warningColorWeight3 = m_themeType == ThemeType::Light ? WarningClickedColor : WarningClickedColor;
    m_normalColorWeight1 = m_themeType == ThemeType::Light ? OrdinaryBackColor : OrdinaryBackColor;
    m_normalColorWeight2 = m_themeType == ThemeType::Light ? OrdinaryHoverColor : OrdinaryHoverColor;
    m_normalColorWeight3 = m_themeType == ThemeType::Light ? LightTextColor : adjustColorOpacity(DarkTextColor, 0.5);
}

void AppGlobal::setThemeType(ThemeType type)
{
    if (m_themeType != type) {
        qInfo() << "set themeType: " << type;
        m_themeType = type;
        AppSettings->writeConfig(Other_Section, Theme_Type_Key, type);
        reload();
        Q_EMIT themeTypeChanged();
    }
}