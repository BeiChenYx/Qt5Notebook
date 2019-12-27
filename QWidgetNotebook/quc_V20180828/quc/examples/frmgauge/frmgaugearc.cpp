#pragma execution_character_set("utf-8")

#include "frmgaugearc.h"
#include "ui_frmgaugearc.h"

frmGaugeArc::frmGaugeArc(QWidget *parent) : QWidget(parent), ui(new Ui::frmGaugeArc)
{
    ui->setupUi(this);
    this->initForm();
}

frmGaugeArc::~frmGaugeArc()
{
    delete ui;
}

void frmGaugeArc::initForm()
{
    ui->widget->setStyleSheet("QWidget#widget{border-image: url(:/image/bg1.jpg);}");

    ui->gaugeArc->setArcColor(QColor(0, 255, 255));
    ui->gaugeArc->setScaleColor(QColor("#00E5EE"));
    ui->gaugeArc->setScaleNumColor(QColor(85, 255, 255));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->gaugeArc, SLOT(setValue(int)));
    ui->horizontalSlider->setValue(88);
}
