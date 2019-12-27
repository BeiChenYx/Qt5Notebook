#pragma execution_character_set("utf-8")

#include "frmgaugedial.h"
#include "ui_frmgaugedial.h"

frmGaugeDial::frmGaugeDial(QWidget *parent) : QWidget(parent), ui(new Ui::frmGaugeDial)
{
    ui->setupUi(this);
    this->initForm();
}

frmGaugeDial::~frmGaugeDial()
{
    delete ui;
}

void frmGaugeDial::initForm()
{    
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->gaugeDial, SLOT(setValue(int)));
    connect(ui->gaugeDial, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
    ui->horizontalSlider->setValue(88);
}
