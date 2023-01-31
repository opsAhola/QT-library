#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectMYSQL();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectMYSQL()
{

   db.setHostName("127.0.0.1");
   db.setDatabaseName("libary");
   db.setUserName("root");
   db.setPassword("111111");
   bool ok = db.open();
   if(ok){
         QMessageBox::information(this, "infor", "connect databases success");
   }else{
         QMessageBox::information(this, "infor", "connect databases failed");
   }

}
 void MainWindow:: qury()
 {
     //查询大小字母不区分；查找前面的名字匹配的；多个条件一起查找
     qmodel = new QSqlQueryModel();
     QStringList tables = db.tables();
        foreach(QString table, tables)
            qDebug()<<table;
    QString str1="select * from Books where ";
    str1+=str2;

    qDebug()<<str1;
    qmodel->setQuery(str1);
    ui->tableView->setModel(qmodel);


 }
 void MainWindow:: modifyrecords(QString cardID, QString BookID,int lend_return)
 {

     QDate D1;
     D1=QDate::currentDate();

     QSqlQuery query;
      QSqlQuery query1;
     QString str;
     if(lend_return==1)
     {

         query.prepare("insert into records(listID,CardID,BookID,lentdate) values(:LisID,:cardID,:BookID,:lentdate)");
          query1.exec("select max(listID) FROM records");
          query1.next();
          listIDint=query1.value(0).toInt();
          listIDint++;
           qDebug()<<listIDint;


               query.bindValue(":LisID", listIDint);
               query.bindValue(":cardID", cardID);
               query.bindValue(":BookID",BookID);
               query.bindValue(":lentdate",D1);
              // query.bindValue(":returndate",NULL);

               if(!query.exec()){
                   QMessageBox::critical(this,"infor","新增失败！");
                   return;
               }
               else
               {
                       QMessageBox::information(this,"infor","借书成功");
               }
     }
     else if(lend_return==2)
     {
         query.prepare("delete from records where BookID=:BookID and cardID=:cardID");
          query.bindValue(":BookID",BookID);
          query.bindValue(":cardID",cardID);
          if(!query.exec()){
              QMessageBox::critical(this,"infor","删除失败！");
              return;
          }
          else
          {
                  QMessageBox::information(this,"infor","还书成功");
          }
     }

 }
 void MainWindow:: modifystorage(QString cardID,QString BookID, int lend_return)
 {

    int storage;
    int storagenew;
     QSqlQuery query;
     QSqlQuery query1;

     query.prepare("select storage from Books where BookID=:bookID");
     query.bindValue(":bookID", BookID);
     query1.prepare("update Books set storage =:storagenew where BookID=:bookID");
     query1.bindValue(":bookID", BookID);
     if(query.exec())
     {

         if(!query.next())
            {
              QMessageBox::warning(this, "Warning", "借书卡号输入错误!");
              return;
         }
         storage=query.value("storage").toInt();
          qDebug()<<storage;

          if(lend_return==1 && storage==0)
          {
              QMessageBox::warning(this,"infor","抱歉，该书已全部借出！");
          }
          else if(lend_return==1)
          {

               storagenew=storage-1;
                 qDebug()<<storage;
                qDebug()<<storagenew;
                 query1.bindValue(":storagenew", storagenew);
                  if(query1.exec())
                  {
                        qDebug()<<"add storage sucess";
                        modifyrecords(cardID, BookID,lend_return);

                   }

          }
          else if(lend_return==2)
          {
              storagenew=storage+1;
                qDebug()<<storage;
               qDebug()<<storagenew;
                query1.bindValue(":storagenew", storagenew);
                     if(query1.exec())
                     {
                           qDebug()<<"sub storage sucess";
                           modifyrecords(cardID, BookID,lend_return);

                      }
          }

     }

 }
 void MainWindow::on_query_clicked()
 {


     q2model= new QSqlQueryModel();
    QString querystr="select * from books";
    //QString querystr="select * from Books where BookID in (select BookID from Records natural join libaryCard where CardID='3200102164')";
     q2model->setQuery(querystr);
     /*qmodel->setHeaderData(0,Qt::Horizontal,"书号");
     qmodel->setHeaderData(1,Qt::Horizontal,"书名");*/
     ui->tableView->setModel(q2model);


 }

 void MainWindow::on_login_clicked()
 {
     adminmodel= new QSqlQueryModel();
     QString querystr="select AdminID from administor where AdminID='";
     int cnt=0;
     QDialog dialog(this);
     QFormLayout form(&dialog);
     form.addRow(new QLabel("请输入账号与密码:"));
     // Value1
     QString value1 = "账号: ";
     QLineEdit *spinbox1 = new QLineEdit(&dialog);
     form.addRow(value1, spinbox1);
     // Value2
     QString value2 = "密码 ";
     QLineEdit *spinbox2 = new QLineEdit(&dialog);
     form.addRow(value2, spinbox2);
     // Add Cancel and OK button
     QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
         Qt::Horizontal, &dialog);
     form.addRow(&buttonBox);
     QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
     QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
     // Process when OK button is clicked
     if (dialog.exec() == QDialog::Accepted) {
         // Do something here
        adminID=spinbox1->text();
         QString temp2=spinbox2->text();
         qDebug()<<adminID;
         qDebug()<<temp2;
         querystr+=adminID;
         querystr+="'";
         querystr+=" and Password='";
         querystr+=temp2;
         querystr+="'";
         qDebug()<<querystr;
        adminmodel->setQuery(querystr);
         cnt=adminmodel->rowCount();
          qDebug()<<"Count="<<cnt;
          if(cnt!=0)
          {
             QMessageBox::information(this, "infor", "登录成功");
             is_login=1;

          }
          else
          {
              QMessageBox::warning(this, "Warning", "账号或密码错误，请重新登录！");
          }

     }

 }
