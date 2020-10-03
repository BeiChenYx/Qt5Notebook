#include <QDebug>
#include <QDateTime>
#include <handleserialport.h>
#include "connectingdevices.h"
#include "ui_connectingdevices.h"

ConnectingDevices::ConnectingDevices(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectingDevices),
    m_pIsAliverTimer(new QTimer)
{
    ui->setupUi(this);
    ui->tableWidget_dev_list->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget_dev_list->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_dev_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_dev_list->setColumnWidth(2, 280);

    ui->comboBox_baude_com->addItems(HandleSerialPort::getBaudRateKeys());
    ui->comboBox_baude_com->setCurrentText("115200");
    ui->comboBox_parity_com->addItems(HandleSerialPort::getParityKeys());
    ui->comboBox_parity_com->setCurrentText(tr("无校验"));
    ui->comboBox_databits_com->addItems(HandleSerialPort::getDataBitsKeys());
    ui->comboBox_databits_com->setCurrentText("8");
    ui->comboBox_stopbits_com->addItems(HandleSerialPort::getStopBitsKeys());
    ui->comboBox_stopbits_com->setCurrentText("1");

    connect(m_pIsAliverTimer.get(), &QTimer::timeout, this, [this](){
        if(m_connectStatus == HID_CONNECT){
            emit reqAliveHid();
        }else if(m_connectStatus == COM_CONNECT){
            m_pHandleSerial->isAlive();
        }
    });
}

ConnectingDevices::~ConnectingDevices()
{
    delete ui;
}

void ConnectingDevices::on_pushButton_scan_hid_clicked()
{
    ui->tableWidget_dev_list->clearContents();
    ui->tableWidget_dev_list->setRowCount(0);
    emit scanHid();
}

void ConnectingDevices::on_pushButton_hid_open_clicked()
{
    if(ui->pushButton_hid_open->isChecked()){
        QString vid = ui->label_vid->text();
        QString pid = ui->label_pid->text();
        QString serialNumber = ui->label_serial->text();
        emit openHid(vid, pid, serialNumber);
    }else{
        emit closeHid();
    }
}

void ConnectingDevices::onIsOpenHid()
{
    ui->pushButton_com_open->setEnabled(false);
    ui->pushButton_hid_open->setText(tr("关闭"));
    ui->pushButton_hid_open->setChecked(true);
    m_connectStatus = HID_CONNECT;
    m_pIsAliverTimer->start(IS_ALIVER_INTERVAL);
    emit hidStatus(true);
}

void ConnectingDevices::onIsCloseHid()
{
    ui->pushButton_com_open->setEnabled(true);
    ui->pushButton_hid_open->setText(tr("打开"));
    ui->pushButton_hid_open->setChecked(false);
    m_connectStatus = NO_CONNECT;
    m_pIsAliverTimer->stop();
    emit hidStatus(false);
}

void ConnectingDevices::onIsOpenSerial()
{
    ui->pushButton_hid_open->setEnabled(false);
    ui->pushButton_com_open->setText(tr("关闭"));
    ui->pushButton_com_open->setChecked(true);
    m_connectStatus = COM_CONNECT;
    m_pIsAliverTimer->start(IS_ALIVER_INTERVAL);
    emit serialPortStatus(true);
}

void ConnectingDevices::onIsCloseSerial()
{
    ui->pushButton_hid_open->setEnabled(true);
    ui->pushButton_com_open->setText(tr("打开"));
    ui->pushButton_com_open->setChecked(false);
    if(m_connectStatus != NO_CONNECT){
        m_connectStatus = NO_CONNECT;
        m_pHandleSerial->close();
        emit serialPortStatus(false);
    }
    m_pIsAliverTimer->stop();
}

void ConnectingDevices::on_tableWidget_dev_list_itemSelectionChanged()
{
    try {
        int row = ui->tableWidget_dev_list->currentRow();
        auto vid = ui->tableWidget_dev_list->item(row, 0)->text();
        auto pid = ui->tableWidget_dev_list->item(row, 1)->text();
        auto serial = ui->tableWidget_dev_list->item(row, 2)->text();
        ui->label_vid->setText(vid);
        ui->label_pid->setText(pid);
        ui->label_serial->setText(serial);
    } catch (...) {
        return;
    }
}

void ConnectingDevices::instertScanResult(QStringList &info)
{
    if(info.contains("UTEK")){
        int row = ui->tableWidget_dev_list->rowCount();
        ui->tableWidget_dev_list->insertRow(row);
        for (QString item: info){
            QTableWidgetItem *itemrow = new QTableWidgetItem(item);
            itemrow->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_dev_list->setItem(row, info.indexOf(item), itemrow);
        }
    }
}

void ConnectingDevices::on_pushButton_scan_com_clicked()
{
    ui->comboBox_com->clear();
    ui->comboBox_com->addItems(HandleSerialPort::getSerialPortNames());
}

void ConnectingDevices::on_pushButton_com_open_clicked()
{
    if(ui->pushButton_com_open->isChecked()){
        SerialTaskPar par;
        par.com = ui->comboBox_com->currentText();
        par.bauderateSerial = ui->comboBox_baude_com->currentText();
        par.dataBit = ui->comboBox_databits_com->currentText();
        par.stopBit = ui->comboBox_stopbits_com->currentText();
        par.parityBit = ui->comboBox_parity_com->currentText();
        if(m_pHandleSerial->open(par)){
            m_pHandleSerial->openCan();
        }
        // 让通信类来控制打开的结果，进而控制按钮的样式
        ui->pushButton_com_open->setChecked(false);
    } else {
        m_pHandleSerial->close();
        emit serialPortStatus(false);
    }
}

void ConnectingDevices::onHidStatus(bool status)
{
    ui->pushButton_com_open->setEnabled(!status);
    ui->pushButton_hid_open->setText(status ? tr("关闭") : tr("打开"));
}

void ConnectingDevices::onSerialStatus(bool status)
{
    ui->pushButton_hid_open->setEnabled(!status);
    ui->pushButton_com_open->setText(status ? tr("关闭") : tr("打开"));
}


