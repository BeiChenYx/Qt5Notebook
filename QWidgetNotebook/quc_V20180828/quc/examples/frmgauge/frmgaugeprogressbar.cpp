#pragma execution_character_set("utf-8")

#include "frmgaugeprogressbar.h"
#include "ui_frmgaugeprogressbar.h"

frmGaugeProgressBar::frmGaugeProgressBar(QWidget *parent) : QWidget(parent), ui(new Ui::frmGaugeProgressBar)
{
    ui->setupUi(this);
    this->initForm();
}

frmGaugeProgressBar::~frmGaugeProgressBar()
{
    delete ui;
}

void frmGaugeProgressBar::initForm()
{
    ui->ckShowPointer->setChecked(true);    
    gauges << ui->gaugeProgressBar1 << ui->gaugeProgressBar2 << ui->gaugeProgressBar3 << ui->gaugeProgressBar4 << ui->gaugeProgressBar5 << ui->gaugeProgressBar6;

    QList<QColor> colors;
    colors << "#47A4E9" << "#00B17D" << "#D64D54" << "#DEAF39" << "#A279C5" << "#009679";

    int count = gauges.count();
    for (int i = 0; i < count; i++) {
        gauges.at(i)->setProgressColor(colors.at(i));
        connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), gauges.at(i), SLOT(setValue(int)));
    }

    ui->horizontalSlider->setValue(88);
}

void frmGaugeProgressBar::on_ckShowPointer_stateChanged(int arg1)
{
    bool check = (arg1 != 0);
    int count = gauges.count();
    for (int i = 0; i < count; i++) {
        gauges.at(i)->setShowPointer(check);
    }
}

void frmGaugeProgressBar::on_ckShowValue_stateChanged(int arg1)
{
    bool check = (arg1 != 0);
    int count = gauges.count();
    for (int i = 0; i < count; i++) {
        gauges.at(i)->setShowValue(check);
    }
}

void frmGaugeProgressBar::on_cboxPointerStyle_currentIndexChanged(int index)
{
    GaugeProgressBar::PointerStyle style = (GaugeProgressBar::PointerStyle)index;
    int count = gauges.count();
    for (int i = 0; i < count; i++) {
        gauges.at(i)->setPointerStyle(style);
    }
}