void MainWindow::on_lend_clicked()
{
   cardIDmodel= new QSqlQueryModel();
   QString querystr="select CardID from libaryCard where CardID='";
    QString temp="BookID in (select BookID from Records natural join libaryCard where CardID='";
    bool ok;
    bool ok1;
    int cnt=0;
    if(is_login==0)
        QMessageBox::warning(this, "Warning", "请先登录管理员账号");
    else{
        QString cardID = QInputDialog::getText(this, tr("借书管理"),tr("请输入借书证卡号"), QLineEdit::Password,0, &ok);
           if (ok && !cardID.isEmpty())
           {
               qDebug()<<cardID;
               querystr+=cardID;
               querystr+="'";
               cardIDmodel->setQuery(querystr);
               cnt=cardIDmodel->rowCount();
                qDebug()<<"Count="<<cnt;
                if(cnt!=0)
                {
                    temp+=cardID;
                    temp+="'";
                    temp+=")";
                    str2=temp;
                    qDebug()<<str2;
                   qury();
                     QString BookID = QInputDialog::getText(this, tr("借书管理"),tr("请输入所要借的书号"), QLineEdit::Password,0, &ok1);

                      if (ok1 && !BookID.isEmpty())
                      {

                           modifystorage(cardID,BookID,1);
                      }


                }
                else
                {
                    QMessageBox::warning(this, "Warning", "借书证卡号输入错误!");
                }

           }

    }



}

