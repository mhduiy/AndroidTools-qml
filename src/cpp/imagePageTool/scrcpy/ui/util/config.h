#pragma once

#include <QObject>
#include <QPointer>

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
    QString getServerVersion();
    int getRenderExpiredFrames();
    QString getPushFilePath();
    QString getServerPath();
    QString getAdbPath();
    QString getLogLevel();
    QString getCodecOptions();
    QString getCodecName();

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

private:
    QPointer<QSettings> m_settings;
    QString projectPath;
};
