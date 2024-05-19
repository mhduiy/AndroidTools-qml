#include "Broadcast.h"

Broadcast::Broadcast(QObject *parent): QObject(parent) {
    udpSocket = new QUdpSocket(this);
    connect(&timer, &QTimer::timeout, this, &Broadcast::broadcastDatagram);
}

void Broadcast::start() {
    timer.start(500);
}
void Broadcast::stop() {
    timer.stop();
}

void Broadcast::broadcastDatagram() {
    //qDebug() << "Now broadcasting datagram:" << messageNo;
    QByteArray datagram = "Qt Count: " + QByteArray::number(messageNo);
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 45454);
    messageNo++;
}
