#pragma once

#include <QObject>
#include <QPointer>
#include <QSize>

#include "adbprocess.h"
#include "tcpserver.h"
#include "videosocket.h"

class Server : public QObject
{
    Q_OBJECT

    enum SERVER_START_STEP
    {
        SSS_NULL,
        SSS_PUSH,
        SSS_ENABLE_TUNNEL_REVERSE,
        SSS_ENABLE_TUNNEL_FORWARD,
        SSS_EXECUTE_SERVER,
        SSS_RUNNING,
    };

public:
    struct ServerParams
    {
        // necessary
        QString serial = "";              // devise serial number
        QString serverLocalPath = "";     // Local Android server path


        // optional
        QString serverRemotePath = "/data/local/tmp/scrcpy-server.jar";    // The server path to be pushed to the remote device
        quint16 localPort = 27183;     // Local listening port in reverse
        quint16 maxSize = 720;         // video resolution
        quint32 bitRate = 8000000;     // video bitrate
        quint32 maxFps = 0;            // Video Maximum Frame Rate
        bool useReverse = true;        // true: use adb reverse first, and automatically use adb forward after failure; false: use adb forward directly
        int lockVideoOrientation = -1; // Whether to lock the video orientation
        int stayAwake = false;         // whether to stay awake
        QString serverVersion = "1.24";// server version
        QString logLevel = "debug";  // log level verbose/debug/info/warn/error
        // Encoding option "" means default
        // For example CodecOptions="profile=1,level=2"
        // More encoding options reference https://d.android.com/reference/android/media/MediaFormat
        QString codecOptions = "";
        // Specify the encoder name (must be H.264 encoder), "" means the default
        // For example CodecName="OMX.qcom.video.encoder.avc"
        QString codecName = "";

        QString crop = "";             // video cropping
        bool control = true;           // Whether the Android terminal accepts keyboard and mouse control
    };

    explicit Server(QObject *parent = nullptr);
    virtual ~Server();

    bool start(Server::ServerParams params);
    void stop();
    bool isReverse();
    Server::ServerParams getParams();
    VideoSocket *removeVideoSocket();
    QTcpSocket *getControlSocket();

signals:
    void serverStarted(bool success, const QString &deviceName = "", const QSize &size = QSize());
    void serverStoped();

private slots:
    void onWorkProcessResult(qsc::AdbProcess::ADB_EXEC_RESULT processResult);
    void onServerProcessResult(qsc::AdbProcess::ADB_EXEC_RESULT processResult);

protected:
    void timerEvent(QTimerEvent *event);

private:
    bool pushServer();
    bool enableTunnelReverse();
    bool disableTunnelReverse();
    bool enableTunnelForward();
    bool disableTunnelForward();
    bool execute();
    bool connectTo();
    bool startServerByStep();
    bool readInfo(VideoSocket *videoSocket, QString &deviceName, QSize &size);
    void startAcceptTimeoutTimer();
    void stopAcceptTimeoutTimer();
    void startConnectTimeoutTimer();
    void stopConnectTimeoutTimer();
    void onConnectTimer();

private:
    qsc::AdbProcess m_workProcess;
    qsc::AdbProcess m_serverProcess;
    TcpServer m_serverSocket; // only used if !tunnel_forward
    QPointer<VideoSocket> m_videoSocket = Q_NULLPTR;
    QPointer<QTcpSocket> m_controlSocket = Q_NULLPTR;

    bool m_tunnelEnabled = false;
    bool m_tunnelForward = false; // use "adb forward" instead of "adb reverse"
    int m_acceptTimeoutTimer = 0;
    int m_connectTimeoutTimer = 0;
    quint32 m_connectCount = 0;
    quint32 m_restartCount = 0;
    QString m_deviceName = "";
    QSize m_deviceSize = QSize();
    ServerParams m_params;

    SERVER_START_STEP m_serverStartStep = SSS_NULL;
};

