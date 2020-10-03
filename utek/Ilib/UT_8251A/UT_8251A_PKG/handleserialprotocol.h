#ifndef HANDLESERIALPROTOCOL_H
#define HANDLESERIALPROTOCOL_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QDateTime>
#include <memory>
#include "handleserialport.h"


#pragma pack(1)
// 串口通信对应的协议
struct TransitionParamREQCom{
    // pc 2 mcu, 设置转换参数
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar transitionModel;      // 转换模式
    uchar transitionDirecte;    // 转换方向
    uchar enableSerial;         // 0x00 不允许CAN帧信息转发到串行帧中, 0x01 允许
    uchar enableID;             // 0x00 不允许CAN帧ID转发到串行帧中, 0x01 允许
    uchar serialOffset;         // CAN标识符在串行帧中的起始偏移
    uchar serialFramLen;        // CAN标识符在串行帧中的长度
    uchar serialTimes;          // 串行帧之间的时间间隔，单位 毫秒
    uchar transitionTimes;      // CAN帧转发的时间间隔, 单位 毫秒
    uchar checkSum;             // 校验和
};
struct GeneralPkgCom{
    // 通用的数据包，包含一个有效的功能命令
    uchar header[2]{0x2E, 0x30};// 包头
    uchar len{0x03};            // 包长度
    uchar cmd;                  // 功能命令
    uchar checkSum;             // 校验和
};
struct GeneralPkgReCom{
    // 通用的数据包，包含一个有效的功能命令
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar re;                   // 返回执行的结果： 0x00 失败, 0x01 成功
    uchar checkSum;             // 校验和
};
struct CANParamREQCom{
    // pc 2 mcu, 设置CAN参数
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar bauterate;            // CAN通信波特率
    uchar canID[4];             // CAN帧ID， 高字节在前
    uchar frameFormat;          // 帧格式: 0x00 标准帧, 0x01 扩展帧
    uchar checkSum;             // 校验和
};

struct FilterData{
    // 滤波规则数据
    uchar idFilterType;         // CAN帧ID滤波类型
    uchar startID[4];           // 起始ID
    uchar endID[4];             // 结束ID
};
struct CANFilterREQCom{
    // pc 2 mcu, 设置CAN帧ID滤波器组
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
//    FilterData *filterData;    // 过滤规则的数据
//    uchar checkSum;             // 校验和
};
struct USARTParamREQCom{
    // pc 2 mcu, 设置串口参数
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar bauderate;            // 串口波特率
    uchar dataBit;              // 数据位
    uchar parityBit;            // 校验位
    uchar stopBit;              // 停止位
    uchar flowControl;          // 流控制
    uchar serailModel;          // 串口模式: 0x00 RS232, 0x01 RS485, 0x02 RS422
    uchar checkSum;             // 校验和
};
struct USARTComfigACKCom{
    // mcu 2 pc, 收到的MCU发送的USART当前配置
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar transitionModel;      // 转换模式
    uchar transitionDirecte;    // 转换方向
    uchar enableSerial;         // 0x00 不允许CAN帧信息转发到串行帧中, 0x01 允许
    uchar enableID;             // 0x00 不允许CAN帧ID转发到串行帧中, 0x01 允许
    uchar serialOffset;         // CAN标识符在串行帧中的起始偏移
    uchar serialFramLen;        // CAN标识符在串行帧中的长度
    uchar serialTimes;          // 串行帧之间的时间间隔，单位 毫秒
    uchar transitionTimes;      // CAN帧转发的时间间隔, 单位 毫秒
    uchar bauderate;            // 串口波特率
    uchar dataBit;              // 数据位
    uchar parityBit;            // 校验位
    uchar stopBit;              // 停止位
    uchar flowControl;          // 流控制
    uchar serailModel;          // 串口模式: 0x00 RS232, 0x01 RS485, 0x02 RS422
    uchar canBauderate;         // CAN通信波特率
    uchar canID[4];             // CAN帧ID， 高字节在前
    uchar frameFormat;          // 帧格式: 0x00 标准帧, 0x01 扩展帧
    uchar filterCount;          // 过滤ID组个数
//    FilterData *filterData;    // 过滤规则的数据
//    uchar checkSum;             // 校验和
};
struct USARTInitInfoREQCom{
    // mcu 2 pc,发送设备初始化信息
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar mVer;                 // 主板本号
    uchar pVer;                 // 次版本号
    uchar checkSum;             // 校验和
};
struct UpgradeREQCom{
    // pc 2 mcu, 发送升级请求
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar fileSize[4];          // 文件大小, 高字节在前
    uchar checkSum;             // 校验和
};
struct USARTIapDataREQCom{
    // pc 2 mcu, 发送升级数据包
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar id[4];                // 当前发送的数据包编号
    uchar pkgLen;               // 有效数据包长度
    uchar data[128];            // 升级数据包
    uchar checkSum;             // 校验和
};
#pragma pack()

enum SerialTask{
    TRANSITION_PAR_CAN = 0,
    PAR_CAN,
    FILTER_CAN,
    CLEAR_FILTER,
    SERIAL_PAR,
    MCU_CONFIG,
    INIT_CAN,
    IS_ALIVE,
    UPDATE_DEVICE_COM,
};

struct FilterConfig{
    QString idFilterType;      // CAN帧ID滤波类型
    QString startID;           // 起始ID
    QString endID;             // 结束ID
};

