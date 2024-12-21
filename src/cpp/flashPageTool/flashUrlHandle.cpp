#include "flashUrlHandle.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

FlashUrlHandle::FlashUrlHandle(QObject *parent)
    : QObject(parent)
{

}

FlashUrlHandle::~FlashUrlHandle()
{
    m_networkManager->deleteLater();
}

void FlashUrlHandle::doWork()
{
    if (!m_networkManager) {
        m_networkManager = new QNetworkAccessManager;
    }
    QUrl url("http://124.70.49.250:1234/flashurls");
    QNetworkRequest request(url);

    QNetworkReply *reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply](){
        QByteArray data = reply->readAll();
        m_info.clear();
        QJsonDocument m_document;
        m_document = QJsonDocument::fromJson(data);
        QJsonArray jsonArray = m_document.array();
        for (auto jsonItem : jsonArray) {
            auto jsonObject = jsonItem.toObject();
            FlashLinKInfo info;
            info.title = jsonObject.value("title").toString();
            info.url = jsonObject.value("url").toString();
            info.description = jsonObject.value("description").toString();
            m_info.append(info);
        }
        emit workFinish();
        reply->deleteLater();
    });
}
