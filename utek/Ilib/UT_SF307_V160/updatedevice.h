#ifndef UPDATEDEVICE_H
#define UPDATEDEVICE_H

#include <QObject>
#include <memory>
#include <QHostInfo>
#include <QHostAddress>
#include <QTcpSocket>
#include <QDebug>
#include "mbcrc.h"

#pragma pack (1)
struct UpdateAck{
    uchar header[2];          // 帧头
    uchar len[2];             // 数据长度 从功能码到校验码的长度
    uchar cmd;               // 功能码
    uchar childCmd;          // 子功能码
    uchar data;              // 应答数据区
    uchar crc[2];            // CRC校验码
};

struct StartReq{
    uchar data[8]{0x5A,0xA5, 0x00, 0x04, 0x02, 0x01,0xC1, 0x10};
};
struct EndReq{
    uchar data[8]{0x5A,0xA5, 0x00, 0x04, 0x02, 0x03,0x40, 0xD1};
};

#pragma pack ()


class UpdateDevice : public QObject
{
    Q_OBJECT
public:
    explicit UpdateDevice(QObject *parent = nullptr, QString ip="", QString fileName="");
    ~UpdateDevice(){ }

    void setInfo(QString ip, QString fileName){
        this->m_fileName = fileName;
        this->m_ip = ip;
    }

signals:
    void isConnected(bool status);
    void isDisconnected();
    void progressBarValue(int value);
    void progressBarRange(int min, int max);
    void progressBarText(QString msg);

public slots:
    void onUpdateDevice();

private:
    // 打开文件，读取所有数据
    bool getFileData();
    // 处理升级请求
    bool updateStart();
    // 发送文件
    bool sendFile();
    // 发送单包文件数据
    qint64 sendFilePage(const QByteArray &bytearray);
    // 处理加载完成
    bool updateEnd();

    // 连接服务器
    bool connectServer();
    // 断开服务器
    void disConnectServer();

private:
    std::shared_ptr<QHostAddress> m_pServerIp;
    std::shared_ptr<QTcpSocket> m_pClientSockt;
    QString m_ip;
    QString m_fileName;
    const ushort m_port = 502;
    QVector<char> m_fileData;
    // 文件拆包后的总包数
    int m_pageCount;
    // 协议的流水序号
    volatile uint m_pageNo = 0;
    // 每一包协议的数据总长
    const int m_pageLen = 1024;
    // 每一包中文件数据最大长度, m_pageLen - 12
    const int m_pageDataLen = 1012;
};

#endif // UPDATEDEVICE_H
