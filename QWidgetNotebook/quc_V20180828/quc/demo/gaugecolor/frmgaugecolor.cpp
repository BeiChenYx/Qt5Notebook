#pragma execution_character_set("utf-8")

#include "frmgaugecolor.h"
#include "ui_frmgaugecolor.h"

frmGaugeColor::frmGaugeColor(QWidget *parent) : QWidget(parent), ui(new Ui::frmGaugeColor)
{
    ui->setupUi(this);
    this->initForm();
}

frmGaugeColor::~frmGaugeColor()
{
    delete ui;
}

void frmGaugeColor::initForm()
{
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->gaugeColor, SLOT(setValue(int)));
    ui->horizontalSlider->setValue(88);
}
