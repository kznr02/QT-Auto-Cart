#include "widget.h"
#include <QFile>
#include <QApplication>
#include <map.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //QSS
    QFile qss(":/qss/widget.qss");
    qss.open(QIODevice::ReadOnly);
    a.setStyleSheet(qss.readAll());
    QFont font = QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/fonts/HarmonyOS_Sans_SC/HarmonyOS_Sans_SC_Regular.ttf")).at(0));
    a.setFont(font);
    qss.close();
    w.show();
    return a.exec();

}
