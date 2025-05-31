#ifndef CONTROLPAGETOOL_H
#define CONTROLPAGETOOL_H

#include <QtQml>
#include <QObject>
#include "../utils/defutils.hpp"

namespace ADT {

class ControlPageTool : public QObject
{
    Q_OBJECT
    SINGLETON(ControlPageTool)
public:

};

} // namespace ADT

#endif
