#pragma execution_character_set("utf-8")

#include "frmgaugesimple.h"
#include "ui_frmgaugesimple.h"

frmGaugeSimple::frmGaugeSimple(QWidget *parent) : QWidget(parent), ui(new Ui::frmGaugeSimple)
{
    ui->setupUi(this);
    this->initForm();
}

frmGaugeSimple::~frmGaugeSimple()
{
    delete ui;
}

void frmGaugeSimple::initForm()
{    
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->gaugeSimple, SLOT(setValue(int)));
    ui->horizontalSlider->setValue(88);
}
