#include "terminalTools.h"
#include <qlogging.h>
#include <qobject.h>
#include <QtQml>
#include <QSysInfo>
#include <qsysinfo.h>
#include "qtermwidgetforqml.h"

TerminalPageTools::TerminalPageTools(QObject *parent)
: QObject(parent)
{
    qmlRegisterSingletonInstance("TerminalPageTools", 1, 0, "TerminalPageTools", this);
    qmlRegisterType<QTermWidgetForQml>("QTermWidgetForQml", 1, 0, "QTermWidgetForQml");
}

void TerminalPageTools::startSystemTerminal()
{
#ifdef Q_OS_LINUX
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString de = env.value("XDG_CURRENT_DESKTOP", "").toLower();
    if (de == "kde") {
        QProcess::startDetached("konsole");
    } else if (de == "gnome") {
        QProcess::startDetached("gnome-terminal");
    } else if (de == "xfce") {
        QProcess::startDetached("xfce4-terminal");
    } else if (de == "lxde") {
        QProcess::startDetached("lxterminal");
    } else if (de == "fluxbox") {
        QProcess::startDetached("xterm");
    } else if (de == "openbox") {
        QProcess::startDetached("xterm");
    } else if (de == "cinnamon") {
        QProcess::startDetached("gnome-terminal");
    } else if (de == "mate") {
        QProcess::startDetached("mate-terminal");
    }


#elif defined(Q_OS_WIN)
    QProcess::startDetached("cmd.exe");
#elif defined(Q_OS_MAC)
    QProcess::startDetached("Terminal.app");
#endif
}
