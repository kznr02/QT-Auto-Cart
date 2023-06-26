#include "scan.h"
#include "opencv2/videoio.hpp"
#include "qdebug.h"
#include "qmessagebox.h"
#include "qnamespace.h"
#include "ui_scan.h"
#include <QZXing/QZXing.h>
#include <cstdint>

using namespace cv;

extern QString Code;
extern uint8_t isScan;
extern uint8_t isEAN13;
extern QString posCode;

scan::scan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scan)
{
    ui->setupUi(this);
    this->move(0, 0);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    ui->confirmButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    config_camera();

    connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer.start(10);
}


scan::~scan()
{
    delete ui;
}

void scan::config_camera()
{
    try {
        capture.open(0, CAP_V4L2);
        capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
        capture.set(CAP_PROP_FPS, 30);
        capture.set(CAP_PROP_FRAME_HEIGHT, 1080);
        capture.set(CAP_PROP_FRAME_WIDTH, 1920);

    } catch (...)
    {
        qDebug("Open camera failed!");
        return;
    }
}

void scan::on_timeout()
{
    try {
        capture >> cap;
    } catch (...) {
        this->timer.stop();
        disconnect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
        this->close();
        return;
    }
    try {
        cap = cap(cv::Rect(cap.cols/2-400, cap.rows/2-400, 800, 800));
    } catch (...) {
        //this->timer.stop();
        qDebug("resize failed");
    }
    cv::cvtColor(cap, cap, cv::COLOR_BGR2RGB);
    tmpimg = QImage((uchar *)cap.data, cap.cols, cap.rows, cap.step, QImage::Format_RGB888);
    tmppix = QPixmap::fromImage(tmpimg);
    tmppix.scaled(ui->photoLable->size(), Qt::KeepAspectRatio);
    ui->photoLable->setScaledContents(true);
    ui->photoLable->setPixmap(tmppix);
}



void scan::on_shotButton_clicked()
{
    ui->confirmButton->setVisible(true);
    ui->cancelButton->setVisible(true);
    timer.stop();
    qDebug("已拍摄！");
}


void scan::on_exitButton_clicked()
{
    timer.stop();
    capture.release();
    this->close();
    this->deleteLater();
}

void scan::on_confirmButton_clicked()
{
    QZXing decoder;
    decoder.setDecoder(QZXing::DecoderFormat_EAN_13);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_Rotate | QZXing::TryHarderBehaviour_ThoroughScanning);
    QString result = decoder.decodeImage(tmpimg);
    if(result.isEmpty())
    {
        decoder.setDecoder(QZXing::DecoderFormat_QR_CODE);
        result = decoder.decodeImage(tmpimg);
        if(result.isEmpty())
        {
            int ret = QMessageBox::critical(this, "错误", "扫码失败，请再试一次", QMessageBox::Ok);
            switch (ret) {
            case QMessageBox::Ok:
                {
                    timer.start();
                    ui->confirmButton->setVisible(false);
                    ui->cancelButton->setVisible(false);
                    break;
                }
            }
        }
        else
        {
            isScan = 1;
            posCode = result;
            timer.stop();
            capture.release();
            this->close();
            this->deleteLater();
            qDebug()<<result;
        }
    }
    else
    {
        isScan = 1;
        isEAN13 = 1;
        Code = result;
        qDebug()<<result;
        timer.stop();
        capture.release();
        this->close();
        this->deleteLater();
    }

}


void scan::on_cancelButton_clicked()
{
    timer.start();
    ui->confirmButton->setVisible(false);
    ui->cancelButton->setVisible(false);
}