void MainWindow::on_return_2_clicked()
{
    cardIDmodel= new QSqlQueryModel();
    QString querystr="select CardID from libaryCard where CardID='";
     QString temp="BookID in (select BookID from Records natural join libaryCard where CardID='";
     bool ok;
     bool ok1;
     int cnt=0;
     if(is_login==0)
         QMessageBox::warning(this, "Warning", "请先登录管理员账号");
     else{
         QString cardID = QInputDialog::getText(this, tr("还书管理"),tr("请输入借书证卡号"), QLineEdit::Password,0, &ok);
            if (ok && !cardID.isEmpty())
            {
                qDebug()<<cardID;
                querystr+=cardID;
                querystr+="'";
                cardIDmodel->setQuery(querystr);
                cnt=cardIDmodel->rowCount();
                 qDebug()<<"Count="<<cnt;
                 if(cnt!=0)
                 {
                     temp+=cardID;
                     temp+="'";
                     temp+=")";
                     str2=temp;
                     qDebug()<<str2;
                    qury();
                      QString BookID = QInputDialog::getText(this, tr("还书管理"),tr("请输入所要还的书号"), QLineEdit::Password,0, &ok1);

                       if (ok1 && !BookID.isEmpty())
                       {

                            modifystorage(cardID,BookID,2);
                       }


                 }
                 else
                 {
                     QMessageBox::warning(this, "Warning", "借书证卡号输入错误!");
                 }

            }

     }
}

