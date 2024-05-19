#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QTimerEvent>

#include "server.h"

#define DEVICE_NAME_FIELD_LENGTH 64
#define SOCKET_NAME "scrcpy"
#define MAX_CONNECT_COUNT 30
#define MAX_RESTART_COUNT 1

Server::Server(QObject *parent) : QObject(parent) {

    connect(&m_workProcess, &qsc::AdbProcess::adbProcessResult, this, &Server::onWorkProcessResult);
    connect(&m_serverProcess, &qsc::AdbProcess::adbProcessResult, this, &Server::onServerProcessResult);

    connect(&m_serverSocket, &QTcpServer::newConnection, this, [this]() {
        QTcpSocket *tmp = m_serverSocket.nextPendingConnection();
        if (dynamic_cast<VideoSocket *>(tmp)) {
            m_videoSocket = dynamic_cast<VideoSocket *>(tmp);
            if (!m_videoSocket->isValid() || !readInfo(m_videoSocket, m_deviceName, m_deviceSize)) {
                stop();
                emit serverStarted(false);
            }
        } else {
            m_controlSocket = tmp;
            if (m_controlSocket && m_controlSocket->isValid()) {
                // we don't need the server socket anymore
                // just m_videoSocket is ok
                m_serverSocket.close();
                // we don't need the adb tunnel anymore
                disableTunnelReverse();
                m_tunnelEnabled = false;
                emit serverStarted(true, m_deviceName, m_deviceSize);
            } else {
                stop();
                emit serverStarted(false);
            }
            stopAcceptTimeoutTimer();
        }
    });
}

Server::~Server() {}

bool Server::pushServer() {
    if (m_workProcess.isRuning()) {
        m_workProcess.kill();
    }
    m_workProcess.push(m_params.serial, m_params.serverLocalPath, m_params.serverRemotePath);
    return true;
}

bool Server::enableTunnelReverse() {
    if (m_workProcess.isRuning()) {
        m_workProcess.kill();
    }
    m_workProcess.reverse(m_params.serial, SOCKET_NAME, m_params.localPort);
    return true;
}

bool Server::disableTunnelReverse() {
    qsc::AdbProcess *adb = new qsc::AdbProcess();
    if (!adb) {
        return false;
    }
    connect(adb, &qsc::AdbProcess::adbProcessResult, this, [this](qsc::AdbProcess::ADB_EXEC_RESULT processResult) {
        if (qsc::AdbProcess::AER_SUCCESS_START != processResult) {
            sender()->deleteLater();
        }
    });
//    adb->reverseRemove(m_params.serial, SOCKET_NAME);
    return true;
}

bool Server::enableTunnelForward() {
    if (m_workProcess.isRuning()) {
        m_workProcess.kill();
    }
    m_workProcess.forward(m_params.serial, m_params.localPort, SOCKET_NAME);
    return true;
}

bool Server::disableTunnelForward() {
    qsc::AdbProcess *adb = new qsc::AdbProcess();
    if (!adb) {
        return false;
    }
    connect(adb, &qsc::AdbProcess::adbProcessResult, this, [this](qsc::AdbProcess::ADB_EXEC_RESULT processResult) {
        if (qsc::AdbProcess::AER_SUCCESS_START != processResult) {
            sender()->deleteLater();
        }
    });
    adb->forwardRemove(m_params.serial, m_params.localPort);
    return true;
}

