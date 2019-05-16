#include "handlehumiture.h"
#include <QtDebug>
#include <QList>

HandleHumiture::HandleHumiture(QObject *parent) : QObject(parent)
{
    m_pModbusThread = new WorkThread();
    m_readHumitureTimer = new QTimer(this);
    m_homeHumitureTimer = new QTimer(this);
    this->initConnect();

    // Tip: 线程启动需要放在线程对象信号槽连接之后;
    m_pModbusThread->start();
}

void HandleHumiture::initConnect()
{
    connect(m_pModbusThread, SIGNAL(openResult(bool, QVariant)),
            this, SLOT(onOpenResult(bool, QVariant)));
    connect(m_pModbusThread, SIGNAL(closeResult()),
            this, SLOT(onCloseResult()));
    connect(m_pModbusThread, SIGNAL(modbusTestResult(QVariant)),
            this, SLOT(onModbusTestResult(QVariant)));
    connect(m_pModbusThread, SIGNAL(readCmdResult(QVariant)),
            this, SLOT(onReadCmdResult(QVariant)));
    connect(m_pModbusThread, SIGNAL(modifyCmdResult(QVariant)),
            this, SLOT(onModifyCmdResult(QVariant)));
    connect(m_pModbusThread, SIGNAL(exitModbus()),
            this, SLOT(onExitModbus()));
    connect(m_readHumitureTimer, &QTimer::timeout, this,
            &HandleHumiture::timesReadHumiture);
    connect(m_homeHumitureTimer, &QTimer::timeout, this,
            &HandleHumiture::timesHomeHumiture);
}


void HandleHumiture::onOpenCom(QString com, int baud)
{
    qDebug() << "Com: " << com << ", baud: " << baud;
    Task task;
    task.m_page = 1;
    task.task_type = Task::TaskType::Open_Device;
    task.m_isRtu = true;
    task.rtuModbusInfo.m_baud = baud;
    task.rtuModbusInfo.m_comDevice = com;
    task.rtuModbusInfo.m_parity = "N";
    task.rtuModbusInfo.m_rsModel = 1;
    task.rtuModbusInfo.m_databits = 8;
    task.rtuModbusInfo.m_stopbits = 1;
    m_pModbusThread->pushToTask(task);
}

void HandleHumiture::onCloseCom()
{
    qDebug() << "Close Com;";
    Task task;
    task.m_page = 1;
    task.task_type = Task::TaskType::Close_Device;
    m_pModbusThread->pushToTask(task);
}

void HandleHumiture::onReadHumiture(int deviceAddr, int registerAddr, int registerLen)
{
    qDebug() << "deviceAddr: " << deviceAddr << ", registerAddr: "
             << registerAddr << ", registerLen: " << registerLen;
    Task task;
    task.m_page = 2;
    task.m_deviceAddr = deviceAddr;
    task.task_type = Task::TaskType::Read_Request;
    task.ReadRegisterType = Task::RegisterType::InputRegisters;
    task.ReadStartAddress = registerAddr;
    task.ReadLen = registerLen;
    m_pModbusThread->pushToTask(task);
}

void HandleHumiture::onTimesReadHumiture(int deviceStartAddr, int deviceStopAddr,
                                         int registerAddr, int registerLen, int times)
{
    qDebug() << "deviceStartAddr: " << deviceStartAddr << ", deviceStopAddr: "
             << deviceStopAddr << ", registerAddr: " << registerAddr
             << ", registerLen: " << registerLen << ", times: " << times;
    if(times){
        m_readHumitureTimer->start(times);
    }else{
        m_readHumitureTimer->stop();
    }
}

void HandleHumiture::onReadCmd(int deviceAddr, int registerAddr, int registerLen)
{
    qDebug() << "deviceAddr: " << deviceAddr << ", registerAddr: "
             << registerAddr << ", registerLen: " << registerLen;
    Task task;
    task.m_page = 3;
    task.m_deviceAddr = deviceAddr;
    task.task_type = Task::TaskType::Read_Request;
    task.ReadRegisterType = Task::RegisterType::HoldingRegisters;
    task.ReadStartAddress = registerAddr;
    task.ReadLen = registerLen;
    m_pModbusThread->pushToTask(task);
}

void HandleHumiture::onModifyCmd(int deviceAddr, int registerAddr, int data)
{
    qDebug() << "deviceAddr: " << deviceAddr << ", registerAddr: "
             << registerAddr << ", data: " << data;
    Task task;
    task.m_page = 4;
    task.m_deviceAddr = deviceAddr;
    task.task_type = Task::TaskType::Read_Request;
    task.ReadRegisterType = Task::RegisterType::HoldingSingleRegister;
    task.ReadStartAddress = registerAddr;
    task.WriteHolding.append(static_cast<uint16_t>(data));
    m_pModbusThread->pushToTask(task);
}

void HandleHumiture::onOpenResult(bool result, QVariant msg)
{
    Task value = msg.value<Task>();
    emit comOpenResult(result, value.rtuModbusInfo.m_baud,
                       value.rtuModbusInfo.m_stopbits,
                       value.rtuModbusInfo.m_parity,
                       value.rtuModbusInfo.m_databits);
    m_homeHumitureTimer->start(1);
}

void HandleHumiture::onCloseResult()
{
    emit comCloseResult();
}

void HandleHumiture::onModbusTestResult(QVariant msg)
{
    Task value = msg.value<Task>();
    // TODO: 是否需要在连续读取的任务中更新设备地址？先保留；
    // 提取温度，湿度等信息
   uint16_t temperatureInt = value.ReadHolding.at(0);
   uint16_t humidityInt = value.ReadHolding.at(1);
   double temperature = (temperatureInt - 4000) / 100.00;
   double humidity = humidityInt / 100.00;
   emit humitureFeedback(QString("%1").arg(temperature, 0, 'f', 2),
                         QString("%1").arg(humidity, 0, 'f', 2));

}

void HandleHumiture::onReadCmdResult(QVariant msg)
{
    Task value = msg.value<Task>();
    QString data;
    foreach(uint16_t uch, value.ReadHolding){
        data += QString("%1").arg(uch);
    }
    emit readCmdFeedback(data);
}

void HandleHumiture::onModifyCmdResult(QVariant msg)
{
    Task value = msg.value<Task>();
    QString data;
    foreach(uint16_t uch, value.ReadHolding){
        data += QString("%1").arg(uch);
    }
    emit modifyCmdFeedback(data);
}

void HandleHumiture::onExitModbusResult()
{
    emit comCloseResult();
}

void HandleHumiture::timesReadHumiture()
{
    // 定时生成 ReadHumiture 任务
    for(int i = m_deviceStartAddr; i <= m_deviceStopAddr; i++){
        Task task;
        task.m_page = 2;
        task.m_deviceAddr = i;
        task.task_type = Task::TaskType::Read_Request;
        task.ReadRegisterType = Task::RegisterType::InputRegisters;
        task.ReadStartAddress = m_registerAddr;
        task.ReadLen = m_registerLen;
        m_pModbusThread->pushToTask(task);
    }
}

void HandleHumiture::timesHomeHumiture()
{
    for(int i = 1; i <= 3; i++){
        Task task;
        task.m_page = 0;
        task.m_deviceAddr = i;
        task.task_type = Task::TaskType::Read_Request;
        task.ReadRegisterType = Task::RegisterType::InputRegisters;
        task.ReadStartAddress = 0;
        task.ReadLen = 2;
        m_pModbusThread->pushToTask(task);
    }
}
