#ifndef HANDLEHID_H
#define HANDLEHID_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QByteArray>
#include <memory>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include
#include <QThread>
// 升级功能没有在UT_8251A_LIB库中
#include "../UT_8251A_LIB/ut_8251a_lib.h"

/*
 * 处理ID字符串，输出格式: 01 AB 0C 0D
 * @return: 为空字符串表示不合法，否则返回 01 AB 0C 0D 格式
 */
QString handleId(QString id);

/*
 * 将uint的id转成字符串的形式，格式: 01 AB 0C 0D
 * @return 格式化的字符串
 */
QString uintToQString(const uint id);

/*
 * 将01 AB 0C 0D转成uint整数
 * @return: 转换失败返回 0, 否则返回实际值
 */
uint stringToUint(const QString id);

// UT_8251A_LIB 的隐藏接口的声明
/*
 * 隐藏接口, 发送升级请求
 * @fileSize: 升级包 BIN 文件大小， 4 个字节高字节在前，低字节在后
 * @return:
 *      1: 请求成功
 *      0: 发生未知错误
 *     -1: 升级请求发送失败
 *     -2: 升级请求无响应
 *     -3: 升级请求反馈校验失败
 */
int upgradeReq(size_t fileSize);

/*
 * 发送升级数据
 * @data: 文件数据，最多16个字节
 * @size: data的大小
 * @return:
 *      1: 发送成功
 *      0: 发生未知错误
 *     -4: 升级数据发送失败
 *     -5: 升级数据反馈超时
 *     -6: 升级数据反馈校验错误
 */
int upgradeDataReq(unsigned int index, uchar *data, size_t size);

/*
 * 接收升级状态, 在发送完文件后立即调用
 * @return:
 *      1: 升级成功
 *      0: 发送未知错误
 *     -7: 升级文件状态校验错误
 *     -8: 升级文件状态确认结果发送失败
 */
int upgradeStatus();

struct TransiteParameterUi{
    TransiteParameterUi(){}
    TransiteParameterUi(
            QString model,
            QString directe,
            QString enableSerial,
            QString enableId,
            QString offset,
            QString frameLen,
            QString serialTimes,
            QString transiteTimes){
        this->model = model;
        this->directe = directe;
        this->enableSerial = enableSerial;
        this->enableId = enableId;
        this->offset = offset;
        this->frameLen = frameLen;
        this->serialTimes = serialTimes;
        this->transiteTimes = transiteTimes;
    }
    QString model;
    QString directe;
    QString enableSerial;
    QString enableId;
    QString offset;
    QString frameLen;
    QString serialTimes;
    QString transiteTimes;
};
Q_DECLARE_METATYPE(TransiteParameterUi)

struct SerialPortParameterUi{
    SerialPortParameterUi(){}
    SerialPortParameterUi(QString baudrate, QString databit,
                          QString paritybit, QString stopbit,
                          QString flow, QString model){
        this->baudrate = baudrate;
        this->databit = databit;
        this->paritybit = paritybit;
        this->stopbit = stopbit;
        this->flow = flow;
        this->model = model;
    }
    QString baudrate;
    QString databit;
    QString paritybit;
    QString stopbit;
    QString flow;
    QString model;
};
Q_DECLARE_METATYPE(SerialPortParameterUi)
Q_DECLARE_METATYPE(QList<QStringList>)


enum HidTask{
    IS_ALIVE_HID = 0,          // HID 是否在线
    OPEN_HID,                  // 打开HID
    CLOSE_HID,                 // 关闭HID
    SET_MODE_CAN,              // 设置CAN模式
    RESET_CAN,                 // 重置CAN
    RESET_ERROR_CAN,           // 重置CAN设备错误状态
    SET_PAR_CAN,               // 设置CAN设备参数
    WRITE_CAN,                 // 向CAN设备写数据
    CAN_PAR,                   // 读取CAN设备参数
    UPDATE_DEVICE,             // 升级设备
    FILTER_ID,                 // 设置过滤规则
    CLEAR_FILTER_ID,           // 清空过滤规则
    SET_TRANSITION_PAR_SERIAL, // 设置串口端转换参数
    SET_PAR_SERIAL,            // 设置串口参数
    MCU_PAR,                   // 读取mcu的参数
    SET_CAN_SERIAL,            // 设置串口端CAN参数
    SCAN_HID,                  // 扫描HID
};
struct HidFilterConfig{
    QString idFilterType;      // CAN帧ID滤波类型
    QString startID;           // 起始ID
    QString endID;             // 结束ID
};

struct HidTaskPar{
    QString m_canModel;
    bool m_rcvCtrl;
    QString m_id;
    QString m_frameType;
    QString m_frameFormat;
    QString m_data;
    QString m_updateFilePath;
    QString m_hidPath;
    QString m_vid;
    QString m_pid;
    QString m_serialNumber;
    QString m_baudrate;
    QString m_mask;
    QString m_filter;
    QList<QStringList> filterData;     // 过滤规则的数据

    // 兼容串口的读取和设置
    QString transitionModel;            // 转换模式
    QString transitionDirecte;          // 转换方向
    QString enableSerial;                  // 0x00 不允许CAN帧信息转发到串行帧中, 0x01 允许
    QString enableID;                      // 0x00 不允许CAN帧ID转发到串行帧中, 0x01 允许
    QString serialOffset;                   // CAN标识符在串行帧中的起始偏移
    QString serialFramLen;                  // CAN标识符在串行帧中的长度
    QString serialTimes;                    // 串行帧之间的时间间隔，单位 毫秒
    QString transitionTimes;                // CAN帧转发的时间间隔, 单位 毫秒

    QString bauterateCan;               // CAN通信波特率
    QString canID;                      // CAN帧ID， 高字节在前
    QString frameFormat;                // 帧格式: 0x00 标准帧, 0x01 扩展帧