void MainWindow::on_pushin_clicked()
{
    QStringList bookinfo;
   //int i=0;
       int bookID;
       QDate D1;
       D1=QDate::currentDate();

       QMessageBox msgBox;
       msgBox.setText("请选择入库方式：");
       QPushButton *ADDButton = msgBox.addButton(tr("单册入库"), QMessageBox::ActionRole);
       QPushButton *ADD2Button = msgBox.addButton(tr("批量入库"), QMessageBox::ActionRole);
       QPushButton *CancelButton = msgBox.addButton(tr("取消"), QMessageBox::ActionRole);
      QSqlQuery query,query1;
      query1.exec("select max(BookID) FROM books");
      query1.next();
      bookID=query1.value(0).toInt();
      bookID++;
       //qDebug()<<listIDint;
     if(is_login==0)
         QMessageBox::warning(this, "Warning", "请先登录管理员账号");
      else{
       msgBox.exec();
      //single one
        if (msgBox.clickedButton() == ADDButton ) {
            qDebug()<<"add";
            QDialog dialog(this);
            QFormLayout form(&dialog);
            form.addRow (new QLabel("图书信息:"));
            // Value1
            QString value1 = "类型: ";
            QLineEdit *spinbox1 = new QLineEdit(&dialog);
            form.addRow(value1, spinbox1);
            // Value2
            QString value2 = "书名: ";
            QLineEdit *spinbox2 = new QLineEdit(&dialog);
            form.addRow(value2, spinbox2);
            // Value3
            QString value3 = "作者: ";
            QLineEdit *spinbox3 = new QLineEdit(&dialog);
            form.addRow(value3, spinbox3);
            // Value4
            QString value4= "出版社: ";
            QLineEdit *spinbox4 = new QLineEdit(&dialog);
            form.addRow(value4, spinbox4);
            // Value5
            QString value5 = "出版年份: ";
            QLineEdit *spinbox5 = new QLineEdit(&dialog);
            form.addRow(value5, spinbox5);
            // Value6
            QString value6= "价格: ";
            QLineEdit *spinbox6 = new QLineEdit(&dialog);
            form.addRow(value6, spinbox6);
            // Value7
            QString value7 = "总量: ";
            QLineEdit *spinbox7 = new QLineEdit(&dialog);
            form.addRow(value7, spinbox7);
            // Value8
            QString value8= "库存: ";
            QLineEdit *spinbox8 = new QLineEdit(&dialog);
            form.addRow(value8, spinbox8);
            // Add Cancel and OK button
            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);
            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
            if (dialog.exec() == QDialog::Accepted)
            {
             qDebug()<<"input ok";
             query.prepare("insert into books values(:bookID,:booktype,:bookname,:author,:publisher,:year,:prince,:total,:storage,:addtime)");
             query.bindValue(":bookID", bookID);
             query.bindValue(":booktype",spinbox1->text());
             query.bindValue(":bookname",spinbox2->text());
             query.bindValue(":author",spinbox3->text());
             query.bindValue(":publisher",spinbox4->text());
             query.bindValue(":year",spinbox5->text());
             query.bindValue(":prince",spinbox6->text());
             query.bindValue(":total",spinbox7->text());
             query.bindValue(":storage",spinbox8->text());
              query.bindValue(":addtime",D1);
              if(!query.exec()){
                  QMessageBox::critical(this,"infor","添加失败！");
                  return;
              }
              else
              {
                      QMessageBox::information(this,"infor","添加成功");
              }
            }
        }
        else if (msgBox.clickedButton() == ADD2Button) {
          qDebug()<<"批量入库";
          QFile file("E:/QT-library/test1/books.txt");
          if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {

              qDebug()<<"Can't open the file!"<<endl;
            }
             else
              {
                  QString line;
                  QTextStream in(&file);  //用文件构造流
                  line = in.readLine();//读取一行放到字符串里

                  while(!line.isNull())//字符串有内容
                  {
                       qDebug() << line;
                       bookinfo=line.split(',');
                        qDebug()<<bookinfo[1];
                        qDebug()<<bookID;
                       query.prepare("insert into books values(:bookID,:booktype,:bookname,:author,:publisher,:year,:prince,:total,:storage,:addtime)");
                       query.bindValue(":bookID", bookID);
                       query.bindValue(":booktype", bookinfo[0]);
                       query.bindValue(":bookname", bookinfo[1]);
                       query.bindValue(":author", bookinfo[2]);
                       query.bindValue(":publisher", bookinfo[3]);
                       query.bindValue(":year", bookinfo[4]);
                       query.bindValue(":prince", bookinfo[5]);
                       query.bindValue(":total", bookinfo[6]);
                       query.bindValue(":storage", bookinfo[7]);
                        query.bindValue(":addtime",D1);
                        if(!query.exec()){
                            QMessageBox::critical(this,"infor","添加失败！");
                            return;
                        }
                        else
                        {
                                qDebug()<<"add 1";
                        }
                        query1.exec("select max(BookID) FROM books");
                        query1.next();
                        bookID=query1.value(0).toInt();
                        bookID++;
                     line=in.readLine();//循环读取下行

              }
                file.close();
                QMessageBox::information(this,"infor","添加成功");


         }

}
      else if (msgBox.clickedButton() == CancelButton)
            return;
     }
}
void MainWindow::on_cardmanage_clicked()
{
     QDate D1;
     D1=QDate::currentDate();
   //  qDebug()<<"D1:"<<D1;
     QMessageBox msgBox;
     msgBox.setText("What do you want to do with the cards?");
     QPushButton *ADDButton = msgBox.addButton(tr("ADD"), QMessageBox::ActionRole);
     QPushButton *DELETEtButton = msgBox.addButton(tr("Delete"), QMessageBox::ActionRole);
    //adminmodel= new QSqlQueryModel();
    //QString querystr="select AdminID from administor where AdminID='";
    QSqlQuery query;
    if(is_login==0)
        QMessageBox::warning(this, "Warning", "请先登录管理员账号");
    else{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("借书证相关信息:"));
    // Value1
    QString value1 = "借书证号: ";
    QLineEdit *spinbox1 = new QLineEdit(&dialog);
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = "用户名: ";
    QLineEdit *spinbox2 = new QLineEdit(&dialog);
    form.addRow(value2, spinbox2);
    // Value3
    QString value3 = "学院: ";
    QLineEdit *spinbox3 = new QLineEdit(&dialog);
    form.addRow(value3, spinbox3);
    // Value4
    QString value4= "联系方式: ";
    QLineEdit *spinbox4 = new QLineEdit(&dialog);
    form.addRow(value4, spinbox4);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    if (dialog.exec() == QDialog::Accepted)
    {
        qDebug()<<"input ok";
         msgBox.exec();
    }

    //ADD and delete

      if (msgBox.clickedButton() == ADDButton ) {
          qDebug()<<"add";
           query.prepare("insert into libaryCard values(:cardID,:username,:Depart,:contact,:date)");
           query.bindValue(":cardID",spinbox1->text());
           query.bindValue(":username",spinbox2->text());
           query.bindValue(":Depart",spinbox3->text());
           query.bindValue(":contact",spinbox4->text());
            query.bindValue(":date",D1);
            if(!query.exec()){
                QMessageBox::critical(this,"infor","添加失败！");
                return;
            }
            else
            {
                    QMessageBox::information(this,"infor","添加成功");
            }





      }
      else if (msgBox.clickedButton() == DELETEtButton) {
           qDebug()<<"delete";
           query.prepare("select CardID from libaryCard where CardID=:cardID");
           query.bindValue(":cardID",spinbox1->text());
           query.exec();
           if(!query.next())
               QMessageBox::critical(this,"infor","卡号不存在！");
           else{
           query.prepare("delete from libaryCard where CardID=:cardID");
           qDebug()<<spinbox1->text();
           //delete from records where BookID=:BookID
           query.bindValue(":cardID",spinbox1->text());
            if(!query.exec()){
                QMessageBox::critical(this,"infor","删除失败！");
                return;
            }
            else
                    QMessageBox::information(this,"infor","删除成功");
           }


      }


}
}




