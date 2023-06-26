#include "widget.h"
#include "qjsondocument.h"
#include "qobjectdefs.h"
#include "ui_widget.h"
#include "QFontDatabase"
#include <QFile>
#include <cstdint>

uint8_t isPay;
QJsonDocument g_checkDoc;

QByteArray uart_data1;
QByteArray uart_data2;

int cur_x = 6;
int cur_y = 6;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

//    this->move(0, 0);
    this->setWindowFlags(Qt::FramelessWindowHint);
    m_imgList.append(":/banner/huawei.jpg");
    m_imgList.append(":/banner/lays.jpg");
    m_imgList.append(":/banner/jjs.jpg");
    m_imgList.append(":/banner/ex.jpg");
    m_imgList.append(":/banner/sk2.jpg");
    ui->adLabel->setScaledContents(true);
    ui->adLabel->setPixmap(QPixmap(":/banner/huawei.jpg"));
    connect(&m_bannerTimer, SIGNAL(timeout()), this, SLOT(on_banner_update()));
    m_bannerTimer.start(2000);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_exitButton_clicked()
{
    this->close();
}


void Widget::on_startButton_clicked()
{
    shopUI = new shop;
    shopUI->show();
}



void Widget::on_banner_update()
{
    ui->adLabel->setPixmap(QPixmap(m_imgList.at(m_currenImgIndex++)));
    if(m_currenImgIndex > m_imgList.count() - 1)
    {
        m_currenImgIndex = 0;
    }
}
