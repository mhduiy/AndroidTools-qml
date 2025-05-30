#include "notificationcontroller.h"
#include <QDebug>
#include <QVariant>

NotificationController::NotificationController(QObject *parent) : QObject(parent)
{

}

void NotificationController::send(const QString &title, const QString &content, NotificationType type, int duration)
{
    qInfo () << "发送通知:" << title << content << type << duration;
    if (duration < 0) {
        qWarning() << "duration can not less than 0";
        return;
    }
    m_title = title;
    m_content = content;
    m_type = type;
    m_duration = duration;

    Q_EMIT requestNotification();
}