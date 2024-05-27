#pragma once

#include <QObject>
#include <QPointer>
#include <QRect>
#include <qtmetamacros.h>
#include "src/cpp/utils/globalsetting.h"

struct UserBootConfig {

    QString recordPath = "";

    quint32 bitRate = 500000;
    int maxSizeIndex = 0;
    int recordFormatIndex = 0;
    int lockOrientationIndex = 0;
    bool recordScreen = false;
    bool recordBackground = false;
    bool reverseConnect = true;
    bool showFPS = false;
    bool windowOnTop = false;
    bool autoOffScreen = false;
    bool framelessWindow = false;
    bool keepAlive = false;
    bool simpleMode = false;
    bool autoUpdateDevice = true;
};

class QSettings;

class Config : public QObject 
{
    Q_OBJECT
    Q_PROPERTY(int maxFps READ getMaxFps WRITE setMaxFps NOTIFY maxFpsChanged)
    Q_PROPERTY(int kBitRate READ getKBitRate WRITE setKBitRate NOTIFY kBitRateChanged)
    Q_PROPERTY(QString recordOutPath READ getRecordOutPath WRITE setRecordOutPath NOTIFY recordOutPathChanged)
public:
    static Config &getInstance();

    // config
    QString getTitle();
    QString getServerVersion();
    int getDesktopOpenGL();
    int getSkin();
    int getRenderExpiredFrames();
    QString getPushFilePath();
    QString getServerPath();
    QString getAdbPath();
    QString getLogLevel();
    QString getCodecOptions();
    QString getCodecName();
    QStringList getConnectedGroups();

    // user data:common
    void setUserBootConfig(const UserBootConfig &config);

    UserBootConfig getUserBootConfig();

    // user data:device
    void setNickName(const QString &serial, const QString &name);
    QString getNickName(const QString &serial);
    void setRect(const QString &serial, const QRect &rc);
    QRect getRect(const QString &serial);

    void deleteGroup(const QString &serial);

    static void declareQml();

    void setProjectPath(QString path);
    QString getProjectPath();

    void setMaxFps(int maxFps);
    int getMaxFps() const;

    void setKBitRate(int kBitRate);
    int getKBitRate() const;

    void setRecordOutPath(const QString &recordOutPath);
    QString getRecordOutPath() const;

signals:
    void maxFpsChanged(int maxFps);
    void kBitRateChanged(int bitRate);
    void recordOutPathChanged(const QString &recordOutPath);

private:
    explicit Config(QObject *parent = nullptr);

    void initConfig();

private:
    static QString s_configPath;
    QPointer<QSettings> m_settings;
    QPointer<QSettings> m_userData;
    QPointer<GlobalSetting> m_globalSetting;
    QString projectPath;
};
