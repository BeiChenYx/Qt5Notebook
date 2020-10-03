#include "sigledebug.h"
#include "ui_sigledebug.h"
#include <QFileInfo>
#include <QSettings>


SigleDebug::SigleDebug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sigleDebug),
    m_pSerial(new HandleSerialPort(this)),
    m_timesSend(new QTimer(this))
{
    ui->setupUi(this);

    ui->comboBox_Com->addItems(m_pSerial->getSerialPortNames());
    ui->comboBox_Com->setCurrentIndex(0);
    // 波特率
    ui->comboBox_Baute->addItems(m_pSerial->getBaudRateKeys());
    ui->comboBox_Baute->setCurrentIndex(8);
    // 校验，
    ui->comboBox_Check->addItems(m_pSerial->getParityKeys());
    // 数据位
    ui->comboBox_Data->addItems(m_pSerial->getDataBitsKeys());
    ui->comboBox_Data->setCurrentIndex(3);
    // 停止位
    ui->comboBox_Stop->addItems(m_pSerial->getStopBitsKeys());
    ui->comboBox_Stop->setCurrentIndex(0);
    // 流控制
    ui->comboBox_Stream->addItems(m_pSerial->getFlowControlKeys());
    ui->lineEdit_RecveCount->setText(tr("0"));
    ui->lineEdit_SendCount->setText(tr("0"));

    ui->checkBox_16Read->setChecked(true);
    ui->checkBox_16SetSend->setChecked(true);
    this->statusEnabled(false);

    connect(m_timesSend, &QTimer::timeout, this, &SigleDebug::on_pushButton_Send_clicked);
    connect(m_pSerial, &HandleSerialPort::readyReadString, this, &SigleDebug::onReadData);
    connect(m_pSerial, &HandleSerialPort::serialPortStatus, this, [this](uint status){
        ui->checkBox_DTR->setChecked(status & QSerialPort::DataTerminalReadySignal);
        ui->checkBox_DSR->setChecked(status & QSerialPort::DataSetReadySignal);
        ui->checkBox_DCD->setChecked(status & QSerialPort::DataCarrierDetectSignal);
        ui->checkBox_RNG->setChecked(status & QSerialPort::RingIndicatorSignal);
        ui->checkBox_RTS->setChecked(status & QSerialPort::RequestToSendSignal);
        ui->checkBox_CTS->setChecked(status & QSerialPort::ClearToSendSignal);
    });
}

SigleDebug::~SigleDebug()
{
    delete ui;
}

bool SigleDebug::WriteInitFile(){
    /*
     * 初始化配置文件
     * 如果配置文件不存在则使用初始化值来初始化界面和文件
    */
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    settings.setValue("SigleDebug/Baute", ui->comboBox_Baute->currentText());
    settings.setValue("SigleDebug/Parity", ui->comboBox_Check->currentText());
    settings.setValue("SigleDebug/Data", ui->comboBox_Data->currentText());
    settings.setValue("SigleDebug/Stop", ui->comboBox_Stop->currentText());
    settings.setValue("SigleDebug/Stream", ui->comboBox_Stream->currentText());;
    settings.setValue("SigleDebug/SendTimes", ui->spinBox_times->value());
    settings.setValue("SigleDebug/LogPath", ui->lineEdit_FilePath->text());
    settings.setValue("SigleDebug/Read16", ui->checkBox_16Read->isChecked());
    settings.setValue("SigleDebug/Send16", ui->checkBox_16SetSend->isChecked());
    settings.setValue("SigleDebug/SendBuf", ui->textEdit_Send->toPlainText());

    return true;
}
bool SigleDebug::ReadInitFile(){
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    ui->comboBox_Baute->setCurrentText(settings.value("SigleDebug/Baute").toString());
    ui->comboBox_Check->setCurrentText(settings.value("SigleDebug/Parity").toString());
    ui->comboBox_Data->setCurrentText(settings.value("SigleDebug/Data").toString());
    ui->comboBox_Stop->setCurrentText(settings.value("SigleDebug/Stop").toString());
    ui->comboBox_Stream->setCurrentText(settings.value("SigleDebug/Stream").toString());
    ui->spinBox_times->setValue(settings.value("SigleDebug/SendTimes").toInt());
    ui->lineEdit_FilePath->setText(settings.value("SigleDebug/LogPath").toString());
    ui->checkBox_16Read->setChecked(settings.value("SigleDebug/Read16").toBool());
    ui->checkBox_16SetSend->setChecked(settings.value("SigleDebug/Send16").toBool());
    ui->textEdit_Send->setText(settings.value("SigleDebug/SendBuf").toString());
    return true;
}

void SigleDebug::on_pushButton_Connect_clicked()
{
    if(m_pSerial->serialPortOpen(ui->comboBox_Com->currentText())){
        m_pSerial->setBaudRate(ui->comboBox_Baute->currentText());
        m_pSerial->setParity(ui->comboBox_Check->currentText());
        m_pSerial->setStopBits(ui->comboBox_Stop->currentText());
        m_pSerial->setDataBits(ui->comboBox_Data->currentText());
        m_pSerial->setFlowControl(ui->comboBox_Stream->currentText());
        on_checkBox_16Read_clicked();
        m_pSerial->setOutDateTime(false);
        ui->pushButton_Close->setEnabled(true);
        ui->comboBox_Com->setEnabled(false);
        ui->checkBox_DTR->setChecked(true);
        ui->checkBox_RTS->setChecked(true);
        on_checkBox_DTR_clicked();
        on_checkBox_RTS_clicked();
        this->statusEnabled(true);
    }
}

