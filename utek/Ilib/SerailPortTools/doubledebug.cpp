#include "doubledebug.h"
#include "ui_doubledebug.h"
#include <QFileInfo>
#include <QSettings>

DoubleDebug::DoubleDebug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleDebug),
    m_pSerial1(new HandleSerialPort(this)),
    m_pSerial2(new HandleSerialPort(this)),
    m_timesSend(new QTimer(this))
{
    ui->setupUi(this);
    auto coms = m_pSerial1->getSerialPortNames();
    ui->comboBox_Com1->addItems(coms);
    ui->comboBox_Com2->addItems(coms);
    ui->comboBox_Com1->setCurrentIndex(0);
    ui->comboBox_Com2->setCurrentIndex(0);

    auto baudRates = m_pSerial1->getBaudRateKeys();
    ui->comboBox_Baute1->addItems(baudRates);
    ui->comboBox_Baute2->addItems(baudRates);
    ui->comboBox_Baute1->setCurrentIndex(8);
    ui->comboBox_Baute2->setCurrentIndex(8);

    auto paritys = m_pSerial1->getParityKeys();
    ui->comboBox_Check1->addItems(paritys);
    ui->comboBox_Check2->addItems(paritys);

    auto dataBits = m_pSerial1->getDataBitsKeys();
    ui->comboBox_Data1->addItems(dataBits);
    ui->comboBox_Data2->addItems(dataBits);

    auto stopbits = m_pSerial1->getStopBitsKeys();
    ui->comboBox_Stop1->addItems(stopbits);
    ui->comboBox_Stop2->addItems(stopbits);

    auto flowControls = m_pSerial1->getFlowControlKeys();
    ui->comboBox_Stream1->addItems(flowControls);
    ui->comboBox_Stream2->addItems(flowControls);
    this->statusEnabled1(false);
    this->statusEnabled2(false);

    connect(m_timesSend, &QTimer::timeout, this, [this](){
        on_pushButton_Send1_clicked();
        on_pushButton_Send2_clicked();
    });
    connect(m_pSerial1, &HandleSerialPort::readyReadString, this, &DoubleDebug::onReadData1);
    connect(m_pSerial1, &HandleSerialPort::serialPortStatus, this, [this](uint status){
        ui->checkBox_DTR->setChecked(status & QSerialPort::DataTerminalReadySignal);
        ui->checkBox_DSR->setChecked(status & QSerialPort::DataSetReadySignal);
        ui->checkBox_DCD->setChecked(status & QSerialPort::DataCarrierDetectSignal);
        ui->checkBox_RNG->setChecked(status & QSerialPort::RingIndicatorSignal);
        ui->checkBox_RTS->setChecked(status & QSerialPort::RequestToSendSignal);
        ui->checkBox_CTS->setChecked(status & QSerialPort::ClearToSendSignal);
    });
    connect(m_pSerial2, &HandleSerialPort::readyReadString, this, &DoubleDebug::onReadData2);
    connect(m_pSerial2, &HandleSerialPort::serialPortStatus, this, [this](uint status){
        ui->checkBox_DTR2->setChecked(status & QSerialPort::DataTerminalReadySignal);
        ui->checkBox_DSR2->setChecked(status & QSerialPort::DataSetReadySignal);
        ui->checkBox_DCD2->setChecked(status & QSerialPort::DataCarrierDetectSignal);
        ui->checkBox_RNG2->setChecked(status & QSerialPort::RingIndicatorSignal);
        ui->checkBox_RTS2->setChecked(status & QSerialPort::RequestToSendSignal);
        ui->checkBox_CTS2->setChecked(status & QSerialPort::ClearToSendSignal);
    });

}

DoubleDebug::~DoubleDebug()
{
    delete ui;
}

