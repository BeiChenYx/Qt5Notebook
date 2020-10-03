#include "datatest.h"
#include "ui_datatest.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "FrameLessWidget/framelesswidget.h"


DataTest::DataTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTest)
{
    ui->setupUi(this);
    ui->tableWidget_rcv->horizontalHeader()->setVisible(true);
}

DataTest::~DataTest()
{
    delete ui;
}

bool DataTest::handleUiData(QString &id, QString &frameType, QString &frameFormat, QString &data)
{
    auto closeFunc = [this](){
        if(ui->checkBox_times->isChecked()){
            ui->checkBox_times->setChecked(false);
            emit txCloseTimes();
        }
    };
    id = handleId(ui->lineEdit_id->text().trimmed());
    frameType = ui->comboBox_type->currentText();
    frameFormat = ui->comboBox_format->currentText();
    data = ui->textEdit->toPlainText().trimmed();
    if(id.isEmpty()){
        CustomInfoBox::customInfoBox(tr("帧ID不合法"), tr("正确格式为: 01 AB 0C 0D"));
        closeFunc();
        return false;
    }
    QString max = ui->comboBox_format->currentIndex() ? "1F FF FF FF" : "00 00 07 FF";
    QString errMsg = ui->comboBox_format->currentIndex() ? tr("扩展帧ID最大0x1FFFFFFF"): tr("标准帧ID最大0x07FF");
    if(id > max){
        CustomInfoBox::customInfoBox(tr("帧ID不合法"), errMsg);
        closeFunc();
        return false;
    }
    return true;
}

void DataTest::on_pushButton_send_clicked()
{
    QString id; QString frameType; QString frameFormat; QString data;
    if(this->handleUiData(id, frameType, frameFormat, data)){
        emit txDataToCan(id, frameType, frameFormat, data);
    }
}

void DataTest::onRcvData(int len, QString id, QString frameType,
                         QString frameFormat, QString timestamp, QString data)
{
    if(len < 0){return;}
    ui->label_rx->setText(QString("%1").arg(ui->label_rx->text().toLongLong() + len));
    int row = ui->tableWidget_rcv->rowCount();
    ui->tableWidget_rcv->insertRow(row);
    // 序号
    QTableWidgetItem *itemId = new QTableWidgetItem(QString("%1").arg(row));
    ui->tableWidget_rcv->setItem(row, 0, itemId);
    // 时间
    QTableWidgetItem *itemDatetime = new QTableWidgetItem(timestamp);
    ui->tableWidget_rcv->setItem(row, 1, itemDatetime);
    // 发送/接收
    QTableWidgetItem *itemIsRcv = new QTableWidgetItem(tr("接收"));
    ui->tableWidget_rcv->setItem(row, 2, itemIsRcv);
    // 帧ID
    QTableWidgetItem *itemFrameId = new QTableWidgetItem(QString("%1").arg(id));
    ui->tableWidget_rcv->setItem(row, 3, itemFrameId);
    // 帧格式
    QTableWidgetItem *itemFrameFormat = new QTableWidgetItem(frameFormat);
    ui->tableWidget_rcv->setItem(row, 4, itemFrameFormat);
    // 帧类型
    QTableWidgetItem *itemFrameType = new QTableWidgetItem(frameType);
    ui->tableWidget_rcv->setItem(row, 5, itemFrameType);
    // 数据长度
    QTableWidgetItem *itemFrameLen = new QTableWidgetItem(QString("%1").arg(len));
    ui->tableWidget_rcv->setItem(row, 6, itemFrameLen);
    // 数据
    QTableWidgetItem *itemFrameData = new QTableWidgetItem(data);
    ui->tableWidget_rcv->setItem(row, 7, itemFrameData);
    // 滚动到最下面
    ui->tableWidget_rcv->scrollToBottom();
}

