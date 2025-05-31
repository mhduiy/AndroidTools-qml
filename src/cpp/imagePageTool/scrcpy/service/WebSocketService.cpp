#include "WebSocketService.h"
#include "service/include/server.h"
#include "src/cpp/utils/constants.h"
#include "src/cpp/utils/notificationcontroller.h"
#include "ui/util/config.h"
#include "src/cpp/adb/connectmanager.h"
#include <qlogging.h>

WebSocketService::WebSocketService(quint16 port, QObject *parent) : QObject(parent) {

    m_pWebSocketServer = new QWebSocketServer("QmlScrcpy Server", QWebSocketServer::NonSecureMode, this);

    resourceService = ServiceManager::getInstance().resourceService();

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        qDebug() << "WebSocket listening on port" << port;

        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketService::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketService::closed);
    }

    connect(resourceService, &ResourceService::qmlGenerateEvents, this, &WebSocketService::qmlGenerateEvents); //Interacting Qml to C++

    connect(&qsc::IDeviceManage::getInstance(), &qsc::IDeviceManage::deviceConnected, this, &WebSocketService::onDeviceConnected);
    connect(&qsc::IDeviceManage::getInstance(), &qsc::IDeviceManage::deviceDisconnected, this, &WebSocketService::onDeviceDisconnected);

    connect(&m_adb, &qsc::AdbProcess::adbProcessResult, this, [this](qsc::AdbProcess::ADB_EXEC_RESULT processResult) {

        QStringList args = m_adb.arguments();

        switch (processResult) {
            case qsc::AdbProcess::AER_ERROR_START:
                responseToClents(Server::SERVER_RESPONSE::MIRROR_ERROR_START);
                break;
            case qsc::AdbProcess::AER_SUCCESS_START:
                //log = "adb run";
                break;
            case qsc::AdbProcess::AER_ERROR_EXEC:
                qDebug() << "AdbProcess::AER_ERROR_EXEC";
                responseToClents(Server::SERVER_RESPONSE::MIRROR_ERROR_START);
                break;
            case qsc::AdbProcess::AER_ERROR_MISSING_BINARY:
                //log = "adb not found";
                break;
            case qsc::AdbProcess::AER_SUCCESS_EXEC:
                //log = m_adb.getStdOut();
                if (args.contains("devices")) {
                    QStringList devices = m_adb.getDevicesSerialFromStdOut();

                    for (auto &item: devices) {
                        if (item.contains(":") || item.contains(".")) { //Ignore if device is Wi-Fi
                            continue;
                        }
                        emit resourceService->cppGenerateEvents("USB_DEVICE_NAME", item);
                        return;
                    }
                    emit resourceService->cppGenerateEvents("USB_DEVICE_NAME", "");
                }
                break;
        }
    });
}

WebSocketService::~WebSocketService() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void WebSocketService::onNewConnection() {

    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketService::socketDisconnected);
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketService::clientRequest);
    qDebug() << "WebSocket: onNewConnection:" << pSocket->peerAddress();

    m_clients << pSocket;
}

void WebSocketService::clientRequest(QString jsonString) { //receive requests of android clients

    QJsonObject jsonObject = createJsonObject(jsonString);

    int clientRequest = jsonObject.value("clientRequest").toInt();

    if (clientRequest == Server::CLIENT_REQUEST::CLIENT_REQUEST_ADB_CONNECT) {

        QString wifiIp = jsonObject.value("wifiIp").toString();
        QStringList adbArgs;
        adbArgs << "connect";
        adbArgs << wifiIp;
        execute("", adbArgs);

    } else if (clientRequest == Server::CLIENT_REQUEST::CLIENT_REQUEST_UPDATE) {

        responseToClents(Server::SERVER_RESPONSE::UPDATE_MIRROR_DATA);

    } else if (clientRequest == Server::CLIENT_REQUEST::CLIENT_REQUEST_MIRROR) {

//        if (resourceService->mirror->username.length() > 3) { // request is reject if if screen mirro is Active. clients must be wait
//            qDebug() << "request mirror rejected";
//            return;
//        }

        resourceService->mirror->wifiIp = jsonObject.value("wifiIp").toString();
        resourceService->mirror->username = jsonObject.value("username").toString();
        resourceService->mirror->title = jsonObject.value("title").toString();
        resourceService->mirror->bitrate = jsonObject.value("bitrate").toInt();
        resourceService->mirror->resolution = jsonObject.value("resolution").toInt();
        resourceService->mirror->connectionType = jsonObject.value("connectionType").toString();

        resourceService->setSerial(resourceService->mirror->wifiIp);
        requestMirrorStart();

//        qDebug() << "";
//        qDebug() << "wifIp:" << resourceService->mirror->wifiIp;
//        qDebug() << "username:" << resourceService->mirror->username;
//        qDebug() << "title:" << resourceService->mirror->title;
//        qDebug() << "bitrate:" << resourceService->mirror->bitrate;
//        qDebug() << "resolution:" << resourceService->mirror->resolution;
//        qDebug() << "connectionType:" << resourceService->mirror->connectionType;
//        qDebug() << "";
    }
}

