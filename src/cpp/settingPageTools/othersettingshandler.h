#pragma once

#include <QObject>
#include <QTimer>
#include "../utils/singleton.hpp"

class OtherSettingsHandler : public QObject
{
    Q_OBJECT
    SINGLETON(OtherSettingsHandler)
    Q_PROPERTY(double wrapperOpacity READ wrapperOpacity WRITE setWrapperOpacity NOTIFY wrapperOpacityChanged)
    Q_PROPERTY(bool useOpenGL READ useOpenGL WRITE setUseOpenGL NOTIFY useOpenGLChanged)
public:
    double wrapperOpacity() const { return m_wrapperOpacity; }
    void setWrapperOpacity(double value);

    bool useOpenGL() const { return m_useOpenGL; }
    void setUseOpenGL(bool value);

signals:
    void wrapperOpacityChanged(double value);
    void useOpenGLChanged(bool value);

private slots:
    void syncConfig();

private:
    double m_wrapperOpacity;
    bool m_useOpenGL;
    QTimer *m_triggerTimer;
};
