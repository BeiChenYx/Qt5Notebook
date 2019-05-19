#include "singleconfig.h"
#include "ui_singleconfig.h"

SingleConfig::SingleConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleConfig)
{
    ui->setupUi(this);
}

SingleConfig::~SingleConfig()
{
    delete ui;
}
