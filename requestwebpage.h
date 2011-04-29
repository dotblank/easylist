#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QDebug>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QCryptographicHash>
#include <QByteArray>

class RequestWebpage : public QObject
{
    Q_OBJECT
public:
    explicit RequestWebpage(QObject *parent = 0);
    void fetch(QString url);
    void post(QString url, QByteArray data);

private:
    QNetworkAccessManager* networkManager;

public slots:

signals:
    void finished(QNetworkReply*);
};

#endif // HTTPREQUEST_H