void SigleDebug::on_pushButton_Close_clicked()
{
    m_pSerial->serialPortClose();
    ui->checkBox_DTR->setChecked(false);
    ui->checkBox_DCD->setChecked(false);
    ui->checkBox_DSR->setChecked(false);
    ui->checkBox_RNG->setChecked(false);
    ui->checkBox_RTS->setChecked(false);
    ui->checkBox_CTS->setChecked(false);
    this->statusEnabled(false);
}

void SigleDebug::on_pushButton_Send_clicked()
{
    QString msg = ui->textEdit_Send->toPlainText().trimmed();
    qint64 re = 0;
    if(ui->checkBox_16SetSend->isChecked()){
        re = this->m_pSerial->writeHex(msg);
    }else{
        re = this->m_pSerial->writeAscii(msg);
    }
    ui->lineEdit_SendCount->setText(QString::number(re + ui->lineEdit_SendCount->text().toLong()));
}

void SigleDebug::onReadData(QString msg, qint64 size)
{
    if(ui->checkBox_StopDisplay->isChecked()){ return; }
    if(ui->textEdit_Read->toPlainText().length() > 10240){ ui->textEdit_Read->clear(); }

    ui->textEdit_Read->moveCursor(QTextCursor::End);
    if(msg != ui->textEdit_Send->toPlainText().toUpper()){
        ui->textEdit_Read->insertHtml("<span style=\"color:#ff0000;\">"+msg+"</span>" + " ");
    }else{
        ui->textEdit_Read->insertPlainText(msg + " ");
    }
    ui->textEdit_Read->moveCursor(QTextCursor::End);
    auto len = size + ui->lineEdit_RecveCount->text().toLong();
    ui->lineEdit_RecveCount->setText(QString::number(len));
    writeLogFile(msg + " ");
}
void SigleDebug::writeLogFile(QString msg)
{
    if(!ui->checkBox_SaveLog->isChecked()
            || ui->lineEdit_FilePath->text().isEmpty()
            || msg.isEmpty()){
        return;
    }

    QFile file(ui->lineEdit_FilePath->text());
    if(!file.open(QIODevice::Append/* | QIODevice::Text*/)){
        QMessageBox::about(this,tr("记录导出"),tr("导出失败"));
        return;
    }
    QTextStream txtOutput(&file);
    txtOutput <<  msg;
    file.close();
}

void SigleDebug::on_pushButton_ClearRead_clicked()
{
    ui->textEdit_Read->clear();
}

void SigleDebug::on_checkBox_times_clicked()
{
    if(ui->checkBox_times->isChecked()){
        m_timesSend->start(ui->spinBox_times->value());
    }else {
        m_timesSend->stop();
    }
}

void SigleDebug::on_pushButton_Scan_clicked()
{
    ui->lineEdit_FilePath->setText(
        QFileDialog::getSaveFileName(this,tr("导出日志文件"), ".", tr("*.txt"))
    );
}

void SigleDebug::on_textEdit_Send_textChanged()
{
    if(ui->checkBox_16SetSend->isChecked()){
        QString sendBuf = ui->textEdit_Send->toPlainText();
        if(sendBuf.isEmpty()){
            return;
        }
        QRegExp rx(QString("[0-9A-Fa-f ]+"));
        bool match = rx.exactMatch(sendBuf);
        if(!match){
            sendBuf.remove(QRegularExpression("[^0-9A-Fa-f ]+"));
            ui->textEdit_Send->setText(sendBuf);
            ui->textEdit_Send->moveCursor(QTextCursor::End);
        }
    }
}

void SigleDebug::on_pushButton_ClearTimes_clicked()
{
    ui->lineEdit_RecveCount->setText(tr("0"));
    ui->lineEdit_SendCount->setText(tr("0"));
}

void SigleDebug::on_checkBox_16SetSend_clicked()
{
    if(ui->checkBox_16SetSend->isChecked()){
        QString buf = ui->textEdit_Send->toPlainText().trimmed();
        if(!buf.isEmpty()){
            QByteArray byteBuf(buf.toUtf8());
            ui->textEdit_Send->setPlainText(byteBuf.toHex(' ').toLower());
        }
    }else{
        QString buf = ui->textEdit_Send->toPlainText().trimmed().replace(" ", "");
        ui->textEdit_Send->setPlainText(QByteArray::fromHex(buf.toLocal8Bit()));
    }
}

void SigleDebug::on_checkBox_16Read_clicked()
{
    m_pSerial->setOutHex(ui->checkBox_16Read->isChecked());
}

void SigleDebug::on_checkBox_DTR_clicked()
{
    m_pSerial->setDTR(ui->checkBox_DTR->isChecked());
}

void SigleDebug::on_checkBox_RTS_clicked()
{
    m_pSerial->setRTS(ui->checkBox_RTS->isChecked());
}

void SigleDebug::statusEnabled(bool status)
{
    ui->comboBox_Com->setEnabled(!status);
    ui->pushButton_Connect->setEnabled(!status);
    ui->pushButton_Close->setEnabled(status);
    ui->pushButton_Send->setEnabled(status);
    ui->checkBox_times->setEnabled(status);
}