void MainWindow::on_start_clicked()
{
     QString string[6];
     QString temp;
     string[0]="booktype=";
     string[1]="bookname=";
     string[2]="Author=";
     string[3]="publisher=";
     string[4]="Year=";
     string[5] ="prince between ";
     int add[6]={0};
    int i=0;
    int first=0;
    QStringListIterator strIterator(condition);
      while (strIterator.hasNext())
      {
          temp=strIterator.next();
          qDebug()<<temp;
         if(temp!="")
           {

               switch(i){
                case 0: string[0]+="'";string[0]+=temp;string[0]+="'";add[0]=1;break;
                case 1: string[1]+="'";string[1]+=temp;string[1]+="'"; add[1]=1; break;
                case 2: string[2]+="'";string[2]+=temp;string[2]+="'";add[2]=1; break;
                case 3: string[3]+="'";string[3]+=temp;string[3]+="'";  add[3]=1; break;
                case 4:    string[4]+=temp;add[4]=1; break;
                case 5:    string[5]+=temp; add[5]=1; break;
                case 6:  string[5]+=" and ";string[5]+=temp;break;
                default: break;
               }
           }
                i++;
               qDebug()<<"i="<<i<<endl;


      }

      for(i=0;i<6;i++)
      {

          if(add[i])
          {
              if(first==0)
              {
                  str2=string[i];
                 first=1;
              }
              else
              {
                  str2+=" and ";
                  str2+=string[i];
              }

          }

         qDebug()<<i<<" add[i]="<<add[i]<<" string[i]= "<<string[i]<<endl;

      }
    qDebug()<<str2;
          qury();
}


void MainWindow::Initcodition()
{
    condition<<NULL<<NULL<<NULL<<NULL<<NULL<<NULL<<NULL;

}
void MainWindow::on_type1_editingFinished()
{
    QString str = ui->type1->text();
    condition.replace(0,str);

}
void MainWindow::on_name2_editingFinished()
{
    QString str = ui->name2->text();
    condition.replace(1,str);
}



void MainWindow::on_author3_editingFinished()
{
    QString str = ui->author3->text();
    condition.replace(2,str);
}

void MainWindow::on_publish4_editingFinished()
{
    QString str = ui->publish4->text();
    condition.replace(3,str);
}

void MainWindow::on_year5_editingFinished()
{
    QString str = ui->year5->text();
    condition.replace(4,str);

}

void MainWindow::on_price1_6_editingFinished()
{
    QString str = ui->price1_6->text();
    condition.replace(5,str);
}

void MainWindow::on_price2_7_editingFinished()
{
    QString str = ui->price2_7->text();
    condition.replace(6,str);
}


