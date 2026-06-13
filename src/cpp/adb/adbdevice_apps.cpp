#include "adbdevice.h"
#include "src/cpp/adb/adbtools.h"
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

namespace ADT {

QList<AppDetailInfo> ADBDevice::getSoftListInfo(SoftListType type) const
{
    QUrl url("http://localhost:18888/apps");
    if (type == SoftListType::ThirdParty) {
        url.setQuery(QUrlQuery("isUser=true"));
    } else if (type == SoftListType::System) {
        url.setQuery(QUrlQuery("isSystem=true"));
    }

    auto ret = syncCallNetGetMethod(url);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ret.data.toUtf8());
    if (!jsonDoc.isArray()) {
        qWarning() << "Invalid JSON response:" << ret.data;
        return {};
    }
    QJsonArray jsonArray = jsonDoc.array();
    QList<AppDetailInfo> appList;
    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            AppDetailInfo info;
            info.packageName = obj.value("packageName").toString();
            info.appName = obj.value("appName").toString();
            info.versionName = obj.value("versionName").toString();
            info.versionCode = obj.value("versionCode").toInt();
            info.isSystemApp = obj.value("isSystemApp").toBool();
            info.isEnabled = obj.value("isEnabled").toBool();
            info.firstInstallTime = QDateTime::fromMSecsSinceEpoch(obj.value("firstInstallTime").toVariant().toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            info.lastUpdateTime = QDateTime::fromMSecsSinceEpoch(obj.value("lastUpdateTime").toVariant().toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            info.iconBase64 = "";
            appList.append(info);
        }
    }
    return appList;
}

QString ADBDevice::getAppIconBase64(const QString &packageName) const
{
    QUrl url("http://localhost:18888/appIcon");
    url.setQuery(QUrlQuery("packageName=" + packageName));

    auto ret = syncCallNetGetMethod(url);
    if (!ret.success) {
        qWarning() << "Failed to get app icon:" << ret.data;
        return {};
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(ret.data.toUtf8());
    if (!jsonDoc.isObject()) {
        qWarning() << "Invalid JSON response for app icon:" << ret.data;
        return {};
    }

    QString iconBase64 = jsonDoc.object().value("iconBase64").toString();
    if (iconBase64.isEmpty() || iconBase64.startsWith("data:")) {
        return iconBase64;
    }

    return "data:image/png;base64," + iconBase64;
}

bool ADBDevice::installApp(const QString &path, bool r, bool s, bool d, bool g)
{
    QStringList args;
    args << "-s" << code() << "install";

    if (r) args << "-r";
    if (s) args << "-s";
    if (d) args << "-d";
    if (g) args << "-g";

    args << path;

    CommandResult result = m_adbTools->executeCommandDetailed(ADBTools::ADB, args, "", INT_MAX);

    qDebug() << "Install App Command:" << result.command;
    qDebug() << "Success:" << result.success;
    qDebug() << "Exit Code:" << result.exitCode;
    qDebug() << "Execution Time:" << result.executionTime << "ms";
    if (!result.errorOutput.isEmpty()) {
        qDebug() << "Error Output:" << result.errorOutput;
    }

    return result.isSuccess() && result.output.contains("Success");
}

bool ADBDevice::clearData(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "pm" << "clear" << packageName;
    CommandResult result = m_adbTools->executeCommandDetailed(ADBTools::ADB, args, "", INT_MAX);

    qDebug() << "Clear Data Command:" << result.command;
    qDebug() << "Success:" << result.success << "Exit Code:" << result.exitCode;
    if (!result.errorOutput.isEmpty()) {
        qDebug() << "Error Output:" << result.errorOutput;
    }

    return result.isSuccess() && result.output.contains("Success");
}

bool ADBDevice::uninstallApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "uninstall" << packageName;
    CommandResult result = m_adbTools->executeCommandDetailed(ADBTools::ADB, args, "", INT_MAX);

    qDebug() << "Uninstall App Command:" << result.command;
    qDebug() << "Success:" << result.success << "Exit Code:" << result.exitCode;
    if (!result.errorOutput.isEmpty()) {
        qDebug() << "Error Output:" << result.errorOutput;
    }

    return result.isSuccess() && result.output.contains("Success");
}

bool ADBDevice::freezeApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "pm" << "disable-user" << packageName;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return !result.contains("Error");
}

bool ADBDevice::unfreezeApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "pm" << "enable" << packageName;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return !result.contains("Error");
}

bool ADBDevice::extractApp(const QString &packageName, const QString &targetPath)
{
    QStringList pathArgs;
    pathArgs << "-s" << code() << "shell" << "pm" << "path" << packageName;
    QString pathResult = m_adbTools->executeCommand(ADBTools::ADB, pathArgs);

    if (pathResult.isEmpty() || !pathResult.contains("package:")) {
        return false;
    }

    QString apkPath = pathResult.simplified().split(':').value(1);
    if (apkPath.isEmpty()) {
        return false;
    }

    QStringList args;
    args << "-s" << code() << "pull" << apkPath << targetPath;
    QString result = m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
    return !result.contains("failed");
}

void ADBDevice::startApp(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "monkey" << "-p" << packageName
         << "-c" << "android.intent.category.LAUNCHER" << "1";
    m_adbTools->executeCommand(ADBTools::ADB, args, "", INT_MAX);
}

bool ADBDevice::stopApp(const QString &packageName)
{
    killActivity(packageName);
    return true;
}

void ADBDevice::killActivity(const QString &packageName)
{
    QStringList args;
    args << "-s" << code() << "shell" << "am" << "force-stop" << packageName;
    m_adbTools->executeCommand(ADBTools::ADB, args);
}

void ADBDevice::startActivity(const QString &activity, const QStringList &args)
{
    QStringList adbArgs;
    adbArgs << "-s" << code() << "shell" << "am" << "start" << "-n" << activity << args;
    m_adbTools->executeCommand(ADBTools::ADB, adbArgs);
}

} // namespace ADT
