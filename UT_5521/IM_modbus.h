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
#include "task.h"
#include "realhumiture.h"
//#include <winsock2.h>
//#include <windows.h>
//#include "Libmodbus/modbus.h"

// 处理数据库
//#include "handlerecord.h"

extern int errno;
const int DEVICE_MAX_COUNT = 3;
// 在线设备间隔检查时间 单位: 分钟
const int DEVICE_CHECK_ONLINE_TIME = 5;
// modbus 通信最长等待时间, 单位: 秒
const int MODBUS_TIMEOUT = 1;



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
//    bool CreateModbusHandle_tcp(QString ip, int port);

    // rsModel: MODBUS_RTU_RS232 和 MODBUS_RTU_RS485
//    bool CreateModbusHandle_rtu(QString device, int baud,
//                                QString parity, int databits,
//                                int stopbits, int rsModel);

//    bool setSlaveId(int deviceId=1);

//    void CloseModbus();

//    int WriteToModbus(Task &info);
//    int ReadFromModbus(Task &info);

    // 功能码 0x01
//    int  ReadCoilStatus(int addr, int nbit, uint8_t *dest);

    // 功能码 0x02
//    int  ReadInputStatus(int addr, int nbit, uint8_t *dest);

    // 功能码 0x03
//    int  ReadHoldingRegisters(int addr, int nb, uint16_t *dest);

    // 功能码 0x04
//    int  ReadInputRegisters(int addr, int nb, uint16_t *dest);

    // 功能码 0x05
//    int  WriteSingleCoil(int addr, int value);

    // 功能码 0x06
//    int  WriteSingleRegister(int addr, int value);

    // 功能码 0x0F
//    int  WriteMultipleCoil(int addr, int nb, const uint8_t *src);

    // 功能码 0x16
//    int  WriteMultipleRegisters(int addr, int nb, const uint16_t *src);

    // 处理错误 POSIX systems 上才有效
//    QString ErrorString();

private:
//    modbus_t           *m_SlaveHandle;
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
//    void comConfigResult(QVariant msg);
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
    int handleReadRquest(ModbusOptation *pdevice, Task & task);
//    bool handleInsertRecord(HandleRecord *phandle, Task & task);

private:
    QQueue<Task>   m_Task;
    QMutex         m_mutex;
    RealHumiture   *m_prealHumiture;
//    HandleRecord   *m_phandleRecord;
    QVector<bool>   m_onlineDevice;
};


#endif // IM_MODBUS_H

