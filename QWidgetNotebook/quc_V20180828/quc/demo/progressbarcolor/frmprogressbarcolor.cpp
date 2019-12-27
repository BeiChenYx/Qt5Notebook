#pragma execution_character_set("utf-8")

#include "frmprogressbarcolor.h"
#include "ui_frmprogressbarcolor.h"

frmProgressBarColor::frmProgressBarColor(QWidget *parent) : QWidget(parent), ui(new Ui::frmProgressBarColor)
{
    ui->setupUi(this);
    this->initForm();
}

frmProgressBarColor::~frmProgressBarColor()
{
    delete ui;
}

void frmProgressBarColor::initForm()
{
    ui->progressBarColor1->setTextColor(QColor(250, 250, 250));
    ui->progressBarColor1->setBarBgColor(QColor(30, 30, 30));

    ui->progressBarColor2->setBarColor(QColor(255, 107, 107));
    ui->progressBarColor2->setPrecision(1);
    ui->progressBarColor2->setStep(5);

    ui->progressBarColor3->setTextColor(QColor(250, 250, 250));
    ui->progressBarColor3->setBarBgColor(QColor(80, 80, 80));
    ui->progressBarColor3->setBarColor(QColor(24, 189, 155));
    ui->progressBarColor3->setStep(7);

    connect(ui->horizontalSlider1, SIGNAL(valueChanged(int)), ui->progressBarColor1, SLOT(setValue(int)));
    connect(ui->horizontalSlider2, SIGNAL(valueChanged(int)), ui->progressBarColor2, SLOT(setValue(int)));
    connect(ui->horizontalSlider3, SIGNAL(valueChanged(int)), ui->progressBarColor3, SLOT(setValue(int)));

    ui->progressBarColor1->setRange(0, 100);
    ui->progressBarColor2->setRange(0, 200);
    ui->progressBarColor3->setRange(-100, 100);

    ui->horizontalSlider1->setRange(0, 100);
    ui->horizontalSlider2->setRange(0, 200);
    ui->horizontalSlider3->setRange(-100, 100);

    ui->horizontalSlider1->setValue(30);
    ui->horizontalSlider2->setValue(50);
    ui->horizontalSlider3->setValue(70);
}
