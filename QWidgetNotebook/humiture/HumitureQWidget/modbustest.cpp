#include "modbustest.h"
#include "ui_modbustest.h"

ModbusTest::ModbusTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModbusTest)
{
    ui->setupUi(this);
    ui->lineEdit_deviceAddr->setText("01");
    ui->lineEdit_registerAddr->setText("0000");
    ui->lineEdit_registerLen->setText("0001");
    ui->lineEdit_startAddr->setText("01");
    ui->lineEdit_endAddr->setText("10");
    ui->lineEdit_send->setText("010400000001");
    ui->lineEdit_times->setText("1000");
    // TODO: 发送和反馈目前不知道怎么配置,先隐藏
    ui->lineEdit_send->hide();
    ui->lineEdit_fb->hide();
    ui->label_12->hide();
    ui->label_13->hide();

    this->humitureTimer = new QTimer(this);
    connect(this->humitureTimer, &QTimer::timeout, this,
               &ModbusTest::timesReadHumiture);
}

ModbusTest::~ModbusTest()
{
    delete ui;
}

void ModbusTest::on_pushButton_Read_clicked()
{
    int deviceAddr = ui->lineEdit_deviceAddr->text().toInt();
    int registerAddr = ui->lineEdit_registerAddr->text().toInt();
    int registerLen = ui->lineEdit_registerLen->text().toInt();
    Task task;
    task.m_page = 2;
    task.m_deviceAddr = deviceAddr;
    task.task_type = Task::TaskType::Read_Request;
    task.ReadRegisterType = Task::RegisterType::InputRegisters;
    task.ReadStartAddress = registerAddr;
    task.ReadLen = registerLen;
    QVariant msg;
    msg.setValue(task);
    emit readHumiture(msg);
}

void ModbusTest::onModbusTestResult(QVariant msg)
{
    Task value = msg.value<Task>();
    // 提取温度，湿度等信息
    if(value.GetReadedLen() == 2){
        uint16_t temperatureInt = value.ReadHolding.at(0);
        uint16_t humidityInt = value.ReadHolding.at(1);
        double temperature = (temperatureInt - 4000) / 100.00;
        double humidity = humidityInt / 100.00;
        ui->lineEdit_T->setText(QString("%1").arg(temperature));
        ui->lineEdit_H->setText(QString("%1").arg(humidity));
    }else{// 一个寄存器就默认是温度
        uint16_t temperatureInt = value.ReadHolding.at(0);
        double temperature = (temperatureInt - 4000) / 100.00;
        ui->lineEdit_T->setText(QString("%1").arg(temperature));
    }
}

void ModbusTest::timesReadHumiture()
{
    int deviceStartAddr = ui->lineEdit_startAddr->text().toInt();
    int deviceStopAddr = ui->lineEdit_endAddr->text().toInt();
    int registerAddr = ui->lineEdit_registerAddr->text().toInt();
    int registerLen = ui->lineEdit_registerLen->text().toInt();
    for(int i = deviceStartAddr; i <= deviceStopAddr; i++){
        Task task;
        task.m_page = 2;
        task.m_deviceAddr = i;
        task.task_type = Task::TaskType::Read_Request;
        task.ReadRegisterType = Task::RegisterType::InputRegisters;
        task.ReadStartAddress = registerAddr;
        task.ReadLen = registerLen;
        QVariant msg;
        msg.setValue(task);
        emit readHumiture(msg);
    }
}

void ModbusTest::on_pushButton_times_clicked()
{
    int times = ui->lineEdit_times->text().toInt();
    if(ui->pushButton_times->text() == "连续读取"){
        ui->pushButton_times->setText("关闭");
        this->humitureTimer->start(times);
    }else {
        this->humitureTimer->stop();
        ui->pushButton_times->setText("连续读取");
    }
}
