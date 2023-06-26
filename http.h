#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QSsl>
#include <QSslSocket>


class http : public QObject
{
    Q_OBJECT
public:
    explicit http(QObject *parent = nullptr);

    bool post_sync(QString url, QMap<QString, QString>header, QByteArray requestData, QByteArray &replyData);
};

#endif // HTTP_H
