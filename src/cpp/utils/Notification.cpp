#include "Notification.h"
#include <QDebug>
#include <QVariant>

NotificationControl::NotificationControl(QObject *parent) : QObject(parent)
{

}

void NotificationControl::send(const QString &text, Type type, int duration)
{
    if (!m_notificationObject) {
        qWarning() << "not found notification qml object, please set it";
        return;
    }
    QMetaObject::invokeMethod(m_notificationObject, "send", Q_ARG(QVariant, text), Q_ARG(QVariant, type), Q_ARG(QVariant, duration));
}

void NotificationControl::setQmlObject(QObject *o)
{
    m_notificationObject = o;
}
