#include <QThread>
#include "scandevices.h"
#include "ui_scandevices.h"

ScanDevices::ScanDevices(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanDevices)
{
    ui->setupUi(this);
    ui->tableWidget_devices->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget_devices->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_devices->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->initBroadcast();
}

ScanDevices::~ScanDevices()
{
    delete ui;
}
bool ScanDevices::scanDevices()
{
    this->on_pushButton_scan_clicked();
    return true;
}

void ScanDevices::onOpenResult()
{
    ui->pushButton_open->setEnabled(false);
    ui->pushButton_close->setEnabled(true);
}

void ScanDevices::onCloseResult()
{
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_close->setEnabled(false);
}

void ScanDevices::on_pushButton_scan_clicked()
{
    ScanReqUdp req;
    ui->tableWidget_devices->clearContents();
    ui->tableWidget_devices->setRowCount(0);
    m_pReceiver->writeDatagram(reinterpret_cast<char*>(&req), sizeof(ScanReqUdp), QHostAddress::Broadcast, m_destPort);
}

void ScanDevices::on_pushButton_open_clicked()
{
    emit openDevice(ui->lineEdit->text());
}

void ScanDevices::on_pushButton_close_clicked()
{
    emit closeDevice();
}

void ScanDevices::initBroadcast()
{
    qDebug() << QDateTime::currentDateTime().toMSecsSinceEpoch();
    m_pReceiver = new QUdpSocket(this);
    // 绑定，第一个参数为端口号，第二儿表示允许其它地址链接该广播
    m_pReceiver->bind(m_destPort,QUdpSocket::ShareAddress);
    qDebug() << QDateTime::currentDateTime().toMSecsSinceEpoch();

    // readyRead:每当有数据报来时发送这个信号
    connect(m_pReceiver,SIGNAL(readyRead()) ,this,SLOT(on_processPengingDatagram()));
}

void ScanDevices::on_processPengingDatagram()
{
    //数据报不为空
    while(m_pReceiver->hasPendingDatagrams()){
        //接收数据报
        QNetworkDatagram datagram = m_pReceiver->receiveDatagram();
        if(this->isAckPkg(datagram)){
            QString macAddr, ipAddr, hardware;
            auto data = datagram.data();
            ScanAckUdp * rst = reinterpret_cast<ScanAckUdp*>(data.data());
            macAddr = QString::asprintf("%.02X-%.02X-%.02X-%.02X-%.02X-%.02X",
                                        rst->mac[0], rst->mac[1], rst->mac[2], rst->mac[3], rst->mac[4], rst->mac[5]);
            ipAddr = QString::asprintf("%d.%d.%d.%d", rst->ip[0], rst->ip[1], rst->ip[2], rst->ip[3]);
            hardware = QString("%1").arg(rst->hardware / 100.00);
            // 添加到界面上
            QList<QTableWidgetItem *> macs = ui->tableWidget_devices->findItems(macAddr, Qt::MatchFlag::MatchContains);
            if(macs.isEmpty()){
                int nOldRowCount = ui->tableWidget_devices->rowCount();
                ui->tableWidget_devices->insertRow(nOldRowCount);
                // ip
                QTableWidgetItem *itemIp = new QTableWidgetItem(ipAddr);
                itemIp->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_devices->setItem(nOldRowCount, 0, itemIp);
                // mac
                QTableWidgetItem *itemMac = new QTableWidgetItem(macAddr);
                itemMac->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_devices->setItem(nOldRowCount, 1, itemMac);
                // hardware
                QTableWidgetItem *itemHardware = new QTableWidgetItem(hardware);
                ui->tableWidget_devices->setItem(nOldRowCount, 2, itemHardware);
                itemHardware->setTextAlignment(Qt::AlignCenter);
                emit deviceIp(ipAddr);
            }
        }
    }
}

bool ScanDevices::isAckPkg(QNetworkDatagram &datagram)
{
    auto data = datagram.data();
    auto tmp = data.mid(3, data.length() - 5);
    auto crc = usMBCRC16(tmp);
    ScanAckUdp * rst = reinterpret_cast<ScanAckUdp*>(data.data());
    if(rst->header[0] == 0x5A && rst->header[1] == 0xA5 && rst->cmd == 0xA1){
        uint rcvCrc = rst->crc16[1];
        rcvCrc = (rcvCrc << 8) | rst->crc16[0];
        return crc == rcvCrc;
    }
    return false;
}

void ScanDevices::on_tableWidget_devices_itemSelectionChanged()
{
    int row = ui->tableWidget_devices->currentRow();
    auto ip = ui->tableWidget_devices->item(row, 0)->text();
    ui->label_current_ip->setText(ip);
    ui->lineEdit->setText(ip);
}
