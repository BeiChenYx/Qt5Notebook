#ifndef IM_MODBUS_H
#define IM_MODBUS_H

/*
 * 文件名: IM_modbus(Instant messaging modbus)
 * 描述：本类目的是在多个窗口中进行通信
 * TODO: 线程中执行所有任务，任务输入到队列中，
 *     任务执行的结果放到列表中，列表保存寄存器类别，
 * 寄存器开始地址，以及处理的结果；外部通过向队列中插入任务
 * Tip:
 *    在  xxx.pro  工程文件中增加下面两个配置:
 *          LIBS     += -Ldll -lws2_32
 *          LIBS     += -lwsock32
 */
#include <QThread>
#include <QVariant>
#include <QString>
#include <QVector>
#include <QBitArray>
#include <errno.h>
#include <exception>
#include <QQueue>
#include <QMutexLocker>
#include <QMutex>
#include <QDateTime>
#include <winsock2.h>
#include <windows.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QCoreApplication>
#include "Libmodbus/modbus.h"

// 处理数据库
//#include "handlerecord.h"

extern int errno;
// 全局变量存在问题...comConfig中定义了一个
const int DEVICE_MAX_COUNT = 10;
// 在线设备间隔检查时间 单位: 分钟
const int DEVICE_CHECK_ONLINE_TIME = 5;
// modbus 通信最长等待时间, 单位: 秒
const int MODBUS_TIMEOUT = 1;

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

    // Tip: 针对 UT-5521 不同页面发出的任务请求，但功能码地址等都一样的情况
    // [0, 1, 2, 3, 4]
    int                     m_page;
    // 当前设备是否使用 Rtu 方式打开，否则用 tcp 方式
    bool                    m_isRtu;
    bool                    m_isRecord;
    bool                    m_ClearQueue;
    QString                 m_queryDate;
};
Q_DECLARE_METATYPE(Task)


struct HumitureRecord{
    int deviceId;
    double temperature;
    double humidity;
    qint64 time;
    QString date;
    bool isRecord;
};
Q_DECLARE_METATYPE(HumitureRecord)

/* doc:
 * https://github.com/stephane/libmodbus/tree/master/doc
 * 经过测试此库一次最多写入123 * sizeof(uint16_t) 字节
 */
class ModbusOptation
{
public:
    // TODO: 先处理 RTU 通信，TCP 通信等日后再完善 2019-05-09 yangx
    // 主要处理 UT-5521 的温湿度展示，最多有 16 个从机
    // TODO: 尽量都改成 Qt 风格的代码
    ModbusOptation();
    ~ModbusOptation();

    // rsModel: MODBUS_RTU_RS232 和 MODBUS_RTU_RS485
    bool CreateModbusHandle_rtu(QString device, int baud,
                                QString parity, int databits,
                                int stopbits, int rsModel);

    bool setSlaveId(int deviceId=1);

    void CloseModbus();

    int WriteToModbus(Task &info);
    int ReadFromModbus(Task &info);

    // 功能码 0x01
    int  ReadCoilStatus(int addr, int nbit, uint8_t *dest);

    // 功能码 0x02
    int  ReadInputStatus(int addr, int nbit, uint8_t *dest);

    // 功能码 0x03
    int  ReadHoldingRegisters(int addr, int nb, uint16_t *dest);

    // 功能码 0x04
    int  ReadInputRegisters(int addr, int nb, uint16_t *dest);

    // 功能码 0x05
    int  WriteSingleCoil(int addr, int value);

    // 功能码 0x06
    int  WriteSingleRegister(int addr, int value);

    // 功能码 0x0F
    int  WriteMultipleCoil(int addr, int nb, const uint8_t *src);

    // 功能码 0x16
    int  WriteMultipleRegisters(int addr, int nb, const uint16_t *src);

    // 处理错误 POSIX systems 上才有效
    QString ErrorString();
public:
    bool               m_isOpen;

private:
    modbus_t           *m_SlaveHandle;
    QString            m_Ip;
    int                m_port;
    mutable            std::mutex mut; // 1 互斥量必须是可变的
};



class WorkThread : public QThread
{
    Q_OBJECT

public:
    WorkThread();

    // 写入任务队列，参数是Task类型
    void pushToTask(Task &task);
    Task popFromTask();

signals:
    // 通过信号向外发送处理结果
    // Libmodbus 错误是根据 errno 机制，windows没有，因此不处理错误原因
    void openResult(bool result, QVariant msg);
    // Libmodbus 关闭没有返回值
    void closeResult();
    void homeResult(QVariant msg);
    void modbusTestResult(QVariant msg);
    void readCmdResult(QVariant msg);
    void modifyCmdResult(QVariant msg);
    void recordResult(QVariant msg);
    void exitModbus();

protected:
     /* run 是线程的执行体， 主线程通过控制m_Task
     * 来分配子线程的任务子线程通过发送信号输出处
     * 理结果, modbus的任何操作都需要放到子线程中
     * 创建即run函数中创建,run函数是个死循环，
     * 因此会导致信号槽失效,只能在run中发出，无法收到 */
    void run();

private:
    bool handleOpenDevice(ModbusOptation *pdevice, Task & task);
    int handleReadRquest(ModbusOptation *pdevice, Task & task, bool isRecord);
    bool handleInsertRecord(Task & task);
    QList<HumitureRecord> handleQueryRecord(int deviceAddr, QString date);

private:
    QQueue<Task>   m_Task;
    QMutex         m_mutex;
//    HandleRecord   *m_phandleRecord;
    QVector<bool>   m_onlineDevice;
};


#endif // IM_MODBUS_H

