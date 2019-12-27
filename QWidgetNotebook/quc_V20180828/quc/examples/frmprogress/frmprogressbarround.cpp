#pragma execution_character_set("utf-8")

#include "frmprogressbarround.h"
#include "ui_frmprogressbarround.h"
#include "progressbarround.h"

frmProgressBarRound::frmProgressBarRound(QWidget *parent) : QWidget(parent), ui(new Ui::frmProgressBarRound)
{
    ui->setupUi(this);
    this->initForm();
}

frmProgressBarRound::~frmProgressBarRound()
{
    delete ui;
}

void frmProgressBarRound::initForm()
{
    //默认样式+值格式+圆环颜色
    QPalette p1;
    p1.setColor(QPalette::Highlight, QColor(24, 189, 155));
    p1.setColor(QPalette::Text, QColor(24, 189, 155));
    p1.setColor(QPalette::Shadow, QColor(24, 189, 155));
    ui->progressBarRound1->setPalette(p1);
    ui->progressBarRound1->setFormat("%v");
    ui->progressBarRound1->setPrecision(0);
    ui->progressBarRound1->setNullPosition(0);
    connectToSlider(ui->progressBarRound1);

    //饼状图+背景色+起始位置
    QPalette p2;
    p2.setColor(QPalette::Base, QColor(97, 117, 118));
    p2.setColor(QPalette::Text, QColor(250, 250, 250));
    p2.setColor(QPalette::Highlight, QColor(255, 107, 107));
    p2.setColor(QPalette::Shadow, QColor(255, 107, 107));
    ui->progressBarRound2->setPalette(p2);
    ui->progressBarRound2->setNullPosition(ProgressBarRound::PositionRight);
    ui->progressBarRound2->setBarStyle(ProgressBarRound::BarStyle_Pie);
    connectToSlider(ui->progressBarRound2);

    //线状图+线颜色
    QPalette p3;
    p3.setColor(QPalette::Highlight, QColor(100, 184, 255));
    p3.setColor(QPalette::Text, QColor(250, 250, 250));
    ui->progressBarRound3->setPalette(p3);
    ui->progressBarRound3->setBarStyle(ProgressBarRound::BarStyle_Line);
    ui->progressBarRound3->setOutlinePenWidth(5);
    ui->progressBarRound3->setDataPenWidth(5);
    connectToSlider(ui->progressBarRound3);

    //默认样式+自定义颜色组合
    QPalette p4;
    p4.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    p4.setColor(QPalette::Text, QColor(255, 107, 107));
    p4.setColor(QPalette::Highlight, QColor(255, 107, 107));
    p4.setColor(QPalette::Shadow, QColor(255, 107, 107));
    ui->progressBarRound4->setPalette(p4);
    ui->progressBarRound4->setNullPosition(ProgressBarRound::PositionLeft);
    ui->progressBarRound4->setPrecision(0);

    QGradientStops gradientPoints4;
    gradientPoints4 << QGradientStop(0, QColor(254, 64, 64)) << QGradientStop(0.5, QColor(255, 127, 36)) << QGradientStop(1, QColor(238, 238, 0));
    ui->progressBarRound4->setGradientData(gradientPoints4);
    connectToSlider(ui->progressBarRound4);

    //饼图样式+自定义颜色组合
    QPalette p5;
    p5.setColor(QPalette::Highlight, QColor(24, 189, 155));
    p5.setBrush(QPalette::Base, QColor(255, 107, 107));
    p5.setColor(QPalette::Text, QColor(250, 250, 250));
    p5.setColor(QPalette::Shadow, QColor(24, 189, 155));
    ui->progressBarRound5->setPalette(p5);
    ui->progressBarRound5->setNullPosition(ProgressBarRound::PositionLeft);
    ui->progressBarRound5->setBarStyle(ProgressBarRound::BarStyle_Pie);
    connectToSlider(ui->progressBarRound5);

    //线状样式+自定义边框宽度
    ui->progressBarRound6->setPalette(p3);
    ui->progressBarRound6->setPrecision(2);
    ui->progressBarRound6->setBarStyle(ProgressBarRound::BarStyle_Line);
    ui->progressBarRound6->setOutlinePenWidth(18);
    ui->progressBarRound6->setDataPenWidth(10);
    ui->progressBarRound6->setClockWise(false);
    connectToSlider(ui->progressBarRound6);
}

void frmProgressBarRound::connectToSlider(ProgressBarRound *bar)
{
    bar->setRange(ui->slider->minimum(), ui->slider->maximum());
    bar->setValue(ui->slider->value());
    connect(ui->slider, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int)));
}