    QString com;                        // 串口号
    QString bauderateSerial;            // 串口波特率

    QString dataBit;                    // 数据位
    QString parityBit;                  // 校验位
    QString stopBit;                    // 停止位
    QString flowControl;                // 流控制
    QString serailModel;                // 串口模式: 0x00 RS232, 0x01 RS485, 0x02 RS422
};
Q_DECLARE_METATYPE(HidTaskPar)

/*
 * 操作HID的类
 */
class HandleHid : public QThread
{
    Q_OBJECT
public:
    explicit HandleHid();
    ~HandleHid();
     void pushToTask(HidTask type);
     void pushToTask(HidTask type, HidTaskPar par);
     void exitWorkThread();

     QStringList canBaudrates(){return m_canBaudrates;}
     QStringList serialPortBaudrates(){return m_serailBaudrates;}

signals:
     void rcvData(int len, QString id, QString frameType, QString frameFormat, QString timestamp, QString data);
     void isInitError(QString error);
     void isOpenHid(bool status, QString version);
     void isCloseHid();
     void isAliveStatus(bool status);
     void isCanSetMode(bool status);
     void isCanReset(bool status);
     void isCanResetError(bool status);
     void isCanSetPar(bool status);
     void numCanWrite(int num, QString id, QString frameType,
                      QString frameFormat, QString data, QString datetime);
     void numCanRead(int num, QString id, QString frameType,
                     QString frameFormat, QString data, QString datetime);
     void isFilterId(bool status);
     void isClearFilterId(bool status);
     void updateStatus(QString msg);
     void updateResult(bool status);
     void processRange(int min, int max);
     void processBarV(int v);
     void isCanPar(QString bauderate, QList<QStringList> filter);
     void isCanParStatus(bool status);

     void isTransitionParCan(bool status);
     void isSerialPar(bool status);
     void isParCan(bool status);
     void isRMcuPar(TransiteParameterUi canPar, SerialPortParameterUi serialPar);
     void isRSerialPar(bool status, QString baudrate, QString id,
                       QString frameFormat, QList<QStringList> filter);

     void isScanHidDev(QList<QStringList> devs);

protected:
     void run();
private:
     bool openHid(QString vid, QString pid, QString serialNumber);
     void closeHid();
     bool isAlive();
     bool openCan();
     bool closeCan();
     bool initCan(QString &version);
     bool enableRcvCan(bool enable);
     bool setCanModel(QString model);
     bool resetCan();
     bool resetCanError();
     bool setCanBaudrate(QString baudrate);
     int writeToCan(QString id, QString frameType, QString frameFormat, QString data);
     void readFromCan();
     bool setCanFilterId(QList<QStringList> &filter);
     bool clearCanFilterId();
     bool readCanPar(QString &baudrate, QList<QStringList> &filter);
     bool setSerialPortTransitionPar(const TransiteParameterUi &par);
     bool setSerialPortPar(const SerialPortParameterUi &par);
     bool setSerialPortCanPar(QString baudrate, QString id, QString frameFormat);
     bool readSerialPortPar(TransiteParameterUi &parTransite, SerialPortParameterUi &parSerial);
     bool readSerialPortCanPar(QString &baudrate, QString &id, QString &frameFormat, QList<QStringList> &filter);

     bool upgradeDevReq(const size_t &fileSize);
     bool upgradeDevData(const unsigned int index, const QByteArray &data);
     bool upgradeDevStatus();

     void upgradeDev(QString &filePath);

     void scanHidDev();

private:
     volatile bool m_isExit = false;
     QQueue<QPair<HidTask, HidTaskPar> >  m_queue;
     QMutex m_mutex;

     const QStringList m_canModel{tr("正常模式"),  tr("自测模式")};
     const QStringList m_canBaudrates{"5K", "10K", "20K", "50K",
                                      "100K", "125K", "250K",
                                      "500K", "800K", "1000K"};
     const QStringList m_serailBaudrates{
             "600", "1200", "2400", "4800", "9600", "14400",
             "19200", "28800", "38400", "56000", "57600",
             "115200", "128000", "230400"};
     const QStringList m_frameType{tr("数据帧"),  tr("远程帧")};
     const QStringList m_frameFormat{tr("标准帧"), tr("扩展帧")};
     const QStringList m_idFilterTypeList{tr("标准帧单ID滤波"), tr("标准帧组ID滤波"), tr("扩展帧单ID滤波"), tr("扩展帧组ID滤波")};
     const QStringList m_databits{"5","6","7","8"};
     const QStringList m_paritybits{tr("无校验"), tr("奇校验"), tr("偶校验"), tr("强制为0"), tr("强制为1")};
     const QStringList m_stopbits{"1", "1.5", "2"};
     const QStringList m_serailmodel{tr("RS232"), tr("RS485"), tr("RS422")};
     const QStringList m_transitionModel{tr("透明转换"), tr("透明带标识转换"), tr("格式转换"), tr("MODBUS转换")};
     const QStringList m_transitionDirecte{tr("双向转换"), tr("仅串口转到CAN"), tr("仅CAN转到串口")};

     const QMap<int, QString> m_upgradeStatus{
         {1, tr("执行成功")},
         {0, tr("发生未知错误")},
         {-1, tr("升级请求发送失败")},
         {-2, tr("升级请求无响应")},
         {-3, tr("升级请求反馈校验失败")},
         {-4, tr("升级数据发送失败")},
         {-5, tr("升级数据反馈超时")},
         {-6, tr("升级数据反馈校验错误")},
         {-7, tr("升级文件状态校验错误")},
         {-8, tr("升级文件状态确认结果发送失败")},
     };
};

#endif // HANDLEHID_H
