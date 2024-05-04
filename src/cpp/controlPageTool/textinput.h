#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QObject>
#include "../utils/singleton.hpp"

class InputText : public QObject
{
    Q_OBJECT
    SINGLETON(InputText)

public:
    Q_INVOKABLE void pushKey(Qt::Key);
};

#endif // TEXTINPUT_H
