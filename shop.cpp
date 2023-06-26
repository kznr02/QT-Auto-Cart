#include "shop.h"
#include "qdebug.h"

#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qmessagebox.h"
#include "qnamespace.h"
#include "qobjectdefs.h"
#include "qserialport.h"
#include "qwidget.h"
#include "ui_shop.h"
#include <cstdint>


QString Code;
QString itemTmp[3];
uint8_t isScan;
uint8_t isEAN13;
uint8_t isIncart;
uint8_t isOutcart;
uint8_t sqlError;
uint8_t isExist;
extern QByteArray uart_data1;
extern QByteArray uart_data2;

extern int cur_x;
extern int cur_y;

extern uint8_t isPay;
extern QJsonDocument g_checkDoc;

shop::shop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::shop)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
//    this->move(0, 0);

    init_cartTable();
    init_db();
    init_uart();
    connect(port1, SIGNAL(readyRead()), this, SLOT(on_uart1_read()));
    connect(port2, SIGNAL(readyRead()), this, SLOT(on_uart2_read()));

//    config_timer(300);
}

shop::~shop()
{
    delete ui;
}

void shop::init_cartTable()
{
    //购物车配置
    ui->cartTable->setColumnCount(4);
    ui->cartTable->setHorizontalHeaderLabels(QStringList()<<"商品"<<"价格"<<"条形码"<<"数量");
    ui->cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->cartTable->setFocusPolicy(Qt::NoFocus);
    ui->cartTable->setAutoScroll(true);
    ui->cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //总计显示
    ui->totalLabel->setText(tr("总计：%1 元").arg(total));
}

void shop::init_db()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        dbconn = QSqlDatabase::database("qt_sql_default_connection");
        dbconn.close();
        dbconn = QSqlDatabase::addDatabase("QSQLITE");
    }
    else
        dbconn = QSqlDatabase::addDatabase("QSQLITE");
    dbconn.setDatabaseName("/opt/db/production.sqlite");

    if(dbconn.open())
    {
        qDebug()<<"Open success";
    }
    else {
        qDebug()<<"Open db failed";
        QMessageBox::critical(this, "错误", "未连接数据库");
        exit(-1);
    }
}



void shop::config_timer(int msec)
{
    posRefreshTimer = new QTimer;
    posRefreshTimer->start(msec);
    connect(posRefreshTimer, SIGNAL(timeout()), this, SLOT(on_pos_refresh()));
}

void shop::on_exitButton_clicked()
{
    cur_x = 6;
    cur_y = 6;
    dbconn.close();
    port1->close();
    port2->close();
    this->close();
    this->deleteLater();
}


void shop::on_checkoutButton_clicked()
{
//    QMessageBox::critical(this, "错误", "购物车内没有此商品", QMessageBox::Cancel);
    if(ui->cartTable->rowCount() == 0)
    {
        QMessageBox::critical(this, "警告", "购物车为空!");
    } else {
        isPay = 1;
        QJsonDocument checkDoc;
        QJsonObject checkObj;
        checkObj.insert("支付状态", QJsonValue(isPay));
        for(int i=0; i<ui->cartTable->rowCount(); i++)
        {
            QJsonArray *tmpArr = new QJsonArray;
            tmpArr->append(QJsonValue(ui->cartTable->takeItem(i, 0)->text()));
            tmpArr->append(QJsonValue(ui->cartTable->takeItem(i, 1)->text().toFloat()));
            tmpArr->append(QJsonValue(ui->cartTable->takeItem(i, 3)->text().toInt()));
            checkObj.insert(tr("%1").arg(QString::number(i+1)), *tmpArr);
            delete tmpArr;
        }
        checkObj.insert("总计", QJsonValue(total));
        checkDoc.setObject(checkObj);
//        port->write(checkDoc.toJson());
        qDebug()<<checkDoc;
        g_checkDoc = checkDoc;

        QMessageBox::information(this, "购物完成", "结帐成功，请前往查验处打印账单");
        dbconn.close();
        port1->close();
        port2->close();
        cur_x = 6;
        cur_y = 6;
        this->close();
        this->deleteLater();
    }


}


void shop::on_incartButton_clicked()
{
    scanUI = new scan();
    connect(scanUI, SIGNAL(destroyed()), this, SLOT(on_scanExit()));
    isIncart = 1;
    scanUI->show();
}

void shop::on_outcartButton_clicked()
{
    scanUI = new scan();
    connect(scanUI, SIGNAL(destroyed()), this, SLOT(on_scanExit()));
    isOutcart = 1;
    scanUI->show();
}

void shop::on_vaButton_clicked()
{
    vaUI = new VA();
    vaUI->show();
}

