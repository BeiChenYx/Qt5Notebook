#ifndef HANDLEHUMITURE_H
#define HANDLEHUMITURE_H

#include <QObject>
#include <QDate>
#include <QTimer>

/*
 * 暴露 C++ 对象给 QML, 对接后台处理线程
 * 1. 负责响应 QML 的触发信号；
 * 2. 负责响应工作线程的数据，将数据呈现给 QML;
 * 3. 负责工作线程的调度;
*/
class HandleHumiture : public QObject
{
    Q_OBJECT
public:
    explicit HandleHumiture(QObject *parent = nullptr);

    // TODO: 主页的历史记录查询和实时数据显示先不去处理，等处理完配置再看. yangx 2019.05.10;
signals:
    // 1. 串口 打开 和 关闭 的反馈
    void comOpenResult(int baud, int stopbits, QString parity, int databits);
    void comCloseResult(bool result);

    // 2. Modbus协议测试页面 反馈温度 和 反馈湿度 的反馈
    void humitureFeedback(QString temperature, QString humidity);

    // 3. 读取温湿度指定页面 响应的数据包
    void readCmd(QString data);

    // 4. 修改参数指令页面 响应的数据包
    void modifyCmd(QString data);

    // 发送数据到页面
    void humitureData(double temperature, double humidity);


public slots:
    // 0. 主页 响应 单点测试查询历史记录 的功能
    void onQueryRecord(int deviceAddr, QDate date);

    // 1. 串口参数设置页面: 响应 串口 打开和关闭功能
    void onOpenCom(QString com, int baud);
    void onCloseCom();

    // 2. Modbus协议测试页面: 响应 读取 和 连续读取
    void onReadHumiture(int deviceAddr, int registerAddr, int registerLen);
    void onTimesReadHumiture(int deviceStartAddr, int deviceStopAddr,
                             int registerAddr, int registerLen, int times);

    // 3. 读取温湿度指令页面
    void onReadCmd(int deviceAddr, int registerAddr, int registerLen);

    // 4. 修改参数指令页面
    void onModifyCmd(int deviceAddr, int registerAddr, int data);

    // 模拟modbus生产数据
    void gerneratorHumiture();

private:
    QTimer  *timer;
};

#endif // HANDLEHUMITURE_H
