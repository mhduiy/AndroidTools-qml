#pragma once

#include <QObject>
#include <QDebug>
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
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

public:
    explicit ResourceService(QObject *parent = nullptr);

    enum DisplayOrientation {
        PORTRAIT,    // Portrait orientation is vertical
        LANDSCAPE,   // Landscape orientation is horizontal
    };
    Q_ENUMS(DisplayOrientation)


    void setSerial(const QString &serial);
    QString serial();

    Q_INVOKABLE void qmlRequest(QString request, QString data);

    bool getMirror() const;

    int orientation() const;
    QSize frameSize() const;

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
    void setOrientation(int);

signals:
    void mirrorChanged(bool value);
    void frameSizeChanged(QSize size);
    void orientationChanged(int orientation);

    void qmlGenerateEvents(QString request, QString data);
    void cppGenerateEvents(QString request, QString data);

private:
    QSize m_frameSize;

    bool m_mirror = false;
    QString m_serial;

    int m_orientation = DisplayOrientation::PORTRAIT;
};
