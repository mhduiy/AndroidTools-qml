#pragma once

#include <QObject>
#include <QList>
#include <QDebug>
#include <QByteArray>

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QtWebSockets>

#include "service/ResourceService.h"
#include "service/ServiceManager.h"

//!
//! This class communicate with android clients, receive request and its answer them
//!
class WebSocketService : public QObject
{
Q_OBJECT

public:
    explicit WebSocketService(quint16 port, QObject *parent = Q_NULLPTR);
    ~WebSocketService();

signals:
    void closed();

private slots:
    void onNewConnection();
    void socketDisconnected();

    void clientRequest(QString jsonString);

    void qmlGenerateEvents(QString request, QString data);

    void onDeviceConnected(bool success, const QString& serial, const QString& deviceName, const QSize& size);
    void onDeviceDisconnected(QString serial);

private:
    void responseToClents(int result);
    void requestMirrorStart();

    bool checkAdbRun();
    void execute(const QString &serial, const QStringList &args);
    static QJsonObject createJsonObject(QString jsonString);

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    ResourceService* resourceService = nullptr;
    qsc::AdbProcess m_adb;
};
