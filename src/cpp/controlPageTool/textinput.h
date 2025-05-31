#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QObject>
#include "../utils/defutils.hpp"

namespace ADT {

class InputText : public QObject
{
    Q_OBJECT
    SINGLETON(InputText)

public:
    Q_INVOKABLE void pushKey(Qt::Key);
};

} // namespace ADT

#endif // TEXTINPUT_H
