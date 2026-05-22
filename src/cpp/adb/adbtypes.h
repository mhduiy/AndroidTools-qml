#pragma once

#include <QString>

namespace ADT {

struct NetResult {
    bool success = false;
    QString data;
};

struct CommandResult {
    bool success = false;
    int exitCode = -1;
    QString output;
    QString errorOutput;
    QString command;
    int executionTime = 0;

    CommandResult() = default;
    CommandResult(bool _success, int _exitCode, const QString& _output,
                    const QString& _errorOutput = "", const QString& _command = "",
                    int _executionTime = 0)
        : success(_success), exitCode(_exitCode), output(_output)
        , errorOutput(_errorOutput), command(_command), executionTime(_executionTime) {}

    bool isSuccess() const { return success && exitCode == 0; }
    QString getAllOutput() const {
        return output.isEmpty() ? errorOutput : output;
    }
};

struct DeviceActivityInfo {
    QString windowCode;
    QString cutPackage;
    QString cutActivity;
};

struct DeviceDetailInfo {
    QString manufacturer;
    QString brand;
    QString model;
    QString deviceName;
    QString deviceCode;
    QString systemInfo;
    QString androidVersion;
    QString resolving;
    QString dpi;
    QString macAddr;
    QString ipAddr;
    QString cpuInfo;
    QString memory;
    QString sdkVersion;
    QString serialNumber;
    QString maxFrep;
    QString maxCoreNum;
};

struct AppDetailInfo {
    QString packageName;
    QString appName;
    QString versionName;
    QString iconBase64;
    quint64 versionCode;
    bool isSystemApp;
    bool isEnabled;
    QString firstInstallTime;
    QString lastUpdateTime;
    QString targetsdk;
    QString minsdk;
    QString appid;
    QString path;
};

} // namespace ADT