void WebSocketService::responseToClents(int result) {

    QJsonObject rootObject;

    rootObject["SERVER_RESPONSE"] = result;

    if (result == Server::SERVER_RESPONSE::MIRROR_SUCCESS_START) {
        //responseToClents to clients to update ui
        rootObject["username"] = resourceService->mirror->username;
        rootObject["title"] = resourceService->mirror->title;
        rootObject["bitrate"] = resourceService->mirror->bitrate;
        rootObject["resolution"] = resourceService->mirror->resolution;
        rootObject["connectionType"] = resourceService->mirror->connectionType;

    } else if (result == Server::SERVER_RESPONSE::MIRROR_ERROR_START) {
        // error ocure in adb to start scrcpy-server
        resourceService->clearMirrorCash();
    } else if (result == Server::SERVER_RESPONSE::UPDATE_MIRROR_DATA) {
        if (resourceService->mirror->username.length() > 3) {
            rootObject["username"] = resourceService->mirror->username;
            rootObject["title"] = resourceService->mirror->title;
            rootObject["bitrate"] = resourceService->mirror->bitrate;
            rootObject["resolution"] = resourceService->mirror->resolution;
            rootObject["connectionType"] = resourceService->mirror->connectionType;
        } else {
            rootObject["username"] = "nothing";
        }
    }

    QJsonDocument doc(rootObject);
    QString jsonString(doc.toJson(QJsonDocument::Compact));

    for (QWebSocket *pClient: qAsConst(m_clients)) { // send server responseToClents to all androi clients
        pClient->sendTextMessage(jsonString);
    }
}

void WebSocketService::qmlGenerateEvents(QString request, QString data)
{
    if (data.isEmpty()) {
        ADT::ADBDevice *device = ADT::ConnectManager::instance()->cutADBDevice();
        data = device ? device->code() : "";
    }
    if (request == "REQUEST_MIRROR_START") {

        if (resourceService->m_usb_serials.indexOf(data) == -1) {//if not exist add to usb list and execute command
            resourceService->m_usb_serials << data;
        }

        resourceService->setSerial(data);
        resourceService->setUsbMirrorParametre();
        requestMirrorStart();

    } else if (request == "REQUEST_MIRROR_FINISH") {

        qsc::IDeviceManage::getInstance().disconnectDevice(resourceService->serial());
        responseToClents(Server::SERVER_RESPONSE::MIRROR_FINISHED);

    } else if (request == "REQUEST_DEVICES_LIST") {

        if (checkAdbRun()) {
            return;
        }
        qDebug() << "command:adb devices";
        m_adb.execute("", QStringList() << "devices");
    }
}

void WebSocketService::onDeviceConnected(bool success, const QString &serial, const QString &deviceName, const QSize &size) {

    qDebug() << "onDeviceConnected" << serial << deviceName << size.width() << "success:" << success;

    if (success) {
        responseToClents(Server::SERVER_RESPONSE::MIRROR_SUCCESS_START);      // notification to users for start mirroring
        emit resourceService->cppGenerateEvents("MIRROR_START", "");        // update qml ui
        qDebug() << "onDeviceConnected:" << serial;
    }
}

void WebSocketService::onDeviceDisconnected(QString serial) {

    resourceService->clearMirrorCash();
    emit resourceService->cppGenerateEvents("MIRROR_FINISHED", "");

    qDebug() << "onDeviceDisconnected:" << serial;
}

void WebSocketService::requestMirrorStart() {

    qDebug() << "requestMirrorStart()";

    if (!QFile::exists(SCRCPYSERVERPATH)) {
        qWarning() << "scrcpy-server not found";
        NotificationController::instance()->send("连接失败", "scrcpy-server未找到", NotificationController::NotificationType::Warning);
    }

    qWarning() << "scrcpy-server:" << SCRCPYSERVERPATH << QFile::exists(SCRCPYSERVERPATH);

    quint16 videoSize = 1080;
    qsc::DeviceParams params;
    params.serial = resourceService->serial();
    params.maxSize = videoSize;
    params.bitRate = Config::getInstance().getKBitRate() * 1000;
    // on devices with Android >= 10, the capture frame rate can be limited
    params.maxFps = static_cast<quint32>(Config::getInstance().getMaxFps());
    params.closeScreen = false;
    params.useReverse = true;
    params.display = true;
    params.renderExpiredFrames = Config::getInstance().getRenderExpiredFrames();
    params.captureOrientationLock = 1;
    params.captureOrientation = 0;
    params.stayAwake = false;
    params.recordFile = false;
    params.recordPath = Config::getInstance().getRecordOutPath();
    params.recordFileFormat = "mp4";
    params.serverLocalPath = SCRCPYSERVERPATH;
    params.serverRemotePath = Config::getInstance().getServerPath();
    params.pushFilePath = Config::getInstance().getPushFilePath();
    params.gameScript = "";
    params.serverVersion = Config::getInstance().getServerVersion();
    params.logLevel = Config::getInstance().getLogLevel();
    params.codecOptions = Config::getInstance().getCodecOptions();
    params.codecName = Config::getInstance().getCodecName();
    params.scid = QRandomGenerator::global()->bounded(1, 10000) & 0x7FFFFFFF;
    qsc::IDeviceManage::getInstance().connectDevice(params);
}

void WebSocketService::socketDisconnected() {

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    qDebug() << "WebSocket: Socket Disconnected:" << pClient->peerAddress();

    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}


bool WebSocketService::checkAdbRun() {
    if (m_adb.isRuning()) {
        qDebug() << "wait for the end of the current command to run";
    }
    return m_adb.isRuning();
}

void WebSocketService::execute(const QString &serial, const QStringList &args) {
    m_adb.execute(serial, args);
}

QJsonObject WebSocketService::createJsonObject(QString jsonString) {

    QByteArray byteArray;
    byteArray.push_back(jsonString.toLocal8Bit());

    //2. Format the content of the byteArray as QJsonDocument
    //and check on parse Errors
    QJsonParseError parseError;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        //return nullptr_t ;
    }

    //3. Create a jsonObject and fill it with the byteArray content, formatted
    //and holding by the jsonDocument Class
    QJsonObject jsonObj;
    jsonObj = jsonDoc.object();

    return jsonObj;
}
