#include "singleconfig.h"
#include "ui_singleconfig.h"
#include <QListView>

SingleConfig::SingleConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleConfig)
{
    ui->setupUi(this);
    ui->comboBox_deviceAddr->setView(new QListView);
    for(int i = 1; i < 10; i++){
        ui->comboBox_deviceAddr->addItem(QString("%1").arg(i));
    }

}

SingleConfig::~SingleConfig()
{
    delete ui;
}

void SingleConfig::on_pushButton_set_clicked()
{
    int currentAddr = ui->comboBox_deviceAddr->currentText().toInt();
    emit currentDeviceAddr(currentAddr);
}
