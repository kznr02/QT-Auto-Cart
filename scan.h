#ifndef SCAN_H
#define SCAN_H

#include <QWidget>
#include <QTimer>
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>

#include "opencv2/opencv.hpp"
#include "QZXing.h"
#include "qobjectdefs.h"

namespace Ui {
class scan;
}



class scan : public QWidget
{
    Q_OBJECT

public:
    explicit scan(QWidget *parent = nullptr);
    ~scan();
    QFont font = QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/fonts/SRCMING-VF.2.002.TTF")).at(0));
    void config_camera();

private slots:
    void on_timeout();

    void on_confirmButton_clicked();

    void on_shotButton_clicked();

    void on_exitButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::scan *ui;

    QTimer timer;
    cv::Mat cap;
    cv::VideoCapture capture;
    QImage tmpimg;
    QPixmap tmppix;

signals:
    void s_scan();
};


#endif // SCAN_H
