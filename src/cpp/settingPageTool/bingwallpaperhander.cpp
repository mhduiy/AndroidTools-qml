#include "bingwallpaperhander.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

BingWallPaperHander::BingWallPaperHander(QObject *parent)
    : QObject(parent)
{

}

BingWallPaperHander::~BingWallPaperHander()
{
    m_networkManager->deleteLater();
}

void BingWallPaperHander::doWork()
{
    if (!m_networkManager) {
        m_networkManager = new QNetworkAccessManager;
    }
    QUrl url("http://124.70.49.250:1234/bing/wallpaper");
    QNetworkRequest request(url);

    QNetworkReply *reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply](){
        QByteArray data = reply->readAll();
        QJsonDocument m_document;
        m_document = QJsonDocument::fromJson(data);
        QJsonObject jsonObject = m_document.object();
        const QString retUrl = jsonObject.value(QStringLiteral("url")).toString();
        emit workFinish(retUrl);
        reply->deleteLater();
    });
}
