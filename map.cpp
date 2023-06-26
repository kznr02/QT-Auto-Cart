#include "map.h"
#include "qcolor.h"
#include "qevent.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qnamespace.h"
#include "ui_map.h"
#include <QFile>

QString key;
QString posCode;
QString desCode;

extern int cur_x;
extern int cur_y;

Map::Map(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Map)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
}

Map::~Map()
{
    delete ui;
}

void Map::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //绘制正方形
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    painter.drawRect(FramePos_x, FramePos_y, FrameWidth, FrameWidth);


    painter.setPen(QPen(Qt::blue, 2, Qt::DashDotLine));

    QFile fd;
    fd.setFileName(":/db/pos.json");
    try {
        fd.open(QIODevice::ReadOnly);
    } catch (...) {
        qDebug()<<"打开失败";
        return;
    }
    QJsonParseError jsonErr;
    QJsonDocument desDoc = QJsonDocument::fromJson(fd.readAll(), &jsonErr);
    int des_x;
    int des_y;
    if(jsonErr.error == QJsonParseError::NoError)
    {
        QJsonObject desObj = desDoc.object();
        des_x = desObj.value(key).toObject().value("x").toInt();
        des_y = desObj.value(key).toObject().value("y").toInt();
        painter.drawEllipse(CirclePos_x1+(des_x-1)*FrameWidth/6-diff_des, CirclePos_y1+(des_y-1)*FrameWidth/6-diff_des, MarkWidth_des, MarkWidth_des);
    }
    painter.setBrush(QBrush(QColor(Qt::red)));
    painter.setPen(QPen(Qt::red, 2, Qt::DashDotLine));
    painter.drawEllipse(CirclePos_x1+(cur_x-1)*FrameWidth/6+diff_self, CirclePos_y1+(cur_y-1)*FrameWidth/6+diff_self, MarkWidth_self, MarkWidth_self);


    painter.setBrush(QBrush(QColor(Qt::black)));
    painter.setPen(Qt::NoPen);
    //(1,1)
    painter.drawEllipse(CirclePos_x1, CirclePos_y1, CircleWidth, CircleWidth);
    //(1,2)
    painter.drawEllipse(CirclePos_x2, CirclePos_y1, CircleWidth, CircleWidth);
    //(1,3)
    painter.drawEllipse(CirclePos_x3, CirclePos_y1, CircleWidth, CircleWidth);
    //(2,1)
    painter.drawEllipse(CirclePos_x1, CirclePos_y2, CircleWidth, CircleWidth);
    //(2,2)
    painter.drawEllipse(CirclePos_x2, CirclePos_y2, CircleWidth, CircleWidth);
    //(2,3)
    painter.drawEllipse(CirclePos_x3, CirclePos_y2, CircleWidth, CircleWidth);
    //(3,1)
    painter.drawEllipse(CirclePos_x1, CirclePos_y3, CircleWidth, CircleWidth);
    //(3,2)
    painter.drawEllipse(CirclePos_x2, CirclePos_y3, CircleWidth, CircleWidth);
    //(3,3)
    painter.drawEllipse(CirclePos_x3, CirclePos_y3, CircleWidth, CircleWidth);
}

void Map::on_exitButton_clicked()
{
    this->close();
}

