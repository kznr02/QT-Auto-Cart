#include "va.h"
#include "qmessagebox.h"
#include "speech.h"
#include "ui_va.h"
#include "scan.h"
#include <cstddef>


uint8_t isMapping;
extern uint8_t isScan;

VA::VA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VA)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->textLabel->setText("");
}

VA::~VA()
{
    delete ui;
}

void VA::on_speakButton_clicked()
{
    if(isSpeak == 0)
    {
        process.start("arecord", arecord_arg);

        if(process.waitForStarted())
        {
            isSpeak = 1;
            ui->speakButton->setText("我说完了");
            ui->textLabel->setText("正在聆听");
        }
        else
        {
            process.close();
            QMessageBox::critical(this, "错误", "录音启动失败，请检查");
        }
    }
    else
    {
        isSpeak = 0;
        process.close();
        process.start("ffmpeg", ffmpeg_arg);
        if(process.waitForStarted())
        {
            ui->textLabel->setText("识别中");
            if(process.waitForFinished())
            {
                Speech my_speech;
                bool ret = my_speech.speechIdentify_aiui("/tmp/tmp1.pcm");
                ui->speakButton->setText("请按下说话");
                if(ret == true)
                {
//                    QMessageBox::information(this, "提示", "请扫描身边的导航二维码");
                    this->close();
                    mapUI = new Map;
                    mapUI->show();
//                    scanUI = new scan();
//                    isMapping = 1;
//                    connect(scanUI, SIGNAL(destroyed()), this, SLOT(on_openMap()));
//                    scanUI->show();

                }
                else if(ret == false)
                    ui->textLabel->setText("我没有听清你的话，请再试一次");
                process.start("rm", QStringList()<<"-rf"<<"/tmp/tmp1.pcm");
            }
        }
    }
}



void VA::on_cancelButton_clicked()
{
    this->close();
}

void VA::on_openMap()
{
    if(isMapping == 1 && isScan == 1)
    {
        isMapping = 0;
        isScan = 0;
        mapUI = new Map;
        mapUI->show();
    }
}
