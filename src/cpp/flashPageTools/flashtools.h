#ifndef FLASHTOOLS_H
#define FLASHTOOLS_H

#include <QObject>
#include <qcontainerfwd.h>
#include "../utils/singleton.hpp"
#include "src/cpp/adb/adbtools.h"

class FlashTools : public QObject
{
    Q_OBJECT
    SINGLETON(FlashTools)
public:
    Q_INVOKABLE void startBoot(const QString &deviceCode, const QString &bootImage);
    Q_INVOKABLE void flash(const QString &deviceCode, const QString &partName, const QString &imagePath);
    Q_INVOKABLE void clear(const QString &deviceCode, const QString &partName);
    Q_INVOKABLE void executeScript(const QString &deviceCode, const QString &scrptPath, const QString &workDir);
    Q_INVOKABLE void flashToXiaomi(const QString &deviceCode, const QString &packagePath);
    Q_INVOKABLE void unzip(const QString &zipPath, const QString &tarPath);

private:
    void executeCommand(ADBTools::APP appType, const QStringList &args);
};

#endif // FLASHTOOLS_H