bool DoubleDebug::WriteInitFile(){
    /*
     * 初始化配置文件
     * 如果配置文件不存在则使用初始化值来初始化界面和文件
    */
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    settings.setValue("DoubleDebug/Baute", ui->comboBox_Baute1->currentText());
    settings.setValue("DoubleDebug/Parity", ui->comboBox_Check1->currentText());
    settings.setValue("DoubleDebug/Data", ui->comboBox_Data1->currentText());
    settings.setValue("DoubleDebug/Stop", ui->comboBox_Stop1->currentText());
    settings.setValue("DoubleDebug/Stream", ui->comboBox_Stream1->currentText());;
    settings.setValue("DoubleDebug/Times", ui->spinBox_times->value());

    settings.setValue("DoubleDebug/Baute2", ui->comboBox_Baute2->currentText());
    settings.setValue("DoubleDebug/Parity2", ui->comboBox_Check2->currentText());
    settings.setValue("DoubleDebug/Data2", ui->comboBox_Data2->currentText());
    settings.setValue("DoubleDebug/Stop2", ui->comboBox_Stop2->currentText());
    settings.setValue("DoubleDebug/Stream2", ui->comboBox_Stream2->currentText());

    settings.setValue("DoubleDebug/Input16_1", ui->checkBox_16In1->isChecked());
    settings.setValue("DoubleDebug/Input16_2", ui->checkBox_16In2->isChecked());
    settings.setValue("DoubleDebug/D16_1", ui->checkBox_16D1->isChecked());
    settings.setValue("DoubleDebug/D16_2", ui->checkBox_16D2->isChecked());
    settings.setValue("DoubleDebug/InputBuf1", ui->textEdit_Input1->toPlainText());
    settings.setValue("DoubleDebug/InputBuf2", ui->textEdit_Input2->toPlainText());

    return true;
}
bool DoubleDebug::ReadInitFile(){
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    ui->comboBox_Baute1->setCurrentText(settings.value("DoubleDebug/Baute").toString());
    ui->comboBox_Check1->setCurrentText(settings.value("DoubleDebug/Parity").toString());
    ui->comboBox_Data1->setCurrentText(settings.value("DoubleDebug/Data").toString());
    ui->comboBox_Stop1->setCurrentText(settings.value("DoubleDebug/Stop").toString());
    ui->comboBox_Stream1->setCurrentText(settings.value("DoubleDebug/Stream").toString());
    ui->spinBox_times->setValue(settings.value("DoubleDebug/Times").toInt());

    ui->comboBox_Baute2->setCurrentText(settings.value("DoubleDebug/Baute2").toString());
    ui->comboBox_Check2->setCurrentText(settings.value("DoubleDebug/Parity2").toString());
    ui->comboBox_Data2->setCurrentText(settings.value("DoubleDebug/Data2").toString());
    ui->comboBox_Stop2->setCurrentText(settings.value("DoubleDebug/Stop2").toString());
    ui->comboBox_Stream2->setCurrentText(settings.value("DoubleDebug/Stream2").toString());

    ui->checkBox_16In1->setChecked(settings.value("DoubleDebug/Input16_1").toBool());
    ui->checkBox_16In2->setChecked(settings.value("DoubleDebug/Input16_2").toBool());

    ui->checkBox_16D1->setChecked(settings.value("DoubleDebug/D16_1").toBool());
    ui->checkBox_16D2->setChecked(settings.value("DoubleDebug/D16_2").toBool());

    ui->textEdit_Input1->setText(settings.value("DoubleDebug/InputBuf1").toString());
    ui->textEdit_Input2->setText(settings.value("DoubleDebug/InputBuf2").toString());

    return true;
}


void DoubleDebug::onReadData1(QString msg, qint64 size)
{
    if(ui->checkBox_Stop1->isChecked()){ return; }
    if(ui->textEdit_Display1->toPlainText().length() > 10240){ ui->textEdit_Display1->clear(); }
    ui->textEdit_Display1->moveCursor(QTextCursor::End);
    if(msg != ui->textEdit_Input2->toPlainText().toUpper()){
        ui->textEdit_Display1->insertHtml("<span style=\"color:#ff0000;\">"+msg+"</span>" + " ");
    }else{
        ui->textEdit_Display1->insertPlainText(msg + " ");
    }
    ui->textEdit_Display1->moveCursor(QTextCursor::End);
    auto len = size + ui->lineEdit_ReadCount1->text().toLong();
    ui->lineEdit_ReadCount1->setText(QString::number(len));
    writeLogFile(QString("[%1]:").arg(ui->comboBox_Com1->currentText()) + msg + " ");
}

void DoubleDebug::onReadData2(QString msg, qint64 size)
{
    if(ui->checkBox_Stop2->isChecked()){ return; }
    if(ui->textEdit_Display2->toPlainText().length() > 10240){ ui->textEdit_Display2->clear(); }
    ui->textEdit_Display2->moveCursor(QTextCursor::End);
    if(msg != ui->textEdit_Input1->toPlainText().toUpper()){
        ui->textEdit_Display2->insertHtml("<span style=\"color:#ff0000;\">"+msg+"</span>" + " ");
    }else{
        ui->textEdit_Display2->insertPlainText(msg + " ");
    }
    ui->textEdit_Display2->moveCursor(QTextCursor::End);
    auto len = size + ui->lineEdit_ReadCount2->text().toLong();
    ui->lineEdit_ReadCount2->setText(QString::number(len));
    writeLogFile(QString("[%1]:").arg(ui->comboBox_Com2->currentText()) + msg + " ");
}


