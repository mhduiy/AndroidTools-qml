#pragma once

#include <QObject>
#include <QTimer>
#include "../utils/singleton.hpp"

class OtherSettingsHandler : public QObject
{
    Q_OBJECT
    SINGLETON(OtherSettingsHandler)
    Q_PROPERTY(double wrapperOpacity READ wrapperOpacity WRITE setWrapperOpacity NOTIFY wrapperOpacityChanged)
public:
    double wrapperOpacity() const { return m_wrapperOpacity; }
    void setWrapperOpacity(double value);

signals:
    void wrapperOpacityChanged(double value);

private slots:
    void syncConfig();

private:
    double m_wrapperOpacity;
    QTimer *m_triggerTimer;
};
