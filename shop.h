#ifndef SHOP_H
#define SHOP_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QStandardItem>
#include "qserialport.h"
#include "scan.h"
#include <cstdint>
#include <va.h>
#include <QDebug>
//#include <sqlite3.h>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QSerialPort>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <QTimer>
namespace Ui {
class shop;
}



class shop : public QWidget
{
    Q_OBJECT

public:
    explicit shop(QWidget *parent = nullptr);
    ~shop();
    void init_cartTable();
    void init_db();

    void init_uart();
    void config_timer(int msec);
private slots:
    void on_exitButton_clicked();

    void on_checkoutButton_clicked();

    void on_incartButton_clicked();

    void on_vaButton_clicked();

    void on_scanExit();

    void on_outcartButton_clicked();
    void on_uart1_read();
    void on_uart2_read();

//    void on_pos_refresh();
private:
    Ui::shop *ui;
    float total = 0;
    scan *scanUI;
    VA *vaUI;
    QSerialPort *port1;
    QSerialPort *port2;
//    sqlite3 *db;
    QSqlDatabase dbconn;

    QTimer  *posRefreshTimer;
};



#endif // SHOP_H
