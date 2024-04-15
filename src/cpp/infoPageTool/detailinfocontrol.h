#ifndef DETAILINFOCONTROL_H
#define DETAILINFOCONTROL_H

#include <QObject>
#include "../utils/singleton.hpp"

class DetailInfoControl : public QObject
{
    Q_OBJECT
    SINGLETON(DetailInfoControl)
public:
    void updateInfo();
};

#endif // DETAILINFOCONTROL_H
