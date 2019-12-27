#pragma execution_character_set("utf-8")

#include "frmgaugelpm.h"
#include "ui_frmgaugelpm.h"

frmGaugeLpm::frmGaugeLpm(QWidget *parent) : QWidget(parent), ui(new Ui::frmGaugeLpm)
{
    ui->setupUi(this);
    this->initForm();
}

frmGaugeLpm::~frmGaugeLpm()
{
    delete ui;
}

void frmGaugeLpm::initForm()
{
    //范围值设置为真实范围的100倍,方便处理
    ui->horizontalSlider->setRange(0, 300);
    ui->horizontalSlider->setValue(227);
}

void frmGaugeLpm::on_horizontalSlider_valueChanged(int value)
{
    //得到的值除以100再设置为值
    double v = (double)value / 100;
    ui->gaugeLpm->setValue(v);
}
