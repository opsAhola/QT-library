#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include<QMessageBox>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <string>
#include <QListWidget>
#include <QTableWidget>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDate>
#include <QFile>
#include <QTextCodec>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Initcodition();
     void connectMYSQL();
     void qury();
     void modifyrecords(QString cardID, QString BookID,int lend_return);
     void modifystorage(QString cardID,QString BookID, int lend_return);



private slots:

     void on_query_clicked();

     void on_login_clicked();

     void on_lend_clicked();

     void on_pushin_clicked();

     void on_cardmanage_clicked();

     void on_start_clicked();

    // void on_clear_clicked();

     void on_name2_editingFinished();

     void on_type1_editingFinished();

     void on_author3_editingFinished();

     void on_publish4_editingFinished();

     void on_year5_editingFinished();

     void on_price1_6_editingFinished();

     void on_price2_7_editingFinished();

     void on_return_2_clicked();

private:
    Ui::MainWindow *ui;
     QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
     QStringList condition; //查询条件数组
     QString str2; //查询拼接结果
     QString adminID;
     int listIDint=10002;
     int is_login=0;
      QSqlQueryModel *qmodel;//查询结果链表
      QSqlQueryModel *q2model;//图书查询结果链表
       QSqlQueryModel *cardIDmodel;//CardID查询结果链表
       QSqlQueryModel *adminmodel;//AdminID查询结果链表



};
#endif // MAINWINDOW_H
