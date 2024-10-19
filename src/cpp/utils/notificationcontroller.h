#pragma once

#include <QObject>
#include "singleton.hpp"

class NotificationController : public QObject
{
    Q_OBJECT
    SINGLETON(NotificationController)

    Q_PROPERTY(QString title MEMBER m_title NOTIFY requestNotification)
    Q_PROPERTY(QString content MEMBER m_content NOTIFY requestNotification)
    Q_PROPERTY(int type MEMBER m_type NOTIFY requestNotification)
    Q_PROPERTY(int duration MEMBER m_duration NOTIFY requestNotification)

public:
    enum NotificationType {
        Info, Warning, Error
    };
    Q_ENUM(NotificationType)

    Q_INVOKABLE void send(const QString &title, const QString &content, NotificationType type = NotificationType::Info, int duration = 3000);

signals:
    void requestNotification();

private:
    QString m_title;
    QString m_content;
    int m_type;
    int m_duration;
};
