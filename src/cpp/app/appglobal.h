#pragma once

#include "src/cpp/utils/singleton.hpp"
#include <QObject>
#include <QColor>

#define App AppGlobal::instance()

class AppGlobal : public QObject
{
    Q_OBJECT
    SINGLETON(AppGlobal)
    Q_PROPERTY(QColor windowTextColor MEMBER m_windowTextColor NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor windowTextAssitColor MEMBER m_windowTextAssitColor NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor baseColor MEMBER m_baseColor NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor midColor MEMBER m_midColor NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor borderColor MEMBER m_borderColor NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor suggestColorWeight1 MEMBER m_suggestColorWeight1 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor suggestColorWeight2 MEMBER m_suggestColorWeight2 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor suggestColorWeight3 MEMBER m_suggestColorWeight3 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor warningColorWeight1 MEMBER m_warningColorWeight1 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor warningColorWeight2 MEMBER m_warningColorWeight2 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor warningColorWeight3 MEMBER m_warningColorWeight3 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor normalColorWeight1 MEMBER m_normalColorWeight1 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor normalColorWeight2 MEMBER m_normalColorWeight2 NOTIFY themeTypeChanged)
    Q_PROPERTY(QColor normalColorWeight3 MEMBER m_normalColorWeight3 NOTIFY themeTypeChanged)

    Q_PROPERTY(ThemeType themeType READ themeType WRITE setThemeType NOTIFY themeTypeChanged)
    
public:
    enum ThemeType{ 
        Light,
        Dark
    };
    Q_ENUM(ThemeType)

    Q_INVOKABLE static QColor adjustColorOpacity(const QColor &color, float opacity);

    ThemeType themeType() const { return m_themeType; };
    Q_INVOKABLE void setThemeType(ThemeType type);

signals:
    void themeTypeChanged();

private:
    void reload();

private:
    QColor m_windowTextColor;
    QColor m_windowTextAssitColor;
    QColor m_baseColor;
    QColor m_midColor;
    QColor m_borderColor;

    QColor m_suggestColorWeight1;
    QColor m_suggestColorWeight2;
    QColor m_suggestColorWeight3;

    QColor m_warningColorWeight1;
    QColor m_warningColorWeight2;
    QColor m_warningColorWeight3;

    QColor m_normalColorWeight1;
    QColor m_normalColorWeight2;
    QColor m_normalColorWeight3;

    ThemeType m_themeType = ThemeType::Light;
};