void DoubleDebug::on_pushButton_Connect1_clicked()
{
    if(m_pSerial1->serialPortOpen(ui->comboBox_Com1->currentText())){
        m_pSerial1->setBaudRate(ui->comboBox_Baute1->currentText());
        m_pSerial1->setParity(ui->comboBox_Check1->currentText());
        m_pSerial1->setStopBits(ui->comboBox_Stop1->currentText());
        m_pSerial1->setDataBits(ui->comboBox_Data1->currentText());
        m_pSerial1->setFlowControl(ui->comboBox_Stream1->currentText());
        on_checkBox_16D1_clicked();
        ui->checkBox_DTR->setChecked(true);
        ui->checkBox_RTS->setChecked(true);
        on_checkBox_DTR_clicked();
        on_checkBox_RTS_clicked();
        statusEnabled1(true);
    }
}

void DoubleDebug::on_pushButton_Close1_clicked()
{
    m_pSerial1->serialPortClose();
    ui->checkBox_DTR->setChecked(false);
    ui->checkBox_DCD->setChecked(false);
    ui->checkBox_DSR->setChecked(false);
    ui->checkBox_RNG->setChecked(false);
    ui->checkBox_RTS->setChecked(false);
    ui->checkBox_CTS->setChecked(false);
    statusEnabled1(false);
}

void DoubleDebug::on_pushButton_Connect2_clicked()
{
    if(m_pSerial2->serialPortOpen(ui->comboBox_Com2->currentText())){
        m_pSerial2->setBaudRate(ui->comboBox_Baute2->currentText());
        m_pSerial2->setParity(ui->comboBox_Check2->currentText());
        m_pSerial2->setStopBits(ui->comboBox_Stop2->currentText());
        m_pSerial2->setDataBits(ui->comboBox_Data2->currentText());
        m_pSerial2->setFlowControl(ui->comboBox_Stream2->currentText());
        on_checkBox_16D2_clicked();
        ui->checkBox_DTR2->setChecked(true);
        ui->checkBox_RTS2->setChecked(true);
        on_checkBox_DTR2_clicked();
        on_checkBox_RTS2_clicked();
        statusEnabled2(true);
    }
}

void DoubleDebug::on_pushButton_Close2_clicked()
{
    m_pSerial2->serialPortClose();
    ui->checkBox_DTR2->setChecked(false);
    ui->checkBox_DCD2->setChecked(false);
    ui->checkBox_DSR2->setChecked(false);
    ui->checkBox_RNG2->setChecked(false);
    ui->checkBox_RTS2->setChecked(false);
    ui->checkBox_CTS2->setChecked(false);
    statusEnabled2(false);
}

void DoubleDebug::on_pushButton_Send1_clicked()
{
    QString msg = ui->textEdit_Input1->toPlainText().trimmed();
    qint64 re = 0;
    if(ui->checkBox_16In1->isChecked()){
        re = this->m_pSerial1->writeHex(msg);
    }else{
        re = this->m_pSerial1->writeAscii(msg);
    }
    ui->lineEdit_SendCount1->setText(QString::number(re + ui->lineEdit_SendCount1->text().toLong()));
}

void DoubleDebug::on_pushButton_Send2_clicked()
{
    QString msg = ui->textEdit_Input2->toPlainText().trimmed();
    qint64 re = 0;
    if(ui->checkBox_16In2->isChecked()){
        re = this->m_pSerial2->writeHex(msg);
    }else{
        re = this->m_pSerial2->writeAscii(msg);
    }
    ui->lineEdit_SendCount2->setText(QString::number(re + ui->lineEdit_SendCount2->text().toLong()));
}

void DoubleDebug::on_pushButton_ClearDisplay1_clicked()
{
    ui->textEdit_Display1->clear();
    ui->lineEdit_SendCount1->setText(tr("0"));
    ui->lineEdit_ReadCount1->setText(tr("0"));
}

void DoubleDebug::on_pushButton_ClearDisplay2_clicked()
{
    ui->textEdit_Display2->clear();
    ui->lineEdit_SendCount2->setText(tr("0"));
    ui->lineEdit_ReadCount2->setText(tr("0"));
}


void DoubleDebug::on_checkBox_DTR_clicked()
{
    m_pSerial1->setDTR(ui->checkBox_DTR->isChecked());
}

void DoubleDebug::on_checkBox_RTS_clicked()
{
    m_pSerial1->setRTS(ui->checkBox_RTS->isChecked());
}

