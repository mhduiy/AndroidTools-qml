#ifndef DETAILINFOCONTROL_H
#define DETAILINFOCONTROL_H

#include <QObject>
#include "../utils/singleton.hpp"

class DetailInfoUpdateHelper : public QObject
{
    Q_OBJECT
public:
    explicit DetailInfoUpdateHelper(QObject *parent = nullptr);
    void updateInfo();
    QVariantList getInfo() { return m_info; }
    friend class DetailInfoControl;
signals:
    void updateFinish();
private:
    QVariantList m_info;
};

class DetailInfoControl : public QObject
{
    Q_OBJECT
    SINGLETON(DetailInfoControl)
    Q_PROPERTY(QVariantList info READ getInfo NOTIFY valueChanged)
public:

    enum DetailInfoType {
        DETA_MANUFACTURER = 0,
        DETA_BRAND,
        DETA_MODEL,
        DETA_DEVICENAME,
        DETA_DEVICECODE,
        DETA_SYSTEMINFO,
        DETA_ANDROIDVERSION,
        DETA_RESOLVING,
        DETA_DPI,
        DETA_MACADDR,
        DETA_IPADDR,
        DETA_CPUINFO,
        DETA_MEMORY,
        DETA_SDKVERSION,
        DETA_SERIALNUMBER,
        DATE_MAXFREP,
        DATE_MAXCORENUM
    };

    Q_ENUM(DetailInfoType)

    ~DetailInfoControl();
    void updateInfo();
    QVariantList getInfo() const { return m_info; };

signals:
    void valueChanged();

private:
    QVariantList m_info;
    DetailInfoUpdateHelper *m_helper;
    QThread *m_thread;
};

#endif // DETAILINFOCONTROL_H
