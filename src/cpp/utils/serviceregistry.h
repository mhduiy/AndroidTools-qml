#pragma once

#include <QObject>

namespace ADT {
class ConnectManager;
class DeviceHelper;
class ADBTools;
class ControlPageTool;
class AppPageTool;
class FlashPageTool;
class ImagePageTool;
class SettingPageTools;
}

class GlobalSetting;
class NotificationController;
class ADBLogModel;
class AppGlobal;
class KeyConvert;

class TaskThread;

class ServiceRegistry : public QObject
{
    Q_OBJECT
public:
    explicit ServiceRegistry(QObject *parent = nullptr);
    ~ServiceRegistry();

    void initialize();

    TaskThread *taskThread() const { return m_taskThread; }

private:
    void initLeafServices();
    void initADBInfrastructure();
    void initPageTools();
    void initScrcpyServices();

    TaskThread *m_taskThread = nullptr;
};
