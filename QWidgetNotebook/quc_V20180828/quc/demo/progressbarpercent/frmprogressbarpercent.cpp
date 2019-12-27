#pragma execution_character_set("utf-8")

#include "frmprogressbarpercent.h"
#include "ui_frmprogressbarpercent.h"
#include "progressbarpercent.h"

frmProgressBarPercent::frmProgressBarPercent(QWidget *parent) : QWidget(parent), ui(new Ui::frmProgressBarPercent)
{
    ui->setupUi(this);
    this->initForm();
}

frmProgressBarPercent::~frmProgressBarPercent()
{
    delete ui;
}

void frmProgressBarPercent::initForm()
{
    bars << ui->progressBarPercent1 << ui->progressBarPercent2 << ui->progressBarPercent3
         << ui->progressBarPercent4 << ui->progressBarPercent5 << ui->progressBarPercent6;

    ui->progressBarPercent2->setUsedColor(QColor(255, 107, 107));
    ui->progressBarPercent3->setUsedColor(QColor(24, 189, 155));
    ui->progressBarPercent5->setUsedColor(QColor(255, 107, 107));
    ui->progressBarPercent6->setUsedColor(QColor(24, 189, 155));

    ui->progressBarPercent4->setShowPercent(false);
    ui->progressBarPercent5->setShowPercent(false);
    ui->progressBarPercent6->setShowPercent(false);

    connect(ui->rbtnArc, SIGNAL(clicked(bool)), this, SLOT(rbtnClick()));
    connect(ui->rbtnPolo, SIGNAL(clicked(bool)), this, SLOT(rbtnClick()));
    connect(ui->rbtnWave, SIGNAL(clicked(bool)), this, SLOT(rbtnClick()));
    connect(ui->rbtnArcPolo, SIGNAL(clicked(bool)), this, SLOT(rbtnClick()));
    connect(ui->rbtnArcWave, SIGNAL(clicked(bool)), this, SLOT(rbtnClick()));

    QColor circleColor(70, 70, 70, 100);
    foreach (ProgressBarPercent *bar, bars) {
        bar->setCircleColor(circleColor);
        connect(ui->horizontalSlider1, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int)));
        connect(ui->horizontalSlider2, SIGNAL(valueChanged(int)), bar, SLOT(setNullPosition(int)));
    }

    ui->rbtnArcWave->click();
    ui->horizontalSlider1->setValue(88);
}

void frmProgressBarPercent::rbtnClick()
{
    QRadioButton *rbtn = (QRadioButton *)sender();
    QString text = rbtn->text();

    if (text == "圆弧") {
        foreach (ProgressBarPercent *bar, bars) {
            bar->setPercentStyle(ProgressBarPercent::PercentStyle_Arc);
        }
    } else if (text == "水池") {
        foreach (ProgressBarPercent *bar, bars) {
            bar->setPercentStyle(ProgressBarPercent::PercentStyle_Polo);
        }
    } else if (text == "波纹") {
        foreach (ProgressBarPercent *bar, bars) {
            bar->setPercentStyle(ProgressBarPercent::PercentStyle_Wave);
        }
    } else if (text == "圆弧水池") {
        foreach (ProgressBarPercent *bar, bars) {
            bar->setPercentStyle(ProgressBarPercent::PercentStyle_Arc_Polo);
        }
    } else if (text == "圆弧波纹") {
        foreach (ProgressBarPercent *bar, bars) {
            bar->setPercentStyle(ProgressBarPercent::PercentStyle_Arc_Wave);
        }
    }
}

void frmProgressBarPercent::on_ckClockWise_stateChanged(int arg1)
{
    bool ok = (arg1 != 0);
    foreach (ProgressBarPercent *bar, bars) {
        bar->setClockWise(ok);
    }
}

void frmProgressBarPercent::on_ckBorderOut_stateChanged(int arg1)
{
    bool ok = (arg1 != 0);
    foreach (ProgressBarPercent *bar, bars) {
        bar->setBorderOut(ok);
    }
}
