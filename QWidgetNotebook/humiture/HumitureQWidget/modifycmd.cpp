#include "modifycmd.h"
#include "ui_modifycmd.h"

ModifyCmd::ModifyCmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyCmd)
{
    ui->setupUi(this);
    ui->lineEdit_send->hide();
    ui->lineEdit_deviceAddr->setText("01");
    ui->lineEdit_registerAddr->setText("0000");
    ui->lineEdit_data->setText("01");
}

ModifyCmd::~ModifyCmd()
{
    delete ui;
}

void ModifyCmd::on_pushButton_write_clicked()
{
    int deviceAddr = ui->lineEdit_deviceAddr->text().toInt();
    int registerAddr = ui->lineEdit_registerAddr->text().toInt();
    int data = ui->lineEdit_data->text().toInt();
    Task task;
    task.m_page = 4;
    task.m_deviceAddr = deviceAddr;
    task.task_type = Task::TaskType::Write_Data;
    task.WriteRegisterType = Task::RegisterType::HoldingSingleRegister;
    task.WriteStartAddress = registerAddr;
    task.WriteHolding.append(static_cast<uint16_t>(data));
    QVariant msg;
    msg.setValue(task);
    emit modifyCmd(msg);
}

void ModifyCmd::onModifyCmdResult(QVariant msg)
{
    Task value = msg.value<Task>();
    QString data;
    foreach(uint16_t uch, value.WriteHolding){
        data += QString("%1").arg(uch);
    }
    ui->lineEdit_fb->setText(data);
}
