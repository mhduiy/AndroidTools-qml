#ifndef FLASHTOOLS_H
#define FLASHTOOLS_H

#include <QObject>
#include "../utils/defutils.hpp"

namespace ADT {

class FlashTools : public QObject
{
    Q_OBJECT
    SINGLETON(FlashTools)
public:
    Q_INVOKABLE void startBoot(const QString &bootImage);
    Q_INVOKABLE void flash(const QString &partName, const QString &imagePath);
    Q_INVOKABLE void clear(const QString &partName);

private:
    void executeCommand(const QStringList &args, const QString &workDir = QString());
};

} // namespace ADT

#endif // FLASHTOOLS_H
