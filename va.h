#ifndef VA_H
#define VA_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <scan.h>
#include <map.h>
#include <QProcess>


namespace Ui {
class VA;
}

class VA : public QDialog
{
    Q_OBJECT

public:
    explicit VA(QWidget *parent = nullptr);
    ~VA();

    void startRecord();

private slots:
    void on_speakButton_clicked();

    void on_cancelButton_clicked();

    void on_openMap();

private:
    Ui::VA *ui;
    scan *scanUI;
    Map *mapUI;

    uint8_t isSpeak = 0;

    QFile tmpAudioFile;

    QProcess process;
    QStringList arecord_arg = {"-D", "hw:0,0",
                            "-f", "S16_LE",
                            "-r", "16000",
                            "-c", "2",
                            "-traw", "/tmp/tmp.pcm"};
    QStringList ffmpeg_arg = {"-f", "s16le",
                              "-ar", "16000",
                              "-ac", "2",
                              "-i", "/tmp/tmp.pcm",
                              "-f", "s16le",
                              "-ar", "16000",
                              "-ac", "1",
                              "/tmp/tmp1.pcm"};
};

#endif // VA_H
