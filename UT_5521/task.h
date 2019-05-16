#ifndef TASK_H
#define TASK_H

#include <QVariant>
#include <QString>
#include <QVector>
#include <QBitArray>

/*
 * Task类定义任务类型
 * 任务主要是操作modbus通信：
 *     打开设备，读/写各种寄存器，关闭设备
 */
class Task
{
public:
    struct TcpDevice{
        QString Ip;
        QString Port;
    };
    struct RtuDevice{
        int                     m_baud;
        int                     m_databits;
        int                     m_stopbits;

        // m_rsModel is MODBUS_RTU_RS232=0  or MODBUS_RTU_RS485=1
        int                     m_rsModel;
        // 串口描述符
        QString                 m_comDevice;
        QString                 m_parity;
    };

    // 任务类型
    enum TaskType {
        Invalids = 0,
        Open_Device = 1,
        Close_Device = 2,
        Write_Data = 3,
        Read_Request = 4,
        Exit_Thread = 5,
        Query_Record = 6,
    };

    // 寄存器类型
    enum RegisterType {
        invalids = 0,
        coils = 1,
        DiscreteInputs = 2,
        InputRegisters = 3,
        HoldingRegisters = 4,
        HoldingSingleRegister = 5
    };

    Task();
    Task(const Task &that);
    Task & operator =(const Task &What);

    // 获取需要写入的长度
    inline int GetWritLen()
    {
        if (WriteRegisterType == RegisterType::coils
                || WriteRegisterType == RegisterType::DiscreteInputs)
        {
            return WriteCoils.size();
        }else if(WriteRegisterType == HoldingRegisters
                 || WriteRegisterType == RegisterType::InputRegisters)
        {
            return WriteHolding.size();
        }else
        {
            return 0;
        }
    }

    // Get Readed len
    inline int GetReadedLen()
    {
        if (ReadRegisterType == RegisterType::coils
                || ReadRegisterType == RegisterType::DiscreteInputs)
        {
            return ReadCoils.size();
        }else if(ReadRegisterType == HoldingRegisters
                 || ReadRegisterType == RegisterType::InputRegisters)
        {
            return ReadHolding.size();
        }else
        {
            return 0;
        }
    }

protected:
    void copyFrom(const Task &that);
public:
    // Tip： 增加成员，需要修改拷贝构造和赋值构造
    // 各个任务所需要的成员对象
    // 用 Rtu 打开设备
    RtuDevice               rtuModbusInfo;
    // 用tcp的方式打开设备
    TcpDevice               tcpModbusInfo;
    // 当前设备是否使用 Rtu 方式打开，否则用 tcp 方式
    bool                    m_isRtu;
    // 任务类型
    TaskType                task_type;

    // 需要写入的参数
    RegisterType            WriteRegisterType;
    QBitArray               WriteCoils;
    QVector<uint16_t>       WriteHolding;
    int                     WriteStartAddress;

    // 需要读取的参数
    RegisterType            ReadRegisterType;
    QBitArray               ReadCoils;
    QVector<uint16_t>       ReadHolding;
    int                     ReadStartAddress;
    int                     ReadLen;
    int                     m_deviceAddr;
    QString                 m_queryDate;

    // Tip: 针对 UT-5521 不同页面发出的任务请求，但功能码地址等都一样的情况
    // [0, 1, 2, 3, 4]
    int                     m_page;
};
Q_DECLARE_METATYPE(Task)

#endif // TASK_H
