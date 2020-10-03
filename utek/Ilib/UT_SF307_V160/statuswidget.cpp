#include "statuswidget.h"
#include "ui_statuswidget.h"

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::setInfo(QString hardware, QString mac, QString ip)
{
    ui->label_hardware->setText(hardware);
    ui->label_mac->setText(mac);
    ui->label_ip->setText(ip);
}
