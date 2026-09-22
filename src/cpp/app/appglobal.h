#pragma once

#include "src/cpp/utils/defutils.hpp"
#include <QObject>

#define App AppGlobal::instance()

class AppGlobal : public QObject
{
    Q_OBJECT
    SINGLETON(AppGlobal)
    Q_PROPERTY(ThemeType themeType READ themeType WRITE setThemeType NOTIFY themeTypeChanged)

public:
    enum ThemeType{
        Light,
        Dark
    };
    Q_ENUM(ThemeType)

    ThemeType themeType() const { return m_themeType; };
    Q_INVOKABLE void setThemeType(ThemeType type);

signals:
    void themeTypeChanged();

private:
    ThemeType m_themeType = ThemeType::Light;
};
