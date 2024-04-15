#ifndef CUTACTIVITYCONTROL_H
#define CUTACTIVITYCONTROL_H

#include <QObject>
#include "../utils/singleton.hpp"

class CutActivityControl : public QObject
{
    Q_OBJECT
    SINGLETON(CutActivityControl)
    Q_PROPERTY(QString identifier READ getIdentifier NOTIFY valueChanged)
    Q_PROPERTY(QString cutPackageName READ getCutPackageName NOTIFY valueChanged)
    Q_PROPERTY(QString cutActivity READ getCutActivity NOTIFY valueChanged)
public:
    void updateInfo();
    Q_INVOKABLE void killCutActivity();

    QString getIdentifier() const {return m_identifier;}
    QString getCutPackageName() const {return m_cutPackageName;}
    QString getCutActivity() const {return m_cutActivity;}

signals:
    void valueChanged();

private:
    QString m_identifier;
    QString m_cutPackageName;
    QString m_cutActivity;
};

#endif // CUTACTIVITYCONTROL_H
