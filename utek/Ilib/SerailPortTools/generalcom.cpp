#include "generalcom.h"
#include "ui_generalcom.h"
#include <QFileInfo>
#include <QSettings>
#include <QDateTime>


GeneralCom::GeneralCom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralCom),
    m_pSerial(new HandleSerialPort(this)),
    m_pSigle(new GeneralSigle(this)),
    m_pDouble(new GeneralDouble(this))
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
    ui->lineEdit_ReadCount->setText(tr("0"));
    ui->lineEdit_SendCount->setText(tr("0"));
    this->statusEnabled(false);

    connect(m_pSerial, &HandleSerialPort::readyReadString, this, &GeneralCom::onReadyString);
    connect(m_pSerial, &HandleSerialPort::serialPortStatus, this, [this](uint status){
        ui->checkBox_DTR->setChecked(status & QSerialPort::DataTerminalReadySignal);
        ui->checkBox_DSR->setChecked(status & QSerialPort::DataSetReadySignal);
        ui->checkBox_DCD->setChecked(status & QSerialPort::DataCarrierDetectSignal);
        ui->checkBox_RNG->setChecked(status & QSerialPort::RingIndicatorSignal);
        ui->checkBox_RTS->setChecked(status & QSerialPort::RequestToSendSignal);
        ui->checkBox_CTS->setChecked(status & QSerialPort::ClearToSendSignal);
    });
    auto sendSerialData = [this](QString msg, bool isHex){
        qint64 re = 0;
        if(isHex){
            re = this->m_pSerial->writeHex(msg);
        }else{
            re = this->m_pSerial->writeAscii(msg);
        }
        ui->lineEdit_SendCount->setText(QString::number(re + ui->lineEdit_SendCount->text().toLong()));
    };
    connect(m_pSigle, &GeneralSigle::sendData, this, sendSerialData);
    connect(m_pDouble, &GeneralDouble::sendData, this, sendSerialData);

    ui->tabWidget->addTab(m_pSigle, tr("单条发送"));
    ui->tabWidget->addTab(m_pDouble, tr("多条发送"));
}

bool GeneralCom::WriteInitFile(){
    /*
     * 初始化配置文件
     * 如果配置文件不存在则使用初始化值来初始化界面和文件
    */
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    settings.setValue("GeneralCom/Baute", ui->comboBox_Baute->currentText());
    settings.setValue("GeneralCom/Parity", ui->comboBox_Check->currentText());
    settings.setValue("GeneralCom/Data", ui->comboBox_Data->currentText());
    settings.setValue("GeneralCom/Stop", ui->comboBox_Stop->currentText());
    settings.setValue("GeneralCom/Stream", ui->comboBox_Stream->currentText());
    settings.setValue("GeneralCom/Set16", ui->checkBox_16Set->isChecked());
    settings.setValue("GeneralCom/LogPath", ui->lineEdit_FilePath->text());
    settings.setValue("GeneralCom/SaveLog", ui->checkBox_SaveLog->isChecked());
    settings.setValue("GeneralCom/DateTimeEnter", ui->checkBox_dataStopFlame->isChecked());
    m_pSigle->writeInitFile();
    m_pDouble->writeInitFile();
    return true;
}
bool GeneralCom::ReadInitFile(){
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    ui->comboBox_Baute->setCurrentText(settings.value("GeneralCom/Baute").toString());
    ui->comboBox_Check->setCurrentText(settings.value("GeneralCom/Parity").toString());
    ui->comboBox_Data->setCurrentText(settings.value("GeneralCom/Data").toString());
    ui->comboBox_Stop->setCurrentText(settings.value("GeneralCom/Stop").toString());
    ui->comboBox_Stream->setCurrentText(settings.value("GeneralCom/Stream").toString());
    ui->checkBox_16Set->setChecked(settings.value("GeneralCom/Set16").toBool());
    ui->lineEdit_FilePath->setText(settings.value("GeneralCom/LogPath").toString());
    ui->checkBox_SaveLog->setChecked(settings.value("GeneralCom/SaveLog").toBool());
    ui->checkBox_dataStopFlame->setChecked(settings.value("GeneralCom/DateTimeEnter").toBool());
    m_pSigle->readInitFile();
    m_pDouble->readInitFile();
    return true;
}

