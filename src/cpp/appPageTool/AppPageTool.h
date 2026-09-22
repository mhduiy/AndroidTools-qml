#ifndef APPPAGETOOL_H
#define APPPAGETOOL_H

#include <QObject>
#include "src/cpp/utils/defutils.hpp"
#include "softlistmodel.h"

namespace ADT {
class AppPageTool : public QObject
{
    Q_OBJECT
    SINGLETON(AppPageTool)
public:
    ~AppPageTool();

private slots:
    void onADBDeviceChanged();
    void updateAppListInfo();

private:
    SoftListModel *m_softListModel;
};

} // namespace ADT

#endif
