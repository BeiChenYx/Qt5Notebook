#include "readcmd.h"
#include "ui_readcmd.h"

ReadCmd::ReadCmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadCmd)
{
    ui->setupUi(this);
    ui->lineEdit_send->hide();
    ui->lineEdit_deviceAddr->setText("01");
    ui->lineEdit_registerAddr->setText("0000");
    ui->lineEdit_registerCount->setText("01");
}

ReadCmd::~ReadCmd()
{
    delete ui;
}

void ReadCmd::on_pushButton_read_clicked()
{
    int deviceAddr = ui->lineEdit_deviceAddr->text().toInt();
    int registerAddr = ui->lineEdit_registerAddr->text().toInt();
    int registerLen = ui->lineEdit_registerCount->text().toInt();
    Task task;
    task.m_page = 3;
    task.m_deviceAddr = deviceAddr;
    task.task_type = Task::TaskType::Read_Request;
    task.ReadRegisterType = Task::RegisterType::HoldingRegisters;
    task.ReadStartAddress = registerAddr;
    task.ReadLen = registerLen;
    QVariant msg;
    msg.setValue(task);
    emit readCmd(msg);
}

void ReadCmd::onReadCmdResult(QVariant msg)
{
    Task value = msg.value<Task>();
    QString data;
    foreach(uint16_t uch, value.ReadHolding){
        data += QString("%1").arg(uch);
    }
    ui->lineEdit_fb->setText(data);
}