GeneralCom::~GeneralCom()
{
    delete ui;
}

void GeneralCom::onReadyString(QString msg, qint64 size)
{
    if(ui->checkBox_StopDisplay->isChecked()){ return; }
    if(ui->textEdit->toPlainText().length() > 10240){ ui->textEdit->clear(); }
    ui->textEdit->moveCursor(QTextCursor::End);
    if(ui->checkBox_16Set->isChecked()){
        ui->textEdit->insertPlainText(msg + " ");
    }else {
        ui->textEdit->insertPlainText(msg);
    }
    ui->textEdit->moveCursor(QTextCursor::End);
    auto len = size + ui->lineEdit_ReadCount->text().toLong();
    ui->lineEdit_ReadCount->setText(QString::number(len));
    writeLogFile(msg + " ");
}

void GeneralCom::on_pushButton_Open_clicked()
{
    if(m_pSerial->serialPortOpen(ui->comboBox_Com->currentText())){
        m_pSerial->setBaudRate(ui->comboBox_Baute->currentText());
        m_pSerial->setParity(ui->comboBox_Check->currentText());
        m_pSerial->setStopBits(ui->comboBox_Stop->currentText());
        m_pSerial->setDataBits(ui->comboBox_Data->currentText());
        m_pSerial->setFlowControl(ui->comboBox_Stream->currentText());
        on_checkBox_16Set_clicked();
        on_checkBox_dataStopFlame_clicked();
//        ui->pushButton_Open->setEnabled(false);
//        ui->pushButton_Close->setEnabled(true);
//        ui->comboBox_Com->setEnabled(false);
        ui->checkBox_DTR->setChecked(true);
        ui->checkBox_RTS->setChecked(true);
        on_checkBox_DTR_clicked();
        on_checkBox_RTS_clicked();
        this->statusEnabled(true);
    }
}

void GeneralCom::on_pushButton_Close_clicked()
{
    m_pSerial->serialPortClose();
//    ui->pushButton_Close->setEnabled(false);
//    ui->pushButton_Open->setEnabled(true);
//    ui->comboBox_Com->setEnabled(true);
    ui->checkBox_DTR->setChecked(false);
    ui->checkBox_DCD->setChecked(false);
    ui->checkBox_DSR->setChecked(false);
    ui->checkBox_RNG->setChecked(false);
    ui->checkBox_RTS->setChecked(false);
    ui->checkBox_CTS->setChecked(false);
    this->statusEnabled(false);
}

void GeneralCom::on_pushButton_5_clicked()
{
    ui->textEdit->clear();
}

void GeneralCom::writeLogFile(QString msg)
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

// scan
void GeneralCom::on_pushButton_clicked()
{
    ui->lineEdit_FilePath->setText(
        QFileDialog::getSaveFileName(this,tr("导出日志文件"), ".", tr("*.txt"))
    );
}

void GeneralCom::on_pushButton_ClearCount_clicked()
{
    ui->lineEdit_ReadCount->setText(tr("0"));
    ui->lineEdit_SendCount->setText(tr("0"));
}

void GeneralCom::on_checkBox_16Set_clicked()
{
    m_pSerial->setOutHex(ui->checkBox_16Set->isChecked());
}

void GeneralCom::on_checkBox_dataStopFlame_clicked()
{
    m_pSerial->setOutDateTime(ui->checkBox_dataStopFlame->isChecked());
}

void GeneralCom::on_checkBox_DTR_clicked()
{
    m_pSerial->setDTR(ui->checkBox_DTR->isChecked());
}

void GeneralCom::on_checkBox_RTS_clicked()
{
    m_pSerial->setRTS(ui->checkBox_RTS->isChecked());
}

void GeneralCom::statusEnabled(bool status)
{
    ui->comboBox_Com->setEnabled(!status);
    ui->pushButton_Open->setEnabled(!status);
    ui->pushButton_Close->setEnabled(status);
    this->m_pSigle->statusEnabled(status);
    this->m_pDouble->statusEnabled(status);
}