bool Server::execute() {
    if (m_serverProcess.isRuning()) {
        m_serverProcess.kill();
    }
    QStringList args;
    args << "shell";
    args << QString("CLASSPATH=%1").arg(m_params.serverRemotePath);
    args << "app_process";

#ifdef SERVER_DEBUGGER
#define SERVER_DEBUGGER_PORT "5005"

    args <<
#ifdef SERVER_DEBUGGER_METHOD_NEW
        /* Android 9 and above */
        "-XjdwpProvider:internal -XjdwpOptions:transport=dt_socket,suspend=y,server=y,address="
#else
        /* Android 8 and below */
        "-agentlib:jdwp=transport=dt_socket,suspend=y,server=y,address="
#endif
        SERVER_DEBUGGER_PORT,
#endif

    args << "/"; // unused;
    args << "com.genymobile.scrcpy.Server";
    args << m_params.serverVersion;

    args << QString("bit_rate=%1").arg(QString::number(m_params.bitRate));
    if (!m_params.logLevel.isEmpty()) {
        args << QString("log_level=%1").arg(m_params.logLevel);
    }
    if (m_params.maxSize > 0) {
        args << QString("max_size=%1").arg(QString::number(m_params.maxSize));
    }
    if (m_params.maxFps > 0) {
        args << QString("max_fps=%1").arg(QString::number(m_params.maxFps));
    }
    if (-1 != m_params.lockVideoOrientation) {
        args << QString("lock_video_orientation=%1").arg(QString::number(m_params.lockVideoOrientation));
    }
    if (m_tunnelForward) {
        args << QString("tunnel_forward=true");
    }
    if (!m_params.crop.isEmpty()) {
        args << QString("crop=%1").arg(m_params.crop);
    }
    if (!m_params.control) {
        args << QString("control=false");
    }
    // The default is 0, no need to set
    // args << "display_id=0";
    // The default is false, no need to set
    // args << "show_touches=false";
    if (m_params.stayAwake) {
        args << QString("stay_awake=true");
    }
    // code option
    // https://github.com/Genymobile/scrcpy/commit/080a4ee3654a9b7e96c8ffe37474b5c21c02852a
    // <https://d.android.com/reference/android/media/MediaFormat>
    if (!m_params.codecOptions.isEmpty()) {
        args << QString("codec_options=%1").arg(m_params.codecOptions);
    }
    if (!m_params.codecName.isEmpty()) {
        args << QString("encoder_name=%1").arg(m_params.codecName);
    }
    // The default is false, no need to set
    // args << "power_off_on_close=false";

    // The following parameters can use the default value of the server to minimize the parameter transmission, and the parameter transmission is too long,
    // which will cause the Samsung mobile phone to report an error：stack corruption detected (-fstack-protector)
    /*
    args << "clipboard_autosync=true";    
    args << "downsize_on_error=true";
    args << "cleanup=true";
    args << "power_on=true";
    
    args << "send_device_meta=true";
    args << "send_frame_meta=true";
    args << "send_dummy_byte=true";
    args << "raw_video_stream=false";
    */

#ifdef SERVER_DEBUGGER
    qInfo("Server debugger waiting for a client on device port " SERVER_DEBUGGER_PORT "...");
    // From the computer, run
    //     adb forward tcp:5005 tcp:5005
    // Then, from Android Studio: Run > Debug > Edit configurations...
    // On the left, click on '+', "Remote", with:
    //     Host: localhost
    //     Port: 5005
    // Then click on "Debug"
#endif

    // adb -s P7C0218510000537 shell CLASSPATH=/data/local/tmp/scrcpy-server app_process / com.genymobile.scrcpy.Server 0 8000000 false
    // mark: crop input format: "width:height:x:y" or "" for no crop, for example: "100:200:0:0"
    // This adb command is blocked, and the m_serverProcess process will not exit
    m_serverProcess.execute(m_params.serial, args);
    return true;
}

bool Server::start(Server::ServerParams params) {
    m_params = params;
    m_serverStartStep = SSS_PUSH;
    return startServerByStep();
}

bool Server::connectTo() {
    if (SSS_RUNNING != m_serverStartStep) {
        qWarning("server not run");
        return false;
    }

    if (!m_tunnelForward && !m_videoSocket) {
        startAcceptTimeoutTimer();
        return true;
    }

    startConnectTimeoutTimer();
    return true;
}

bool Server::isReverse() {
    return !m_tunnelForward;
}

Server::ServerParams Server::getParams() {
    return m_params;
}

void Server::timerEvent(QTimerEvent *event) {
    if (event && m_acceptTimeoutTimer == event->timerId()) {
        stopAcceptTimeoutTimer();
        emit serverStarted(false);
    } else if (event && m_connectTimeoutTimer == event->timerId()) {
        onConnectTimer();
    }
}

VideoSocket *Server::removeVideoSocket() {
    VideoSocket *socket = m_videoSocket;
    m_videoSocket = Q_NULLPTR;
    return socket;
}

QTcpSocket *Server::getControlSocket() {
    return m_controlSocket;
}

void Server::stop() {
    if (m_tunnelForward) {
        stopConnectTimeoutTimer();
    } else {
        stopAcceptTimeoutTimer();
    }

    if (m_controlSocket) {
        m_controlSocket->close();
        m_controlSocket->deleteLater();
    }
    // ignore failure
    m_serverProcess.kill();
    if (m_tunnelEnabled) {
        if (m_tunnelForward) {
            disableTunnelForward();
        } else {
            disableTunnelReverse();
        }
        m_tunnelForward = false;
        m_tunnelEnabled = false;
    }
    m_serverSocket.close();
}

