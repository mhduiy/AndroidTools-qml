#ifndef DETAILINFOCONTROL_H
#define DETAILINFOCONTROL_H

#include <QObject>
#include "../utils/singleton.hpp"

class DetailInfoControl : public QObject
{
    Q_OBJECT
    SINGLETON(DetailInfoControl)

    Q_PROPERTY(QVariantList info READ getInfo NOTIFY valueChanged)
public:
    void updateInfo();
    QVariantList getInfo() const { return m_info; };
    QString get();

signals:
    void valueChanged();

private:
    QVariantList m_info;
};

#endif // DETAILINFOCONTROL_H
