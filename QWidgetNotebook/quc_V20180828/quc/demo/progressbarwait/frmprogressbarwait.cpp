#pragma execution_character_set("utf-8")

#include "frmprogressbarwait.h"
#include "ui_frmprogressbarwait.h"

frmProgressBarWait::frmProgressBarWait(QWidget *parent) : QWidget(parent), ui(new Ui::frmProgressBarWait)
{
    ui->setupUi(this);
    this->initForm();
}

frmProgressBarWait::~frmProgressBarWait()
{
    delete ui;
}

void frmProgressBarWait::initForm()
{
    ui->progressBarWait1->setShowPercent(true);
    ui->progressBarWait2->setShowPercent(true);
    ui->progressBarWait3->setShowPercent(false);
    ui->progressBarWait4->setShowPercent(true);
    ui->progressBarWait5->setShowPercent(true);
    ui->progressBarWait6->setShowPercent(true);
    ui->progressBarWait7->setShowPercent(false);
    ui->progressBarWait8->setShowPercent(false);

    ui->progressBarWait1->setBarStyle(ProgressBarWait::BarStyle_Line);

    ui->progressBarWait2->setForeground(QColor(225, 102, 255));
    ui->progressBarWait2->setTextColor(QColor(225, 102, 255));
    ui->progressBarWait2->setBarStyle(ProgressBarWait::BarStyle_RoundCircle);

    ui->progressBarWait3->setForeground(QColor(24, 189, 155));
    ui->progressBarWait3->setBarStyle(ProgressBarWait::BarStyle_Arc);

    ui->progressBarWait4->setForeground(QColor(255, 107, 107));
    ui->progressBarWait4->setTextColor(QColor(30, 30, 30));
    ui->progressBarWait4->setBarStyle(ProgressBarWait::BarStyle_Pie);

    ui->progressBarWait5->setClockWise(false);
    ui->progressBarWait5->setForeground(QColor(255, 107, 107));
    ui->progressBarWait5->setTextColor(QColor(255, 107, 107));
    ui->progressBarWait5->setBarStyle(ProgressBarWait::BarStyle_Line);

    ui->progressBarWait6->setForeground(QColor(24, 189, 155));
    ui->progressBarWait6->setTextColor(QColor(24, 189, 155));
    ui->progressBarWait6->setBarStyle(ProgressBarWait::BarStyle_Ring);

    ui->progressBarWait7->setBarStyle(ProgressBarWait::BarStyle_SingleCircle);

    ui->progressBarWait8->setBarStyle(ProgressBarWait::BarStyle_DoubleCircle);
}