struct SerialTaskPar{
    QString transitionModel;            // 转换模式
    QString transitionDirecte;          // 转换方向
    QString enableSerial;                  // 0x00 不允许CAN帧信息转发到串行帧中, 0x01 允许
    QString enableID;                      // 0x00 不允许CAN帧ID转发到串行帧中, 0x01 允许
    int serialOffset;                   // CAN标识符在串行帧中的起始偏移
    int serialFramLen;                  // CAN标识符在串行帧中的长度
    int serialTimes;                    // 串行帧之间的时间间隔，单位 毫秒
    int transitionTimes;                // CAN帧转发的时间间隔, 单位 毫秒

    QString bauterateCan;               // CAN通信波特率
    QString canID;                      // CAN帧ID， 高字节在前
    QString frameFormat;                // 帧格式: 0x00 标准帧, 0x01 扩展帧

    QVector<FilterConfig> filterData;     // 过滤规则的数据

    QString com;                        // 串口号
    QString bauderateSerial;            // 串口波特率

    QString dataBit;                    // 数据位
    QString parityBit;                  // 校验位
    QString stopBit;                    // 停止位
    QString flowControl;                // 流控制
    QString serailModel;                // 串口模式: 0x00 RS232, 0x01 RS485, 0x02 RS422

    QString filePath;                   // 升级文件的路径
};
Q_DECLARE_METATYPE(SerialTaskPar)


class HandleSerialProtocol : public QObject
{
    Q_OBJECT
public:
    explicit HandleSerialProtocol(QObject *parent = nullptr);
    ~HandleSerialProtocol();
    bool open(SerialTaskPar &par);
    void close();

    // 设置转换参数
    bool setTransitionPar(SerialTaskPar &par);
    // 设置CAN参数
    bool setCanPar(SerialTaskPar &par);
    // 设置滤波规则
    bool setFilter(SerialTaskPar &par);
    // 清除滤波
    bool clearfilter();
    // 设置串口参数
    bool setSerialPar(SerialTaskPar &par);
    // 读取MCU配置参数的请求
    bool readMcuConfig();
    // 发送心跳包
    bool isAlive();
    // 打开CAN
    bool openCan();
    // 初始化CAN，并获取版本号
    bool initCan();
    // 升级设备
    bool upgradeDev(QString filePath);

signals:
    void isTransitionParCan(bool status);
    void isParCan(bool status);
    void isFilterCan(bool status);
    void isClearFilter(bool status);
    void isSerialPar(bool status);
    void isOpenCan(bool status);
    void isMcuConfig(SerialTaskPar rst, bool re);
    void isInitCan(QString version, bool status);
    void isAliveCan(bool status);

    void updateStatus(QString msg);
    void updateResult(bool status);
    void processRange(int min, int max);
    void processBarV(int v);

    void readTimeout();

private slots:
    void onReadyBytes(QByteArray bytes);
    unsigned char checkSum(unsigned char *buff, int len);

    bool onSetTransitionPar(QByteArray &bytes);
    bool onSetCanPar(QByteArray &bytes);
    bool onSetFilter(QByteArray &bytes);
    bool onSetSerialPar(QByteArray &bytes);
    bool onReadMcuConfig(QByteArray &bytes);
    bool onIsAlive(QByteArray &bytes);
    bool onOpenCan(QByteArray &bytes);
    bool onInitCan(QByteArray &bytes);

    // 升级
    bool upgradeDevReq();
    bool upgradeDevData();

    bool onUpgradeDevReq(QByteArray &bytes);
    bool onUpgradeDevData(QByteArray &bytes);
    bool onUpgradeDevStatus(QByteArray &bytes);

    void setTimeOutEnable(bool isOpen=false){
        m_isReq = isOpen;
        if(isOpen){m_reqTime = QDateTime::currentMSecsSinceEpoch();}
    }

private:
    std::shared_ptr<HandleSerialPort> m_pHandleSerial;
    // 开启请求, 在REQ里面设置为true, 在ACK中设置false
    volatile bool m_isReq = false;
    // 用来计算超时
    volatile qint64 m_reqTime;
    // 定时器检测超时
    QTimer m_reqTimer;

    bool m_isSetFilter = true;

    // 升级
    QByteArray m_fileBuf;
    const int m_pkgLen = 128;
    uint32_t m_pkgIndex = 1;
    int m_index = 0;
    bool m_isUpgrade = false;

    // 心跳
    volatile bool m_isAlive = false;

    const QStringList m_baudrates{
        "5K", "10K", "20K", "50K",
        "100K", "125K", "250K", "500K", "800K", "1000K"};
    const QStringList m_serailBaudrates{
        "600", "1200", "2400", "4800", "9600", "14400",
        "19200", "28800", "38400", "56000", "57600",
        "115200", "128000", "230400"
    };
    const QStringList idFilterTypeList{tr("标准帧单ID滤波"), tr("标准帧组ID滤波"), tr("扩展帧单ID滤波"), tr("扩展帧组ID滤波")};
    const QStringList databitslist{"5","6","7","8"};
    const QStringList paritybitslist{tr("无校验"), tr("奇校验"), tr("偶校验"), tr("强制为0"), tr("强制为1")};
    const QStringList stopbitslist{"1", "1.5", "2"};
    const QStringList serailmodellist{tr("RS232"), tr("RS485"), tr("RS422")};
    const QStringList transitionModelList{tr("透明转换"), tr("透明带标识转换"), tr("格式转换"), tr("MODBUS转换")};
    const QStringList transitionDirecteList{tr("双向转换"), tr("仅串口转到CAN"), tr("仅CAN转到串口")};
};

#endif // HANDLESERIALPROTOCOL_H
