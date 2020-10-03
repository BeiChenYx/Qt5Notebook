#include <QFileDialog>
#include "upgradedevice.h"
#include "ui_upgradedevice.h"
#include "FrameLessWidget/framelesswidget.h"

UpgradeDevice::UpgradeDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpgradeDevice)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->textBrowser_info->hide();
}

UpgradeDevice::~UpgradeDevice()
{
    delete ui;
}

void UpgradeDevice::on_pushButton_update_clicked()
{
    if(!m_isHid && !m_isSerial){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID或串口的连接."));
        return;
    }
    if(m_isHid){
        HidTaskPar par;
        par.m_updateFilePath = ui->lineEdit_path->text().trimmed();
        m_pHidWorkThread->pushToTask(HidTask::UPDATE_DEVICE, par);
        ui->progressBar->setValue(0);
        ui->textBrowser_info->clear();
        ui->progressBar->show();
        ui->textBrowser_info->show();
    }
    if(m_isSerial){
        m_pHandleSerial->upgradeDev(ui->lineEdit_path->text().trimmed());
        ui->progressBar->setValue(0);
        ui->textBrowser_info->clear();
        ui->progressBar->show();
        ui->textBrowser_info->show();
    }
}

void UpgradeDevice::on_pushButton_file_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("升级文件"), ".", tr("*.bin"));
    ui->lineEdit_path->setText(fileName);
}

void UpgradeDevice::onUpdateStatus(QString msg)
{
    ui->textBrowser_info->moveCursor(QTextCursor::End);
    ui->textBrowser_info->insertPlainText(msg);
}

void UpgradeDevice::onUpdateResult(bool status)
{
    if(status){
        ui->progressBar->hide();
        ui->textBrowser_info->hide();
    }
}

void UpgradeDevice::onProcessRange(int min, int max)
{
    ui->progressBar->setRange(min, max);
    ui->progressBar->setValue(min);
}

void UpgradeDevice::onProcessBarV(int v)
{
    ui->progressBar->setValue(v);
}
