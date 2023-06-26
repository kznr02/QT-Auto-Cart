#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qserialport.h"
#include "qtimer.h"
#include "shop.h"
#include <QTimer>
#include <map.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_exitButton_clicked();

    void on_startButton_clicked();



    void on_banner_update();
private:
    Ui::Widget *ui;

    shop *shopUI;



    QList<QString> m_imgList;
    QTimer m_bannerTimer;
    int m_currenImgIndex = 1;
};

#endif // WIDGET_H
