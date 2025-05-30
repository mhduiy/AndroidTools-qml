// #ifndef ADBINTERFACE_H
// #define ADBINTERFACE_H

// #include <QObject>
// #include "adbtools.h"
// #include <QMap>
// #include <QTimer>
// #include <QSharedPointer>
// #include "../utils/defutils.hpp"



// class ADBInterface : public QObject
// {
//     Q_OBJECT
//     SINGLETON(ADBInterface)
// public slots:
//     bool startADBService();
//     bool stopADBService();
//     bool restartADBService();
// public:

//     QString adbVersion() const;
//     QVector<QString> getDeviceCodes() const;
//     DeviceBatteryInfo getBatteryInfo(const QString &code) const;
//     DeviceCutActivityInfo getCutActivityInfo(const QString &code) const;
//     DeviceDetailInfo getDeviceDetailInfo(const QString &code);
//     void killActivity(const QString &packageName, const QString &deviceCode = "");
//     void pushKey(const QString &key, const QString &deviceCode);
//     void startActivity(const QString &deviceCode, const QString &activity, const QStringList &args);
//     QList<AppListInfo> getSoftListInfo(const QString &deviceCode);
//     AppDetailInfo getAppDetailInfo(const QString &deviceCode, const QString &packageName);

//     void shotScreen(const QString &deviceCode, const QString &outPath);
//     void disConnectDeivce(const QString &deviceCode);
// signals:
//     void adbStarted();
//     void adbKilled();

// private:
//     QString getDeviceProp(const QString &deviceCode, const QString &prop);

// private:
//     ADBTools *m_adbTools = ADBTools::instance();
//     QString m_adbVersion = "";
// };

// #endif // ADBINTERFACE_H
