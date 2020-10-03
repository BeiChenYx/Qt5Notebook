#ifndef CONNECTINGDEVICES_H
#define CONNECTINGDEVICES_H

#include <QWidget>
#include <QTimer>
#include <memory>
#include "handlehid.h"
#include "handleserialprotocol.h"


namespace Ui {
class ConnectingDevices;
}

enum ConnectStatus{
    NO_CONNECT = 0,
    HID_CONNECT = 1,
    COM_CONNECT = 2,
};


class ConnectingDevices : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectingDevices(QWidget *parent = nullptr);
    ~ConnectingDevices();
    void instertScanResult(QStringList &info);
    void setHandle(HandleSerialProtocol *pHanle){
        m_pHandleSerial = pHanle;
    }

public slots:
    void onIsOpenHid();
    void onIsCloseHid();

    // HID连接状态
    void onHidStatus(bool status);

    void onIsOpenSerial();
    void onIsCloseSerial();
    // 串口状态
    void onSerialStatus(bool status);

signals:
    void openHid(QString vid, QString pid, QString serialNumber);
    void closeHid();
    void scanHid();
    void reqAliveHid();

    void serialPortStatus(bool status);
    void hidStatus(bool status);

private slots:
    void on_pushButton_scan_hid_clicked();
    void on_pushButton_hid_open_clicked();
    void on_tableWidget_dev_list_itemSelectionChanged();

    void on_pushButton_scan_com_clicked();
    void on_pushButton_com_open_clicked();

private:
    Ui::ConnectingDevices *ui;
    HandleSerialProtocol *m_pHandleSerial;

    // 设备连接的方式
    ConnectStatus m_connectStatus = NO_CONNECT;

    // 定时检测设备活性
    std::shared_ptr<QTimer> m_pIsAliverTimer;
    const int IS_ALIVER_INTERVAL = 1000;
};

#endif // CONNECTINGDEVICES_H
