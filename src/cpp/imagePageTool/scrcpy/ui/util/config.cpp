#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QtQml>

#include "config.h"
#include "src/cpp/utils/globalsetting.h"

#define GROUP_COMMON "common"
#define GROUP_SCRCPY "scrcpy"

// config
#define COMMON_PUSHFILE_KEY "PushFilePath"
#define COMMON_PUSHFILE_DEF "/sdcard/"

#define COMMON_SERVER_VERSION_DEF "3.0.2"

#define COMMON_SERVER_PATH_KEY "ServerPath"
#define COMMON_SERVER_PATH_DEF "/data/local/tmp/scrcpy-server.jar"

#define COMMON_MAX_FPS_KEY "MaxFps"
#define COMMON_MAX_FPS_DEF 60

#define COMMON_RENDER_EXPIRED_FRAMES_KEY "RenderExpiredFrames"
#define COMMON_RENDER_EXPIRED_FRAMES_DEF 0

#define COMMON_ADB_PATH_KEY "AdbPath"
#define COMMON_ADB_PATH_DEF ""

#define COMMON_LOG_LEVEL_KEY "LogLevel"
#define COMMON_LOG_LEVEL_DEF "info"

#define COMMON_CODEC_OPTIONS_KEY "CodecOptions"
#define COMMON_CODEC_OPTIONS_DEF ""

#define COMMON_CODEC_NAME_KEY "CodecName"
#define COMMON_CODEC_NAME_DEF ""

// user config
#define COMMON_RECORD_KEY "RecordPath"

#define COMMON_BITRATE_KEY "BitRate"
#define COMMON_BITRATE_DEF 2000

Config::Config(QObject *parent) : QObject(parent) {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QDir dir(configDir);
    if(!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
    m_settings = new QSettings(configDir + "/config.ini", QSettings::IniFormat);

}

Config &Config::getInstance() {
    static Config config;
    return config;
}

void Config::setProjectPath(QString path) {
    projectPath = path;
}
QString Config::getProjectPath() {
    return projectPath;
}

QString Config::getServerVersion() {
    return COMMON_SERVER_VERSION_DEF;
}

void Config::setMaxFps(int maxFps) {
    maxFps = qBound(10, maxFps, 165);
    AppSettings->writeConfig(GROUP_SCRCPY, COMMON_MAX_FPS_KEY, maxFps);
    emit maxFpsChanged(maxFps);
}

int Config::getMaxFps() const 
{
    int maxFps = AppSettings->readConfig(GROUP_SCRCPY, COMMON_MAX_FPS_KEY, COMMON_MAX_FPS_DEF).toUInt();
    return qBound(10, maxFps, 165);
}

void Config::setKBitRate(int kBitRate)
{
    kBitRate = qBound(100, kBitRate, 100000);    // 0.1Mbps ~ 100Mbps
    AppSettings->writeConfig(GROUP_SCRCPY, COMMON_BITRATE_KEY, kBitRate);
    emit kBitRateChanged(kBitRate);
}
int Config::getKBitRate() const
{
    int kBitRate = AppSettings->readConfig(GROUP_SCRCPY, COMMON_BITRATE_KEY, COMMON_BITRATE_DEF).toUInt();
    return qBound(100, kBitRate, 100000);
}

void Config::setRecordOutPath(const QString &recordOutPath)
{
    AppSettings->writeConfig(GROUP_SCRCPY, COMMON_RECORD_KEY, recordOutPath);
    emit recordOutPathChanged(recordOutPath);
}
QString Config::getRecordOutPath() const
{
    return AppSettings->readConfig(GROUP_SCRCPY, COMMON_RECORD_KEY, QDir::homePath()).toString() + "/Documents";
}

int Config::getRenderExpiredFrames() {
    int renderExpiredFrames = 1;
    m_settings->beginGroup(GROUP_COMMON);
    renderExpiredFrames = m_settings->value(COMMON_RENDER_EXPIRED_FRAMES_KEY, COMMON_RENDER_EXPIRED_FRAMES_DEF).toInt();
    m_settings->endGroup();
    return renderExpiredFrames;
}

QString Config::getPushFilePath() {
    QString pushFile;
    m_settings->beginGroup(GROUP_COMMON);
    pushFile = m_settings->value(COMMON_PUSHFILE_KEY, COMMON_PUSHFILE_DEF).toString();
    m_settings->endGroup();
    return pushFile;
}

QString Config::getServerPath() {
    QString serverPath;
    m_settings->beginGroup(GROUP_COMMON);
    serverPath = m_settings->value(COMMON_SERVER_PATH_KEY, COMMON_SERVER_PATH_DEF).toString();
    m_settings->endGroup();
    return serverPath;
}

QString Config::getAdbPath() {
    QString adbPath;
    m_settings->beginGroup(GROUP_COMMON);
    adbPath = m_settings->value(COMMON_ADB_PATH_KEY, COMMON_ADB_PATH_DEF).toString();
    m_settings->endGroup();
    return adbPath;
}

QString Config::getLogLevel() {
    QString logLevel;
    m_settings->beginGroup(GROUP_COMMON);
    logLevel = m_settings->value(COMMON_LOG_LEVEL_KEY, COMMON_LOG_LEVEL_DEF).toString();
    m_settings->endGroup();
    return logLevel;
}

QString Config::getCodecOptions() {
    QString codecOptions;
    m_settings->beginGroup(GROUP_COMMON);
    codecOptions = m_settings->value(COMMON_CODEC_OPTIONS_KEY, COMMON_CODEC_OPTIONS_DEF).toString();
    m_settings->endGroup();
    return codecOptions;
}

QString Config::getCodecName() {
    QString codecName;
    m_settings->beginGroup(GROUP_COMMON);
    codecName = m_settings->value(COMMON_CODEC_NAME_KEY, COMMON_CODEC_NAME_DEF).toString();
    m_settings->endGroup();
    return codecName;
}

void Config::declareQml()
{
    qmlRegisterSingletonInstance("ScrcpyConfig", 1, 0, "ScrcpyConfig", &Config::getInstance());
}
