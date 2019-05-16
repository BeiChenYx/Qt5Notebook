#include "task.h"

const int MODBUS_RTU_RS485 = 1;
Task::Task()
{
    task_type = TaskType::Invalids;
    tcpModbusInfo.Ip = "127.0.0.1";
    tcpModbusInfo.Port = "502";
    rtuModbusInfo.m_comDevice = "COM1";
    rtuModbusInfo.m_baud = 9600;
    rtuModbusInfo.m_databits = 8;
    rtuModbusInfo.m_stopbits = 1;
    rtuModbusInfo.m_parity = "N";
    rtuModbusInfo.m_rsModel = MODBUS_RTU_RS485;
    this->m_isRtu = true;
    WriteRegisterType = RegisterType::invalids;
    WriteCoils.clear();
    WriteHolding.clear();
    WriteStartAddress = 0;
    ReadRegisterType = RegisterType::invalids;
    ReadStartAddress = 0;
    ReadLen = 0;
    m_deviceAddr = 1;

    // 针对 UT-5521 的项目
    m_page = 0;
}

void Task::copyFrom(const Task &that)
{
    this->task_type = that.task_type;
    this->tcpModbusInfo = that.tcpModbusInfo;
    this->rtuModbusInfo = that.rtuModbusInfo;
    this->WriteRegisterType = that.WriteRegisterType;
    this->WriteStartAddress = that.WriteStartAddress;
    this->ReadRegisterType = that.ReadRegisterType;
    this->ReadStartAddress = that.ReadStartAddress;
    this->ReadLen = that.ReadLen;
    this->m_isRtu = that.m_isRtu;
    this->m_deviceAddr = that.m_deviceAddr;

    // 针对 UT-5521 项目
    this->m_page = that.m_page;

    if(ReadRegisterType == RegisterType::coils
            || ReadRegisterType == RegisterType::DiscreteInputs)
    {
        this->ReadCoils.clear();
        this->ReadCoils.resize(that.ReadCoils.size());
//        qDebug() << "that.ReadCoils size is : " << that.ReadCoils.size();
        for(int i = 0; i < that.ReadCoils.size(); i++)
        {
            bool bit = that.ReadCoils.at(i);
//            qDebug() << "this->ReadCoils size is : " << this->ReadCoils.size();
            this->ReadCoils.setBit(i, bit);
        }
    }else{
        this->ReadHolding.clear();
        this->ReadHolding = that.ReadHolding;
    }

    if(WriteRegisterType == RegisterType::coils
            || WriteRegisterType == RegisterType::DiscreteInputs)
    {
        this->WriteCoils.clear();
        this->WriteCoils.resize(that.WriteCoils.size());
        for(int i = 0; i < that.WriteCoils.size(); i++)
        {
            this->WriteCoils.setBit(i, that.WriteCoils.at(i));
        }
    }else {
        this->WriteHolding.clear();
        this->WriteHolding = that.WriteHolding;
    }
}

Task::Task(const Task &that)
{
    copyFrom(that);
}

Task & Task::operator =(const Task &what)
{
    if(&what != this)
    {
        copyFrom(what);
    }
    return *this;
}
