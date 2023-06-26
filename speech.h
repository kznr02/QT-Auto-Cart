#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>
#include <QObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include "http.h"
#include "qobjectdefs.h"
#include <QHostInfo>
#include <QDateTime>
#include <QCryptographicHash>
#include <QMessageBox>

const QString baiduTokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";
const QString client_id = "1Aue8PRAZ5A7aCsxQa6t80Uv";
const QString client_secret = "YFMcHkZj9qy1LGGs2fPXoKngEHwBCYjh";

const QString baiduSpeechUrl = "https://vop.baidu.com/server_api?dev_pid=1537&cuid=%1&token=%2";

const QString aiuiUrl = "http://openapi.xfyun.cn/v2/aiui";
const QString AIUI_apiKey = "62577336321cfcd21f677451dd7f152d";
const QString AIUI_appId = "149c8459";

class Speech : public QObject
{
    Q_OBJECT
public:
    explicit Speech(QObject *parent = nullptr);
    QString speechIdentify_baidu(QString fileName);
    bool speechIdentify_aiui(QString fileName);
private:
    QString getJsonValue(QByteArray ba, QString key);


};

#endif // SPEECH_H
