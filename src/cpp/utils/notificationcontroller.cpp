#include "notificationcontroller.h"
#include <QThread>
#include <QDebug>

NotificationController::NotificationController(QObject *parent) : QObject(parent)
{

}

void NotificationController::send(const QString &title, const QString &content, NotificationType type, int duration)
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, [this, title, content, type, duration]() {
            send(title, content, type, duration);
        }, Qt::QueuedConnection);
        return;
    }

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