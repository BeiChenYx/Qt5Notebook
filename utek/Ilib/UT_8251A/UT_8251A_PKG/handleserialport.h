#ifndef HANDLESERIALPORT_H
#define HANDLESERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QMap>
#include <QVector>


class HandleSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit HandleSerialPort(QObject *parent = nullptr);
    virtual ~HandleSerialPort();
    bool isOpen(){return m_pSerial->isOpen();}

    // 获取可用的串口名字
    static QStringList getSerialPortNames();

    /* 设置串口参数，并且提供获取支持的串口参数的字符串列表
     * 提供获取key的字符串列表方便直接添加到QComboBox中
     * 使用的时候直接获取当前值后通过setXXX接口完成设置
     * baudRate如果添加了不存在的key，会默认插入，并更新key,
     * 其他接口不提供插入新的值
     * @return：设置成功返回true,反之false
     */
    bool setBaudRate(QString baudRate);
    static QStringList getBaudRateKeys();
    bool setParity(QString parity);
    static QStringList getParityKeys();
    bool setDataBits(QString dataBits);
    static QStringList getDataBitsKeys();
    bool setStopBits(QString stopBits);
    static QStringList getStopBitsKeys();
    bool setFlowControl(QString flowControl);
    static QStringList getFlowControlKeys();

    // 设置串口名字: 如: COM1
    void setPortName(QString portName);

    /* 设置是否输出Hex字符串
    * @separator: 输出HEX的间隔符, 如果为空则不输出HEX
    */
    void setOutHex(bool isHex, QChar separator=' '){
        this->m_outHex = isHex;
        this->m_setparator = separator;
    }

    void setOutDateTime(bool isDateTime){
        this->m_outDateTime = isDateTime;
    }

    // 以16进制的字符串发送
    qint64 writeHex(const QString &msg, QChar separator=' ');
    qint64 writeAscii(const QString &msg);
    qint64 writeByte(const QByteArray &bytes);
    qint64 writeByte(uchar *data, int size);

    // 打开关闭串口
    bool serialPortOpen(QString portName="");
    void serialPortClose();

    // DTR及RTS针脚设置
    bool setDTR(bool set){
        return m_pSerial->setDataTerminalReady(set);
    }
    bool setRTS(bool set){
        return m_pSerial->setRequestToSend(set);
    }

    void setNeedLen(int len){
        m_needLen = len;
        m_pSerial->read(m_pSerial->bytesAvailable());
    }

signals:
    void readyReadString(QString msg, qint64 size);
    void readyReadByte(QByteArray bytes);
    void serialPortStatus(uint status);
    void writeError(QString msg);

private slots:
    void onReadData();

private:
    QSerialPort *m_pSerial;
    // 至少需要读的字节个数，避免使用缓存来拆包
    volatile int m_needLen = 1;

    // 波特率, 校验位，数据位，停止位，流控制对应的字符串和值
    QMap<QString, qint32> m_baudRateMap;

    static const QStringList m_baudRateKeys;
    static const QMap<QString, QSerialPort::Parity> m_parityMap;
    static const QMap<QString, QSerialPort::DataBits> m_dataBitsMap;
    static const QMap<QString, QSerialPort::StopBits> m_stopBitsMap;
    static const QMap<QString, QSerialPort::FlowControl> m_flowControlMap;

    // 控制HEX字符串
    bool m_outHex = false;
    QChar m_setparator = ' ';

    // 控制时间信息
    bool m_outDateTime = false;

    QTimer *m_pPinoutTimer;
    const int PINOUT_TIMES = 300;
};

#endif // HANDLESERIALPORT_H
