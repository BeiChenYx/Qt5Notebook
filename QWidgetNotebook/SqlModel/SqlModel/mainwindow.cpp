#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "connection.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    createConnection();

//    QSqlQuery query;
//    query.exec("select * from student");
//    while (query.next()) {
//        qDebug() << query.value(0).toInt() << ": " << query.value(1).toString();
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // create
    createDatabaseConnection();
}

void MainWindow::on_pushButton_2_clicked()
{
    // query
    // 获取 静态库创建的连接， 连接名字 student1
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    // 查询数据
    query.exec("select * from student");
    // 直接打印
    while (query.next()) {
        qDebug() << query.value(0).toInt() << ": " << query.value(1).toString();
    }

    // 使用 model
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from student");
    model->setHeaderData(0, Qt::Horizontal, tr("序号"));
    model->setHeaderData(1, Qt::Horizontal, tr("名字"));
    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_Insert_clicked()
{
    // insert
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    qDebug() << query.lastError();
    int id = ui->lineEdit_id->text().toInt();
    QString name = ui->lineEdit_name->text();
    QString sql = QString("insert into student values (%1, '%2')").arg(id).arg(name);
    query.exec(sql);
    qDebug() << query.lastError();

}

bool MainWindow::createDatabaseConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(":memory:");
    QString path = QCoreApplication::applicationDirPath();
    db.setDatabaseName(path + "/SqlModel.db");
//    db.setDatabaseName("/Users/yx/WorkSpace/Qt5Notebook/Core/SqlModel/SqlModel/SqlModel.db");
    if(!db.open()){
        qDebug() << "Can not open database;";
        return  false;
    }
    qDebug() << "Database is open.";
    return true;
}

bool MainWindow::createTable(QString tableName)
{
    QSqlQuery query(db);
    query.exec("Create table student (id int primary key, name varchar(20))");
}
