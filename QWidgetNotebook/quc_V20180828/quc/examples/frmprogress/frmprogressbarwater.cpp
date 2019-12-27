#pragma execution_character_set("utf-8")

#include "frmprogressbarwater.h"
#include "ui_frmprogressbarwater.h"
#include "progressbarwater.h"

frmProgressBarWater::frmProgressBarWater(QWidget *parent) : QWidget(parent), ui(new Ui::frmProgressBarWater)
{
    ui->setupUi(this);
    this->initForm();
}

frmProgressBarWater::~frmProgressBarWater()
{
    delete ui;
}

void frmProgressBarWater::initForm()
{    
    ui->progressBarWater1->setUsedColor(QColor(255, 107, 107));
    ui->progressBarWater2->setUsedColor(QColor(24, 189, 155));

    connect(ui->sliderValue, SIGNAL(valueChanged(int)), ui->progressBarWater1, SLOT(setValue(int)));
    connect(ui->sliderValue, SIGNAL(valueChanged(int)), ui->progressBarWater2, SLOT(setValue(int)));

    ui->sliderValue->setValue(66);
    ui->sliderBorderWidth->setValue(10);
    ui->sliderWaterHeight->setValue(5);
    ui->sliderWaterDensity->setValue(2);
    ui->cboxPercentStyle->setCurrentIndex(1);
}

void frmProgressBarWater::on_sliderBorderWidth_valueChanged(int value)
{
    double v = (double)value;
    ui->progressBarWater1->setBorderWidth(v);
    ui->progressBarWater2->setBorderWidth(v);
}

void frmProgressBarWater::on_sliderWaterHeight_valueChanged(int value)
{
    double v = (double)value / 100;
    ui->progressBarWater1->setWaterHeight(v);
    ui->progressBarWater2->setWaterHeight(v);
}

void frmProgressBarWater::on_sliderWaterDensity_valueChanged(int value)
{
    double v = (double)value;
    ui->progressBarWater1->setWaterDensity(v);
    ui->progressBarWater2->setWaterDensity(v);
}

void frmProgressBarWater::on_ckReverse_stateChanged(int arg1)
{
    bool reverse = (arg1 != 0);
    ui->progressBarWater1->setReverse(reverse);
    ui->progressBarWater2->setReverse(reverse);
}

void frmProgressBarWater::on_cboxPercentStyle_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "矩形风格") {
        ui->progressBarWater1->setPercentStyle(ProgressBarWater::PercentStyle_Rect);
        ui->progressBarWater2->setPercentStyle(ProgressBarWater::PercentStyle_Rect);
    } else if (arg1 == "圆形风格") {
        ui->progressBarWater1->setPercentStyle(ProgressBarWater::PercentStyle_Circle);
        ui->progressBarWater2->setPercentStyle(ProgressBarWater::PercentStyle_Circle);
    } else if (arg1 == "椭圆风格") {
        ui->progressBarWater1->setPercentStyle(ProgressBarWater::PercentStyle_Ellipse);
        ui->progressBarWater2->setPercentStyle(ProgressBarWater::PercentStyle_Ellipse);
    } else if (arg1 == "圆柱风格") {
        ui->progressBarWater1->setPercentStyle(ProgressBarWater::PercentStyle_Cylinder);
        ui->progressBarWater2->setPercentStyle(ProgressBarWater::PercentStyle_Cylinder);
    }
}

void frmProgressBarWater::on_btnPause_clicked()
{
    if (ui->btnPause->text() == "暂停") {
        ui->progressBarWater1->stop();
        ui->progressBarWater2->stop();
        ui->btnPause->setText("启动");
    } else {
        ui->progressBarWater1->start();
        ui->progressBarWater2->start();
        ui->btnPause->setText("暂停");
    }
}
