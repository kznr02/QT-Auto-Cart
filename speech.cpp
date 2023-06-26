#include "speech.h"
#include "qdebug.h"
extern QString key;

Speech::Speech(QObject *parent)
    : QObject{parent}
{

}

QString Speech::speechIdentify_baidu(QString fileName)
{
    QString tokenUrl = QString(baiduTokenUrl).arg(client_id).arg(client_secret);
    http my_http;
    QMap<QString, QString>header;
    header.insert(QString("Content-Type"), QString("audio/pcm;rate=16000"));
    QByteArray requestData;
    QByteArray replyData;

    qDebug()<<tokenUrl;
    bool result = my_http.post_sync(tokenUrl, header, requestData, replyData);
    if(result)
    {
        QString key = "access_token";
        QString accessToken = getJsonValue(replyData, key);
        qDebug()<<accessToken;

        QString speechUrl = QString(baiduSpeechUrl) .arg(QHostInfo::localHostName()).arg(accessToken);
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        requestData = file.readAll();
        file.close();

        result = my_http.post_sync(speechUrl, header, requestData, replyData);
        if(result)
        {
            QString key = "result";
            QString retText = getJsonValue(replyData, key);
            qDebug()<<retText;
            return retText;
        }
        else
        {
            return NULL;
        }
    }
    else{
        return "error";
    }
}

bool Speech::speechIdentify_aiui(QString fileName)
{
    QJsonObject aiuiObj;
    QJsonDocument aiuiDoc;
    aiuiObj.insert("auth_id", "2049a1b2fdedae553bd03ce6f4820ac4");
    aiuiObj.insert("data_type", "audio");
    aiuiObj.insert("sample_rate", "16000");
    aiuiObj.insert("aue", "raw");
    aiuiObj.insert("scene", "main_box");
    aiuiObj.insert("result_level", "plain");
    aiuiDoc.setObject(aiuiObj);
    QString paramBase64 =aiuiDoc.toJson().toBase64();
    QString curTime = QString::number(QDateTime::currentDateTimeUtc().toTime_t());
    QString checkSum = QCryptographicHash::hash((AIUI_apiKey+curTime+paramBase64).toLatin1(), QCryptographicHash::Md5).toHex();
    QMap<QString, QString> header;
    header.insert(QString("X-Appid"), AIUI_appId);
    header.insert(QString("X-CurTime"), curTime);
    header.insert(QString("X-Param"), paramBase64);
    header.insert(QString("X-CheckSum"), checkSum);

    QByteArray requestData;
    QByteArray replyData;
    http my_http;
    QFile file;
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);
    qDebug()<<"file.readAll();";
    requestData = file.readAll();
    qDebug()<<"post_sync";
    my_http.post_sync(aiuiUrl, header, requestData, replyData);
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyData, &parseError);
    qDebug()<<jsonDocument.toJson().toBase64();
    if(parseError.error == QJsonParseError::NoError)
    {
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains("data"))
        {
            QJsonValue val1 = jsonObject.value("data");
            if(val1.isArray())
            {
                QJsonArray arr1 =val1.toArray();
                QJsonValue val2 = arr1.at(1);
                if(val2.isObject())
                {
                    QJsonObject obj1 = val2.toObject();
                    if(obj1.contains("intent"))
                    {
                        QJsonValue val3 = obj1.value("intent");
                        if(val3.isObject())
                        {
                            QJsonObject obj2 = val3.toObject();
                            if(obj2.contains("semantic"))
                            {
                                QJsonValue val4 = obj2.value("semantic");
                                if(val4.isArray())
                                {
                                    QJsonArray arr2 = val4.toArray();
                                    QJsonValue val5 = arr2.at(0);
                                    if(val5.isObject())
                                    {
                                        QJsonObject obj3 = val5.toObject();
                                        if(obj3.contains("slots"))
                                        {
                                            QJsonValue val6 = obj3.value("slots");
                                            if(val6.isArray())
                                            {
                                                QJsonArray arr3 = val6.toArray();
                                                QJsonValue val7 = arr3.at(0);
                                                if(val7.isObject())
                                                {
                                                    QJsonObject obj4 = val7.toObject();
                                                    if(obj4.contains("normValue"))
                                                    {
                                                        key = obj4.value("normValue").toString();
                                                        return true;
                                                    }
                                                    else
                                                    {
                                                        return false;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

QString Speech::getJsonValue(QByteArray ba, QString key)
{
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &parseError);
    if(parseError.error == QJsonParseError::NoError)
    {
        QJsonObject jsonObject = jsonDocument.object();
        if(jsonObject.contains(key))
        {
            QJsonValue jsonValue = jsonObject.value(key);
            if(jsonValue.isString())
                return jsonValue.toString();
            else if(jsonValue.isArray())
            {
                QJsonArray arr = jsonValue.toArray();
                QJsonValue val = arr.at(0);
                return val.toString();
            }
        }
    }
    return "";
}

