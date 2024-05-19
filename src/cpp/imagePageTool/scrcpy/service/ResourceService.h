#pragma once

#include <QObject>
#include <QDebug>
#include <QPointer>
#include <QSize>
#include "service/models/Mirror.h"
#include "core/include/adbprocess.h"

//! This class will act as a MainThread data cullmination point
//! Save all parametrs for connection between objects
//!
class ResourceService : public QObject
{
Q_OBJECT

    Q_PROPERTY(double mirror READ getMirror WRITE setMirror NOTIFY mirrorChanged)
    Q_PROPERTY(QSize frameSize READ frameSize WRITE setFrameSize NOTIFY frameSizeChanged)
    Q_PROPERTY(QSize portraitSize READ portraitSize WRITE setPortraitSize NOTIFY portraitSizeChanged)
    Q_PROPERTY(QSize landscapeSize READ landscapeSize WRITE setLandscapeSize NOTIFY landscapeSizeChanged)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

    Q_PROPERTY(int scene READ scene WRITE setScene NOTIFY sceneChanged)

public:
    explicit ResourceService(QObject *parent = nullptr);

    static void declareQml();

    enum DisplayOrientation {
        PORTRAIT,    // Portrait orientation is vertical
        LANDSCAPE,   // Landscape orientation is horizontal
    };
    Q_ENUMS(DisplayOrientation)


    void setSerial(const QString &serial);
    QString serial();

    Q_INVOKABLE void qmlToolbarClick(QString request);
    Q_INVOKABLE void qmlRequest(QString request, QString data);

    bool getMirror() const;

    int orientation() const;
    int scene() const;
    QSize frameSize() const;

    QSize portraitSize() const;
    QSize landscapeSize() const;

    void clearMirrorCash();
    void setUsbMirrorParametre() const;

    Mirror *mirror = nullptr;

    //! save serial for restart tcpip 5555 for first time. this cause we can access by Wi-Fi connection
    //! #adb -s RZ8NC0KA54H tcpip 5555
    //! #adb connect 192.168.1.161
    //!
    QList<QString> m_usb_serials;

public slots:

    void setMirror(bool vlaue);
    void setFrameSize(QSize size);
    void setPortraitSize(QSize size);
    void setLandscapeSize(QSize size);
    void setOrientation(int);
    void setScene(int);

signals:
    void mirrorChanged(bool value);
    void frameSizeChanged(QSize size);
    void portraitSizeChanged(QSize size);
    void landscapeSizeChanged(QSize size);
    void orientationChanged(int orientation);
    void sceneChanged(int scene);

    void qmlGenerateEvents(QString request, QString data);
    void cppGenerateEvents(QString request, QString data);

private:
    QSize m_frameSize;
    QSize m_portraitSize;
    QSize m_landscapeSize;

    bool m_mirror = false;
    QString m_serial;

    int m_orientation = DisplayOrientation::PORTRAIT;
    int m_scene = 0;


    bool m_showTouch = false;
};
