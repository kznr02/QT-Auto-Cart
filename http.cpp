#include "http.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkrequest.h"

http::http(QObject *parent)
    : QObject{parent}
{

}

bool http::post_sync(QString url,QMap<QString,QString>header,QByteArray requestData,QByteArray &replyData)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "x-www-form-urlencoded");
    request.setUrl(url);
    QMapIterator<QString, QString> it(header);
    while (it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toLatin1(), it.value().toLatin1());
    }

//    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//    conf.setProtocol(QSsl::TlsV1_0);
//    request.setSslConfiguration(conf);

    QNetworkReply *reply = manager.post(request, requestData);
    QEventLoop l;

    connect(reply, &QNetworkReply::finished, &l, &QEventLoop::quit);
    l.exec();
    if(reply != nullptr && reply->error() == QNetworkReply::NoError)
    {
        replyData = reply->readAll();
        return true;
    }
    else
    {
        qDebug()<<"request error!";
        return false;
    }
}
