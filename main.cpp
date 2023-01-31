#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //输出可用数据库
     /*   qDebug()<<"available drivers:";
        QStringList drivers = QSqlDatabase::drivers();
        foreach(QString driver, drivers)
         qDebug()<<driver;*/

       QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));


    MainWindow w;
    //w.connectMYSQL();
    w.Initcodition();
    w.show();
    //w.qury();
    return a.exec();
}
