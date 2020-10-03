#ifndef SCANDEVICES_H
#define SCANDEVICES_H

#include <QWidget>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTimer>
#include <QDateTime>
#include "mbcrc.h"

namespace Ui {
class ScanDevices;
}

#pragma pack(1)
/* 扫描设备的协议: UDP/IP协议
 * 扫描方: 发送UDP广播包
 * 设备方: 接受到UDP广播，然后根据收到的广播地址，
 * 定向发送UDP回应包, 回应数据应该包含mac,ip,hardware,没有数据填零
 */
struct ScanReqUdp{
    // 请求头 0x5A 0xA5
    uchar header[2]{0x5A, 0xA5};
    // len 之后的数据长度, cmd 到crc16的所有字节长度
    uchar len{0x03};
    // 命令 0x01, 代表扫描设备
    uchar cmd{0x01};
    // modbuf crc16 低字节在前, 校验位len后面的字节，不包括len
    uchar crc16[2]{0x7E, 0x80};
};
struct ScanAckUdp{
    // 请求头 0x5A 0xA5
    uchar header[2]{0x5A, 0xA5};
    // 数据长度 ip, mac, hardware 长度
    uchar len{0x0E};
    // 命令 0xA1 回复 0x01 的命令字
    uchar cmd{0xA1};
    // 设备ip
    uchar ip[4]{0};
    // 设备mac地址
    uchar mac[6]{0};
    // 固件版本号
    uchar hardware{0};
    uchar crc16[2];
};

#pragma pack()



class ScanDevices : public QWidget
{
    Q_OBJECT

public:
    explicit ScanDevices(QWidget *parent = nullptr);
    ~ScanDevices();

    // 扫描本网段内所有的设备
    bool scanDevices();
    void initBroadcast();
    bool isAckPkg(QNetworkDatagram &datagram);

public slots:
    // 设备打开结果
    void onOpenResult();
    // 设备关闭结果
    void onCloseResult();
    void on_processPengingDatagram();
    void on_pushButton_scan_clicked();

signals:
    // 打开指定的设备
    void openDevice(QString ip);
    void closeDevice();
    // 设备信息列表
    void deviceIp(QString ip);


private slots:
    void on_pushButton_open_clicked();
    void on_pushButton_close_clicked();

    void on_tableWidget_devices_itemSelectionChanged();

private:
    Ui::ScanDevices *ui;
    QUdpSocket *m_pReceiver;
    const ushort m_destPort = 4000;
};

#endif // SCANDEVICES_H