bool Server::startServerByStep() {
    bool stepSuccess = false;
    // push, enable tunnel et start the server
    if (SSS_NULL != m_serverStartStep) {
        switch (m_serverStartStep) {
            case SSS_PUSH:
                stepSuccess = pushServer();
                break;
            case SSS_ENABLE_TUNNEL_REVERSE:
                stepSuccess = enableTunnelReverse();
                break;
            case SSS_ENABLE_TUNNEL_FORWARD:
                stepSuccess = enableTunnelForward();
                break;
            case SSS_EXECUTE_SERVER:
                // server will connect to our server socket
                stepSuccess = execute();
                break;
            default:
                break;
        }
    }

    if (!stepSuccess) {
        emit serverStarted(false);
    }
    return stepSuccess;
}

bool Server::readInfo(VideoSocket *videoSocket, QString &deviceName, QSize &size) {
    unsigned char buf[DEVICE_NAME_FIELD_LENGTH + 4];
    if (videoSocket->bytesAvailable() <= (DEVICE_NAME_FIELD_LENGTH + 4)) {
        videoSocket->waitForReadyRead(300);
    }

    qint64 len = videoSocket->read((char *) buf, sizeof(buf));
    if (len < DEVICE_NAME_FIELD_LENGTH + 4) {
        qInfo("Could not retrieve device information");
        return false;
    }
    buf[DEVICE_NAME_FIELD_LENGTH - 1] = '\0'; // in case the client sends garbage
    // strcpy is safe here, since name contains at least DEVICE_NAME_FIELD_LENGTH bytes
    // and strlen(buf) < DEVICE_NAME_FIELD_LENGTH
    deviceName = (char *) buf;
    size.setWidth((buf[DEVICE_NAME_FIELD_LENGTH] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 1]);
    size.setHeight((buf[DEVICE_NAME_FIELD_LENGTH + 2] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 3]);
    return true;
}

void Server::startAcceptTimeoutTimer() {
    stopAcceptTimeoutTimer();
    m_acceptTimeoutTimer = startTimer(1000);
}

void Server::stopAcceptTimeoutTimer() {
    if (m_acceptTimeoutTimer) {
        killTimer(m_acceptTimeoutTimer);
        m_acceptTimeoutTimer = 0;
    }
}

void Server::startConnectTimeoutTimer() {
    stopConnectTimeoutTimer();
    m_connectTimeoutTimer = startTimer(300);
}

void Server::stopConnectTimeoutTimer() {
    if (m_connectTimeoutTimer) {
        killTimer(m_connectTimeoutTimer);
        m_connectTimeoutTimer = 0;
    }
    m_connectCount = 0;
}

void Server::onConnectTimer() {
    // device server need time to start
    // The connection here is too early and the time is not enough, so the Android listening socket has not been established yet,
    // and readInfo will fail, so a timing retry strategy is adopted
    // Tried every 100ms, up to MAX_CONNECT_COUNT attempts
    QString deviceName;
    QSize deviceSize;
    bool success = false;

    VideoSocket *videoSocket = new VideoSocket();
    QTcpSocket *controlSocket = new QTcpSocket();

    videoSocket->connectToHost(QHostAddress::LocalHost, m_params.localPort);

    if (!videoSocket->waitForConnected(1000)) {
        // Connecting to adb is very fast, and there is no retry if there is a failure here
        m_connectCount = MAX_CONNECT_COUNT;
        qWarning("video socket connect to server failed");
        goto result;
    }

    controlSocket->connectToHost(QHostAddress::LocalHost, m_params.localPort);

    if (!controlSocket->waitForConnected(1000)) {
        // Connecting to adb is very fast, and there is no retry if there is a failure here
        m_connectCount = MAX_CONNECT_COUNT;
        qWarning("control socket connect to server failed");
        goto result;
    }

    if (QTcpSocket::ConnectedState == videoSocket->state()) {
        // connect will success even if devices offline, recv data is real connect success
        // because connect is to pc adb server
        videoSocket->waitForReadyRead(1000);
        // devices will setClients 1 byte first on tunnel forward mode
        QByteArray data = videoSocket->read(1);
        if (!data.isEmpty() && readInfo(videoSocket, deviceName, deviceSize)) {
            success = true;
            goto result;
        } else {
            qWarning("video socket connect to server read device info failed, try again");
            goto result;
        }
    } else {
        qWarning("connect to server failed");
        m_connectCount = MAX_CONNECT_COUNT;
        goto result;
    }

    result:
    if (success) {
        stopConnectTimeoutTimer();
        m_videoSocket = videoSocket;
        m_controlSocket = controlSocket;
        // we don't need the adb tunnel anymore
        disableTunnelForward();
        m_tunnelEnabled = false;
        m_restartCount = 0;
        emit serverStarted(success, deviceName, deviceSize);
        return;
    }

    if (videoSocket) {
        videoSocket->deleteLater();
    }
    if (controlSocket) {
        controlSocket->deleteLater();
    }

    if (MAX_CONNECT_COUNT <= m_connectCount++) {
        stopConnectTimeoutTimer();
        stop();
        if (MAX_RESTART_COUNT > m_restartCount++) {
            qWarning("restart server auto");
            start(m_params);
        } else {
            m_restartCount = 0;
            emit serverStarted(false);
        }
    }
}

