#ifndef TERMINALPAGETOOLS_H
#define TERMINALPAGETOOLS_H

#include <QObject>
#include <qtmetamacros.h>
#include "../utils/singleton.hpp"

class TerminalPageTools : public QObject
{
    Q_OBJECT
    SINGLETON(TerminalPageTools)
public:
    Q_INVOKABLE void startSystemTerminal();
};

#endif
