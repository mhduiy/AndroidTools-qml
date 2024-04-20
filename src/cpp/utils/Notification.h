#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QObject>
#include "singleton.hpp"

class NotificationControl : public QObject
{
    Q_OBJECT
    SINGLETON(NotificationControl)
public:
    enum Type {
        Info, Warning, Error
    };
    Q_INVOKABLE void send(const QString &text, Type type = Type::Info, int duration = 3000);
    void setQmlObject(QObject *o);
private:
    QObject *m_notificationObject = nullptr;
};

#endif