void Server::onWorkProcessResult(qsc::AdbProcess::ADB_EXEC_RESULT processResult) {
    if (sender() == &m_workProcess) {
        if (SSS_NULL != m_serverStartStep) {
            switch (m_serverStartStep) {
                case SSS_PUSH:
                    if (qsc::AdbProcess::AER_SUCCESS_EXEC == processResult) {
                        if (m_params.useReverse) {
                            m_serverStartStep = SSS_ENABLE_TUNNEL_REVERSE;
                        } else {
                            m_tunnelForward = true;
                            m_serverStartStep = SSS_ENABLE_TUNNEL_FORWARD;
                        }
                        startServerByStep();
                    } else if (qsc::AdbProcess::AER_SUCCESS_START != processResult) {
                        qCritical("adb push failed");
                        m_serverStartStep = SSS_NULL;
                        emit serverStarted(false);
                    }
                    break;
                case SSS_ENABLE_TUNNEL_REVERSE:
                    qWarning("SSS_ENABLE_TUNNEL_REVERSE");
                    if (qsc::AdbProcess::AER_SUCCESS_EXEC == processResult) {
                        // At the application level, the device part is "the server" because it
                        // serves video stream and control. However, at the network level, the
                        // client listens and the server connects to the client. That way, the
                        // client can listen before starting the server app, so there is no need to
                        // try to connect until the server socket is listening on the device.
                        m_serverSocket.setMaxPendingConnections(5);
                        if (!m_serverSocket.listen(QHostAddress::LocalHost, m_params.localPort)) {
                            qCritical() << QString("Could not listen on port %1").arg(
                                    m_params.localPort).toStdString().c_str();
                            m_serverStartStep = SSS_NULL;
                            disableTunnelReverse();
                            emit serverStarted(false);
                            break;
                        }

                        m_serverStartStep = SSS_EXECUTE_SERVER;
                        startServerByStep();
                    } else if (qsc::AdbProcess::AER_SUCCESS_START != processResult) {
                        // Some devices reverse will report more than one device, adb bug
                        // https://github.com/Genymobile/scrcpy/issues/5
                        qCritical("adb reverse failed");
                        m_tunnelForward = true;
                        m_serverStartStep = SSS_ENABLE_TUNNEL_FORWARD;
                        startServerByStep();
                    }
                    break;
                case SSS_ENABLE_TUNNEL_FORWARD:
                    qWarning("SSS_ENABLE_TUNNEL_FORWARD");
                    if (qsc::AdbProcess::AER_SUCCESS_EXEC == processResult) {
                        m_serverStartStep = SSS_EXECUTE_SERVER;
                        startServerByStep();
                    } else if (qsc::AdbProcess::AER_SUCCESS_START != processResult) {
                        qCritical("adb forward failed");
                        m_serverStartStep = SSS_NULL;
                        emit serverStarted(false);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Server::onServerProcessResult(qsc::AdbProcess::ADB_EXEC_RESULT processResult) {
    if (sender() == &m_serverProcess) {
        qDebug() << "m_serverProcess";
        if (SSS_EXECUTE_SERVER == m_serverStartStep) {
            if (qsc::AdbProcess::AER_SUCCESS_START == processResult) {
                m_serverStartStep = SSS_RUNNING;
                m_tunnelEnabled = true;
                connectTo();
            } else if (qsc::AdbProcess::AER_ERROR_START == processResult) {
                if (!m_tunnelForward) {
                    m_serverSocket.close();
                    disableTunnelReverse();
                } else {
                    disableTunnelForward();
                }
                qCritical("adb shell start server failed");
                m_serverStartStep = SSS_NULL;
                emit serverStarted(false);
            }
        } else if (SSS_RUNNING == m_serverStartStep) {
            m_serverStartStep = SSS_NULL;
            emit serverStoped();
        }
    }
}
