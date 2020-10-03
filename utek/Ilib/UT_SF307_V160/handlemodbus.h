#ifndef HANDLEMODBUS_H
#define HANDLEMODBUS_H
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "Libmodbus/modbus.h"
#include <memory>


// 自动注销时间的默认值, 5分钟
const int AUTO_UNLOAD = 5 * 60 * 1000;
// 数据间隔读取的时间, 毫秒
const int TIMER_UPDATE_INTER = 5000;

// 报警信息一页数据个数
const int ALARM_PAGE_COUNT = 20;

// 用于控制当前登录的用户：
// 0:  未登录
// 1:  操作员
// 2:  管理员
// 3:  root
extern int g_indexUser;
// 开关控制状态
// 如二氧化碳开关关闭则主页二氧化碳数据为****
// 1.热回收热水比例阀开关
extern bool g_Hot;
// 2.二氧化碳传感器开关
extern bool g_CO2;
// 3.压差传感器开关
extern bool g_PA;
// 4.湿度传感器开关
extern bool g_Wet;
// 5.热回收机组旁通阀开关
extern bool g_HotGroup;
// 6.氨气是否显示的开关
extern bool g_NH3;
// 7. 舍内热回收器数量, 为0则主页不显示
extern int g_HotCount;

// 注销任务
extern bool g_LogOut;

/* 主要添加互斥锁，用来处理线程安全
 * Libmodbus 库参考文档:
 *      https://github.com/stephane/libmodbus/tree/master/doc
 */
class HandleModbus
{
public:
    HandleModbus();
    ~HandleModbus();
    bool openModbus(QString ip, int port=502);
    bool closeModbus();
    bool isOpen();
    QString getIp(){return this->m_ip;}
    int  readCoilStatus(int addr, int nbit, uint8_t *dest);
    int  readInputStatus(int addr, int nbit, uint8_t *dest);
    int  readInputRegisters(int addr, int nb, uint16_t *dest);
    int  readHoldingRegisters(int addr, int nb, uint16_t *dest);
    int  writeSingleCoil(int addr, int value);
    int  writeMultipleCoil(int addr, int nb, const uint8_t *src);
    int  writeSingleRegister(int addr, int value);
    int  writeMultipleRegisters(int addr, int nb, const uint16_t *src);
    QString errorString();

private:
    QMutex m_mutex;
    modbus_t *m_SlaveHandle;
    QString m_ip;
};

#endif // HANDLEMODBUS_H
