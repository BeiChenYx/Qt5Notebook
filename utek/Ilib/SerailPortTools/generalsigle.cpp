#include "generalsigle.h"
#include "ui_generalsigle.h"
#include <QFileInfo>
#include <QSettings>


GeneralSigle::GeneralSigle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::generalSigle),
    m_timesSend(new QTimer(this)),
    m_fileSend(new QTimer(this))
{
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(m_timesSend, &QTimer::timeout, this, &GeneralSigle::on_pushButton_send_clicked);
    connect(m_fileSend, &QTimer::timeout, this, &GeneralSigle::on_sendFile);
}

GeneralSigle::~GeneralSigle()
{
    delete ui;
}

bool GeneralSigle::writeInitFile(){
    /*
     * 初始化配置文件
     * 如果配置文件不存在则使用初始化值来初始化界面和文件
    */
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    settings.setValue("GeneralSigle/SendBuf", ui->textEdit->toPlainText());
    settings.setValue("GeneralSigle/SendTimes", ui->spinBox_times->value());
    settings.setValue("GeneralSigle/Set16", ui->checkBox_hex->isChecked());
    settings.setValue("GeneralSigle/NewLine", ui->checkBox_enter->isChecked());

    return true;
}
bool GeneralSigle::readInitFile(){
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    ui->textEdit->insertPlainText(settings.value("GeneralSigle/SendBuf").toString());
    ui->spinBox_times->setValue(settings.value("GeneralSigle/SendTimes").toInt());
    ui->checkBox_hex->setChecked(settings.value("GeneralSigle/Set16").toBool());
    ui->checkBox_enter->setChecked(settings.value("GeneralSigle/NewLine").toBool());
    return true;
}

void GeneralSigle::on_pushButton_send_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty()){ return; }
    QString buf = ui->textEdit->toPlainText().trimmed();
    QString newLine = ui->checkBox_enter->isChecked() ? QString("\r\n") : QString();
    emit sendData(buf + newLine, ui->checkBox_hex->isChecked());
}

void GeneralSigle::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

void GeneralSigle::on_pushButton_scan_clicked()
{
    ui->lineEdit_path->setText(QFileDialog::getOpenFileName(this, tr("open file"), ".", tr("*.*")) );
}

void GeneralSigle::on_pushButton_send_file_clicked()
{
    m_index = 0;
    m_fileBuf.clear();
    auto file = new QFile(ui->lineEdit_path->text());
    if(file->open(QIODevice::ReadOnly)){
        m_fileBuf = file->readAll();
        file->close();
        ui->progressBar->show();
        ui->progressBar->setRange(0, m_fileBuf.length());
        m_fileSend->start(50);
    }
}

void GeneralSigle::on_checkBox_times_clicked()
{
    if(ui->checkBox_times->isChecked()){
        m_timesSend->start(ui->spinBox_times->value());
    }else {
        m_timesSend->stop();
    }
}

void GeneralSigle::on_sendFile()
{
    if(m_index < m_fileBuf.length()){
        auto buf = m_fileBuf.mid(m_index, 1024);
        m_index += buf.length();
        emit sendData(buf, false);
        ui->progressBar->setValue(m_index);
    }else{
        m_fileSend->stop();
        ui->progressBar->setValue(m_fileBuf.length());
        ui->progressBar->hide();
        QMessageBox::information(this, tr("文件发送"), tr("文件发出完成"));
    }
}


void GeneralSigle::on_checkBox_hex_clicked()
{
    if(ui->checkBox_hex->isChecked()){
        ui->checkBox_enter->setChecked(false);
        QString buf = ui->textEdit->toPlainText().trimmed();
        if(!buf.isEmpty()){
            QByteArray byteBuf(buf.toUtf8());
            ui->textEdit->setPlainText(byteBuf.toHex(' ').toLower());
        }
    }else{
        QString buf = ui->textEdit->toPlainText().trimmed().replace(" ", "");
        ui->textEdit->setPlainText(QByteArray::fromHex(buf.toLocal8Bit()));
    }
}

void GeneralSigle::on_checkBox_enter_clicked()
{
    if(ui->checkBox_enter->isChecked()){
        if(ui->checkBox_hex->isChecked()){
            QString buf = ui->textEdit->toPlainText().trimmed().replace(" ", "");
            ui->textEdit->setPlainText(QByteArray::fromHex(buf.toLocal8Bit()));
        }
        ui->checkBox_hex->setChecked(false);
    }
}

void GeneralSigle::statusEnabled(bool status)
{
    ui->pushButton_send->setEnabled(status);
    ui->checkBox_times->setEnabled(status);
}
