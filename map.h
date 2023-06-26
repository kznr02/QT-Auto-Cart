#ifndef MAP_H
#define MAP_H

#include "qwidget.h"
#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

#define CircleWidth 50
#define FrameWidth 450
#define FramePos_x      this->width()/2-FrameWidth/2
#define FramePos_y      this->height()/2-FrameWidth/2
#define CirclePos_x1    FramePos_x+FrameWidth/6-CircleWidth/2
#define CirclePos_x2    FramePos_x+FrameWidth/2-CircleWidth/2
#define CirclePos_x3    FramePos_x+FrameWidth/6*5-CircleWidth/2
#define CirclePos_y1    FramePos_y+FrameWidth/6-CircleWidth/2
#define CirclePos_y2    FramePos_y+FrameWidth/2-CircleWidth/2
#define CirclePos_y3    FramePos_y+FrameWidth/6*5-CircleWidth/2
#define MarkWidth_self  30
#define MarkWidth_des   70
#define diff_des        (MarkWidth_des - CircleWidth)/2
#define diff_self       (CircleWidth - MarkWidth_self)/2

namespace Ui {
class Map;
}

class Map : public QDialog
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = nullptr);
    ~Map();

private:
    Ui::Map *ui;

protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void on_exitButton_clicked();
};

#endif // MAP_H