void DataTest::onWriteData(int &len, QString &id, QString &frameType,
                   QString &frameFormat, QString &data, QString &timestamp)
{
    if(len < 0){return;}
    ui->label_tx->setText(QString("%1").arg(ui->label_tx->text().toLongLong() + len));
    int row = ui->tableWidget_rcv->rowCount();
    ui->tableWidget_rcv->insertRow(row);
    // 序号
    QTableWidgetItem *itemId = new QTableWidgetItem(QString("%1").arg(row));
    ui->tableWidget_rcv->setItem(row, 0, itemId);
    // 时间
    QTableWidgetItem *itemDatetime = new QTableWidgetItem(timestamp);
    ui->tableWidget_rcv->setItem(row, 1, itemDatetime);
    // 发送/接收
    QTableWidgetItem *itemIsRcv = new QTableWidgetItem(tr("发送"));
    ui->tableWidget_rcv->setItem(row, 2, itemIsRcv);
    // 帧ID
    QTableWidgetItem *itemFrameId = new QTableWidgetItem(QString("%1").arg(id));
    ui->tableWidget_rcv->setItem(row, 3, itemFrameId);
    // 帧格式
    QTableWidgetItem *itemFrameFormat = new QTableWidgetItem(frameFormat);
    ui->tableWidget_rcv->setItem(row, 4, itemFrameFormat);
    // 帧类型
    QTableWidgetItem *itemFrameType = new QTableWidgetItem(frameType);
    ui->tableWidget_rcv->setItem(row, 5, itemFrameType);
    // 数据长度
    QTableWidgetItem *itemFrameLen = new QTableWidgetItem(QString("%1").arg(len));
    ui->tableWidget_rcv->setItem(row, 6, itemFrameLen);
    // 数据
    QTableWidgetItem *itemFrameData = new QTableWidgetItem(data);
    ui->tableWidget_rcv->setItem(row, 7, itemFrameData);
    // 滚动到最下面
    ui->tableWidget_rcv->scrollToBottom();
}

void DataTest::on_checkBox_times_clicked()
{
    if(ui->checkBox_times->isChecked()){
        on_pushButton_send_clicked();
        QString id; QString frameType; QString frameFormat; QString data;
        if(this->handleUiData(id, frameType, frameFormat, data)){
            auto times = ui->spinBox_times->value();
            if(times > 0){
                emit txTimes(times, id, frameType, frameFormat, data);
            }
        }
    }else{
        emit txCloseTimes();
    }
}

void DataTest::canDataFunc(CanRcvData *pData, void *pUser)
{
    const static QStringList frameType{tr("数据帧"),  tr("远程帧")};
    const static QStringList frameFormat{tr("标准帧"), tr("扩展帧")};
    QByteArray buf(reinterpret_cast<char*>(pData->data), static_cast<int>(pData->size));
    DataTest *pHandle = reinterpret_cast<DataTest*>(pUser);
    pHandle->onRcvData(static_cast<int>(pData->size), uintToQString(pData->id),
                     frameType[pData->frameType], frameFormat[pData->frameFormat],
                     QString::number(pData->timestamp), buf.toHex(' ').toUpper());
}

void DataTest::on_pushButton_clear_clicked()
{
    ui->tableWidget_rcv->clearContents();
    ui->tableWidget_rcv->setRowCount(0);
    ui->label_tx->setText("0");
    ui->label_rx->setText("0");
}

void DataTest::on_pushButton_output_clicked()
{
    try {
        QString path = QFileDialog::getSaveFileName(this,tr("导出数据"), ".", tr("*.csv"));
        QFile outInfo(path);
        if(outInfo.open(QFile::WriteOnly | QFile::Truncate)){
            QTextStream out(&outInfo);
            out << tr("序号") << "," << tr("时间") << ","
                << tr("发送/接收") << "," << tr("帧ID") << ","
                << tr("帧格式") << "," << tr("帧类型") << ","
                << tr("数据长度") << "," << tr("数据HEX") << "\n";
            int row = ui->tableWidget_rcv->rowCount();
            int col = ui->tableWidget_rcv->columnCount();
            for (int i=0; i<row; ++i) {
                QString tmp;
                for (int j=0; j<col; ++j) {
                    QString txt = ui->tableWidget_rcv->item(i, j)->text();
                    tmp += txt + ",";
                }
                tmp = tmp.mid(0, tmp.length()-1);
                out << tmp << "\n";
            }
            outInfo.close();
        }
        CustomInfoBox::customInfoBox(tr("数据导出"), tr("数据导出完成!"));
    } catch (...) {
        CustomErrorBox::customErrorBox(tr("数据导出"), tr("数据导出完成!"));
    }
}
