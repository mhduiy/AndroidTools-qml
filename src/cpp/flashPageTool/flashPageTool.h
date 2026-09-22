#ifndef FLASHPAGETOOL_H
#define FLASHPAGETOOL_H

#include <QObject>
#include "../utils/defutils.hpp"

namespace ADT {

class FlashPageTool : public QObject
{
    Q_OBJECT
    SINGLETON(FlashPageTool)
};

} // namespace ADT

#endif
