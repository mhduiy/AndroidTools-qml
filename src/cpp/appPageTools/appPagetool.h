#ifndef APPPAGETOOL_H
#define APPPAGETOOL_H

#include <QObject>
#include <QThread>
#include "../utils/singleton.hpp"
#include "softlistmodel.h"

class AppInfoHandle : public QObject
{
    Q_OBJECT
public:
    explicit AppInfoHandle(QObject *parent = nullptr);
    QList<AppListInfo> getInfo();
    void updateListInfo();
    // void updatePackageInfo();

signals:
    void updateListFinish();
    void updatePackageInfoFinish();

private:
    QList<AppListInfo> m_info;
};

class AppPageTool : public QObject
{
    Q_OBJECT
    SINGLETON(AppPageTool)
public:

private:
void initData();

private:
    void updateAppListInfo();

private:
    SoftListModel *m_softListModel;
    AppInfoHandle *m_appHandle;
    QThread *m_appHandleThread;
};

#endif
