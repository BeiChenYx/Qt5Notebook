#include "comconfig.h"
#include "ui_comconfig.h"

ComConfig::ComConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComConfig)
{
    ui->setupUi(this);
}

ComConfig::~ComConfig()
{
    delete ui;
}
