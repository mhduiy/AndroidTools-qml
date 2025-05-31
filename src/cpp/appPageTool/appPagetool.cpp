#include "appPagetool.h"
#include "appDetailControl.h"

#include "src/cpp/utils/utils.hpp"
#include "src/cpp/adb/connectmanager.h"

#include <QtQml>
#include <QDebug>
#include <QMetaObject>

namespace ADT {

AppPageTool::AppPageTool(QObject *parent)
: QObject(parent)
, m_softListModel(new SoftListModel(this))
{
    qmlRegisterSingletonInstance("SoftListModel", 1, 0, "SoftListModel", m_softListModel);
    qmlRegisterSingletonInstance("AppDetailControl", 1, 0, "AppDetailControl", AppDetailControl::instance(this));

    connect(AppDetailControl::instance(), &AppDetailControl::softListTypeChanged, this, &AppPageTool::updateAppListInfo);
    connect(ConnectManager::instance(), &ConnectManager::cutADBDeviceChanged, this, &AppPageTool::onADBDeviceChanged);

    initData();
}

AppPageTool::~AppPageTool()
{

}

void AppPageTool::initData()
{
    updateAppListInfo();
}

void AppPageTool::onADBDeviceChanged()
{
    m_softListModel->clearData();
    updateAppListInfo();
}

void AppPageTool::updateAppListInfo()
{
    m_softListModel->clearData();
    std::function<void()> update = [this](){
        auto device = ConnectManager::instance()->cutADBDevice();
        if (!device) {
            return;
        }

        auto infos = device->getSoftListInfo();
        
        // 将模型更新操作调度回主线程执行
        QMetaObject::invokeMethod(this, [this, infos]() {
            for (const auto &info : infos) {
                m_softListModel->appendRow(info);
            }
        }, Qt::QueuedConnection);
    };

    asyncOperator(update);
}

} // namespace ADT