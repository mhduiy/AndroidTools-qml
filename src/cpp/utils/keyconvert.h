#ifndef KEYCONVERT_H
#define KEYCONVERT_H

#include <QObject>
#include "defutils.hpp"

class KeyConvert : public QObject
{
    Q_OBJECT
    SINGLETON(KeyConvert)
public:
    int convert(Qt::Key key);
};

#endif // KEYCONVERT_H
