#ifndef APPPAGETOOL_H
#define APPPAGETOOL_H

#include <QObject>
#include <QThread>
#include "src/cpp/utils/defutils.hpp"
#include "src/cpp/adb/adbdevice.h"
#include "softlistmodel.h"

namespace ADT {
class AppPageTool : public QObject
{
    Q_OBJECT
    SINGLETON(AppPageTool)
public:
    ~AppPageTool();

private:
    void initData();

private slots:
    void onADBDeviceChanged();
    void updateAppListInfo();

private:
    SoftListModel *m_softListModel;
};

} // namespace ADT

#endif
