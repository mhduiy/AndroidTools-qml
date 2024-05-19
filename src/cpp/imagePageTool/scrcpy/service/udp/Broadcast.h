#pragma once

#include <QObject>
#include <QTimer>

#include <QtNetwork>
#include <QUdpSocket>

//!
//! Broadcast QmlScrcpy IP for android clients until can connect to websoket server
//!
class Broadcast : public QObject
{
    Q_OBJECT

public:
    explicit Broadcast(QObject *parent = nullptr);

    void start();
    void stop();

private slots:
    void broadcastDatagram();

private:
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    int messageNo = 1;
};
