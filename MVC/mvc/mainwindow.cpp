#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // MVC 方案
    studentclass.setId("1");
    studentclass.setName("1班");

    // 为 model 设置标题
    QStringList headers;
    headers << "姓名" << "所属班级";
    model = new QStandardItemModel;
    model->setHorizontalHeaderLabels(headers);

    // 为 view 设置 model
    ui->listView->setModel(model);
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
   QString id = ui->lineEdit_id->text();
   QString name = ui->lineEdit_name->text();
   Student student;
   student.setId(id);
   student.setName(name);

   if(studentclass.add(student))
   {
       //创建model对应的item
       QStandardItem* itemName = new QStandardItem(name);
       QStandardItem* itemStudentClass = new QStandardItem(student.getName());
       // MVC 模式需要修改ui
//       ui->listWidget->addItem(student.getName());
       QList<QStandardItem*> itemList;
       itemList << itemName << itemStudentClass;
       model->appendRow(itemList);
   }
}
