#ifndef FLASHURLHANDLE_H
#define FLASHURLHANDLE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QPointer>

struct FlashLinKInfo
{
    QString url;
    QString title;
    QString description;

    FlashLinKInfo(const QString &_url = "", const QString &_title = "", const QString &_description = "")
        : url(_url)
        , title(_title)
        , description(_description)
    {

    }
};

class FlashUrlHandle : public QObject
{
    Q_OBJECT
public:
    explicit FlashUrlHandle(QObject *parent = nullptr);
    ~FlashUrlHandle();

public:
    void doWork();
    QList<FlashLinKInfo> getInfo() const { return m_info; };

signals:
    void workFinish();

private:
    QPointer<QNetworkAccessManager> m_networkManager;
    QList<FlashLinKInfo> m_info;
};

#endif // FLASHURLHANDLE_H
