#ifndef HANDLEHUMITURE_H
#define HANDLEHUMITURE_H

#include <QObject>
#include <QDate>
#include <QTimer>
#include "IM_modbus.h"

extern  const int DEVICE_MAX_COUNT;

/*
 * 暴露 C++ 对象给 QML, 对接后台处理线程
 * 1. 负责响应 QML 的触发信号；
 * 2. 负责工作线程的调度;
 *
 * 只处理 串口配置 和 功能设置，不包含 主页数据，历史记录
*/
class HandleHumiture : public QObject
{
    Q_OBJECT
public:
    explicit HandleHumiture(QObject *parent = nullptr);

    void initConnect();

signals:
    // 将后台处理的任务结果 通过信号 和 槽的方式发送懂啊 QML 中
    // 1. 串口 打开 和 关闭 的反馈
    void comOpenResult(bool result, int baud, int stopbits, QString parity, int databits);
    void comCloseResult();

    // 2. Modbus协议测试页面 反馈温度 和 反馈湿度 的反馈
    void humitureFeedback(QString temperature, QString humidity);

    // 3. 读取温湿度指定页面 响应的数据包
    void readCmdFeedback(QString data);

    // 4. 修改参数指令页面 响应的数据包
    void modifyCmdFeedback(QString data);

    void testSlot(int num);

public slots:
    // 用来响应 QML 页面的任务
    // 1. 串口参数设置页面: 响应 串口 打开和关闭功能
    void onOpenCom(QString com, int baud);
    void onCloseCom();

    // 2. Modbus协议测试页面: 响应 读取 和 连续读取
    void onReadHumiture(int deviceAddr, int registerAddr, int registerLen);
    // times == 0 就关闭计时器
    void onTimesReadHumiture(int deviceStartAddr, int deviceStopAddr,
                             int registerAddr, int registerLen, int times);

    // 3. 读取温湿度指令页面
    void onReadCmd(int deviceAddr, int registerAddr, int registerLen);

    // 4. 修改参数指令页面
    void onModifyCmd(int deviceAddr, int registerAddr, int data);

    // WorkThread 反馈结果的槽
    void onOpenResult(bool result, QVariant msg);
    void onCloseResult();
    void onModbusTestResult(QVariant msg);
    void onReadCmdResult(QVariant msg);
    void onModifyCmdResult(QVariant msg);
    void onExitModbusResult();
    // 定时器的槽
    void timesReadHumiture();
    void timesHomeHumiture();
private:
    // Modbus协议测试页面，连续读取所需要的参数
    int             m_deviceStartAddr;
    int             m_deviceStopAddr;
    int             m_registerAddr;
    int             m_registerLen;

    WorkThread      *m_pModbusThread;
    QTimer          *m_readHumitureTimer;
    QTimer          *m_homeHumitureTimer;
};

#endif // HANDLEHUMITURE_H