void DoubleDebug::on_checkBox_DTR2_clicked()
{
    m_pSerial2->setDTR(ui->checkBox_DTR2->isChecked());
}

void DoubleDebug::on_checkBox_RTS2_clicked()
{
    m_pSerial2->setRTS(ui->checkBox_RTS2->isChecked());
}

void DoubleDebug::on_checkBox_Times_clicked()
{
    if(ui->checkBox_Times->isChecked()){
        m_timesSend->start(ui->spinBox_times->value());
    }else {
        m_timesSend->stop();
    }
}

void DoubleDebug::on_pushButton_Scan_clicked()
{
    ui->lineEdit_FilePath->setText(
        QFileDialog::getSaveFileName(this,tr("导出日志文件"), ".", tr("*.txt"))
    );
}

void DoubleDebug::on_checkBox_16In1_clicked()
{
    if(ui->checkBox_16In1->isChecked()){
        QString buf = ui->textEdit_Input1->toPlainText().trimmed();
        if(!buf.isEmpty()){
            QByteArray byteBuf(buf.toUtf8());
            ui->textEdit_Input1->setPlainText(byteBuf.toHex(' ').toLower());
        }
    }else{
        QString buf = ui->textEdit_Input1->toPlainText().trimmed().replace(" ", "");
        ui->textEdit_Input1->setPlainText(QByteArray::fromHex(buf.toLocal8Bit()));
    }
}

void DoubleDebug::on_checkBox_16In2_clicked()
{
    if(ui->checkBox_16In2->isChecked()){
        QString buf = ui->textEdit_Input2->toPlainText().trimmed();
        if(!buf.isEmpty()){
            QByteArray byteBuf(buf.toUtf8());
            ui->textEdit_Input2->setPlainText(byteBuf.toHex(' ').toLower());
        }
    }else{
        QString buf = ui->textEdit_Input2->toPlainText().trimmed().replace(" ", "");
        ui->textEdit_Input2->setPlainText(QByteArray::fromHex(buf.toLocal8Bit()));
    }
}

void DoubleDebug::on_textEdit_Input1_textChanged()
{
    if(ui->checkBox_16In1->isChecked()){
        QString sendBuf = ui->textEdit_Input1->toPlainText();
        if(sendBuf.isEmpty()){
            return;
        }
        QRegExp rx(QString("[0-9A-Fa-f ]+"));
        bool match = rx.exactMatch(sendBuf);
        if(!match){
            sendBuf.remove(QRegularExpression("[^0-9A-Fa-f ]+"));
            qDebug() << sendBuf;
            ui->textEdit_Input1->setText(sendBuf);
            ui->textEdit_Input1->moveCursor(QTextCursor::End);
        }
    }
}

void DoubleDebug::on_textEdit_Input2_textChanged()
{
    if(ui->checkBox_16In2->isChecked()){
        QString sendBuf = ui->textEdit_Input2->toPlainText();
        if(sendBuf.isEmpty()){
            return;
        }
        QRegExp rx(QString("[0-9A-Fa-f ]+"));
        bool match = rx.exactMatch(sendBuf);
        if(!match){
            sendBuf.remove(QRegularExpression("[^0-9A-Fa-f ]+"));
            qDebug() << sendBuf;
            ui->textEdit_Input2->setText(sendBuf);
            ui->textEdit_Input2->moveCursor(QTextCursor::End);
        }
    }
}

void DoubleDebug::on_checkBox_16D1_clicked()
{
    m_pSerial1->setOutHex(ui->checkBox_16D1->isChecked());
}

void DoubleDebug::on_checkBox_16D2_clicked()
{
    m_pSerial2->setOutHex(ui->checkBox_16D2->isChecked());
}

void DoubleDebug::writeLogFile(QString msg)
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

void DoubleDebug::statusEnabled1(bool status)
{
    ui->comboBox_Com1->setEnabled(!status);
    ui->pushButton_Connect1->setEnabled(!status);
    ui->pushButton_Close1->setEnabled(status);
    ui->pushButton_Send1->setEnabled(status);
    bool flag = ui->pushButton_Connect1->isEnabled() || ui->pushButton_Connect2->isEnabled();
    ui->checkBox_Times->setEnabled(!flag);
}

void DoubleDebug::statusEnabled2(bool status)
{
    ui->comboBox_Com2->setEnabled(!status);
    ui->pushButton_Connect2->setEnabled(!status);
    ui->pushButton_Close2->setEnabled(status);
    ui->pushButton_Send2->setEnabled(status);
    bool flag = ui->pushButton_Connect1->isEnabled() || ui->pushButton_Connect2->isEnabled();
    ui->checkBox_Times->setEnabled(!flag);
}
