#ifndef STARTACTIVITY_H
#define STARTACTIVITY_H

#include <QObject>
#include "../utils/defutils.hpp"

namespace ADT {

class StartActivity : public QObject
{
    Q_OBJECT
    SINGLETON(StartActivity)
public:
    Q_INVOKABLE void start(const QString &activity, const QString &argsStr);
};

} // namespace ADT

#endif // STARTACTIVITY_H