void shop::on_scanExit()
{
    if(isIncart == 1 && isScan == 1 && isEAN13 == 1)
    {
        isScan = 0;
        isIncart = 0;
        isEAN13 = 0;
        QString sql = "SELECT * FROM production WHERE 条形码 = " + Code;
//        Code.clear();
        qDebug()<<sql;
        QSqlQuery sqlquery(dbconn);
        sqlquery.prepare(sql);
        if(!sqlquery.exec())
        {
            qDebug()<<"Exec failed";
            qDebug()<<sqlquery.lastError();
        }
        else
        {
            sqlquery.first();
            if(sqlquery.isNull(0))
            {
                QMessageBox::critical(this, "错误", "该商品不存在");
            }
            else
            {
                QList<QTableWidgetItem *> item = ui->cartTable->findItems(Code, Qt::MatchFlag::MatchExactly);
                if(item.isEmpty())
                {
                    ui->cartTable->setRowCount(ui->cartTable->rowCount()+1);
                    ui->cartTable->setItem(ui->cartTable->rowCount()-1, 0, new QTableWidgetItem(sqlquery.value(0).toString()));
                    ui->cartTable->setItem(ui->cartTable->rowCount()-1, 1, new QTableWidgetItem(QString::number(sqlquery.value(1).toFloat())));
                    ui->cartTable->setItem(ui->cartTable->rowCount()-1, 2, new QTableWidgetItem(sqlquery.value(3).toString()));
                    ui->cartTable->setItem(ui->cartTable->rowCount()-1, 3, new QTableWidgetItem("1"));

                }
                else
                {
                    ui->cartTable->setItem(item[0]->row(), 3, new QTableWidgetItem(tr("%1").arg(QString::number(ui->cartTable->takeItem(item[0]->row(), 3)->text().toInt()+1))));
                }
                total += sqlquery.value(1).toFloat();
                ui->totalLabel->setText(tr("总计：%1 元").arg(total));
            }
            sqlquery.finish();
        }
    }
    else if(isOutcart == 1 && isScan == 1 && isEAN13 == 1)
    {
        isScan = 0;
        isOutcart = 0;
        isEAN13 = 0;
        QList<QTableWidgetItem *> item = ui->cartTable->findItems(Code, Qt::MatchFlag::MatchExactly);
        if(item.isEmpty())
        {
            QMessageBox::critical(this, "错误", "购物车内没有此商品", QMessageBox::Cancel);
        }
        else
        {
            total -= ui->cartTable->item(item[0]->row(), 1)->text().toFloat();
            if(ui->cartTable->takeItem(item[0]->row(), 3)->text().toInt()>1)
            {
                ui->cartTable->setItem(item[0]->row(), 3, new QTableWidgetItem(tr("%1").arg(QString::number(ui->cartTable->takeItem(item[0]->row(), 3)->text().toInt()-1))));
            }
            else
            {
                ui->cartTable->removeRow(item[0]->row());
            }
            ui->totalLabel->setText(tr("总计：%1 元").arg(total));
        }
    }
}



//void shop::on_pos_refresh()
//{
//    ui->subposLabel->setText(tr("(%1, %2)").arg(cur_x).arg(cur_y));
//}


void shop::on_uart1_read()
{
    uart_data1 = port1->readAll();
    if(QJsonDocument::fromJson(uart_data1).isObject())
    {
        QJsonObject pos_obj = QJsonDocument::fromJson(uart_data1).object();
        cur_x = pos_obj.value("x").toInt();
        cur_y = pos_obj.value("y").toInt();
        qDebug()<<"X: "<<cur_x;
        qDebug()<<"Y: "<<cur_y;
    }
}

void shop::on_uart2_read()
{
    uart_data2 = port2->readAll();
    if(uart_data2.startsWith('a') && isPay)
    {
        isPay = 0;
        port2->write(g_checkDoc.toJson());
    } else if(uart_data2.startsWith('a'))
    {
        port2->write("b");
    }
}

void shop::init_uart()
{
    port1 = new QSerialPort;
    port2 = new QSerialPort;
    port1->setPortName("ttyUSB0");
    port2->setPortName("ttyUSB1");
    port1->open(QIODevice::ReadWrite);
    port2->open(QIODevice::ReadWrite);
    if(!(port1->isOpen() && port2->isOpen()))
    {
        QMessageBox::critical(this, "错误", "扫描仪未连接");
        //        exit(-1);
    } else {
        //设置波特率
        port1->setBaudRate(QSerialPort::Baud9600);
        //设置数据位
        port1->setDataBits(QSerialPort::Data8);
        //设置停止位
        port1->setStopBits(QSerialPort::OneStop);
        //设置极性
        port1->setParity(QSerialPort::NoParity);
        //设置控制流
        port1->setFlowControl(QSerialPort::NoFlowControl);

        //设置波特率
        port2->setBaudRate(QSerialPort::Baud9600);
        //设置数据位
        port2->setDataBits(QSerialPort::Data8);
        //设置停止位
        port2->setStopBits(QSerialPort::OneStop);
        //设置极性
        port2->setParity(QSerialPort::NoParity);
        //设置控制流
        port2->setFlowControl(QSerialPort::NoFlowControl);
    }
}
