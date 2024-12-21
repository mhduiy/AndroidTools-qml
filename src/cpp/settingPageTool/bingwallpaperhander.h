#ifndef BINGWALLPAPERHANDER_H
#define BINGWALLPAPERHANDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QPointer>

class BingWallPaperHander : public QObject
{
    Q_OBJECT
public:
    explicit BingWallPaperHander(QObject *parent = nullptr);
    ~BingWallPaperHander();

public:
    void doWork();

signals:
    void workFinish(const QString &url);

private:
    QPointer<QNetworkAccessManager> m_networkManager;
};

#endif // BINGWALLPAPERHANDER_H
