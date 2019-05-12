#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QStringList>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // 查询支持的数据库类型
//    qDebug() << "Available drivers: ";
//    QStringList drivers = QSqlDatabase::drivers();
//    foreach(QString driver, drivers){
//        qDebug() << driver;
//    }
//    createConnection();

    return a.exec();
}
