#include "handlemodbus.h"
#include "leftbartonwidget.h"
#include "ui_leftbartonwidget.h"

LeftBartonWidget::LeftBartonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeftBartonWidget)
{
    ui->setupUi(this);
}

LeftBartonWidget::~LeftBartonWidget()
{
    delete ui;
}

void LeftBartonWidget::setAimT(uint16_t value)
{
    ui->label_target_t->setText(QString("%1°C").arg(static_cast<int16_t>(value) / 100.00));
}

void LeftBartonWidget::setInPa(uint16_t value)
{
    if(g_PA){
        ui->label_indoor_pressure->setText(QString("%1Pa").arg(static_cast<int16_t>(value) / 100.00));
    }else{
        ui->label_indoor_pressure->setText("***Pa");
    }
}

void LeftBartonWidget::setRunStatus(bool status)
{
    ui->label_run_states->setText(status ? tr("运行") : tr("停止"));
    ui->label_run_states->setStyleSheet(status ? "color: #000000" : "color: #DC0000");
}

void LeftBartonWidget::setAirLevel(uint16_t value)
{
    ui->label_air_level->setNum(static_cast<int16_t>(value));
}

void LeftBartonWidget::setAirLevelLimit(uint16_t min, uint16_t max)
{
    ui->label_air_limit_level->setText(QString("%1-%2").arg(static_cast<int16_t>(min)).arg(static_cast<int16_t>(max)));
}

void LeftBartonWidget::setBreadNums(uint16_t heigh, uint16_t low)
{
    uint count = heigh;
    count = (count << 16) | low;
    ui->label_bread_nums->setNum(static_cast<int>(count));
}

void LeftBartonWidget::setCurrntAge(uint16_t age)
{
   ui->label_current_days->setNum(static_cast<int16_t>(age));
}


