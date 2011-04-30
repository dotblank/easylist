#include "requestwebpage.h"

RequestWebpage::RequestWebpage(QObject *parent) :
    QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SIGNAL(finished(QNetworkReply*)));
}

void RequestWebpage::fetch(QString url)
{
    networkManager->get(QNetworkRequest(QUrl(url)));
}
void RequestWebpage::post(QString url, QByteArray data)
{
    QNetworkRequest req;
    req.setUrl(url);
    networkManager->post(req,data);
}
