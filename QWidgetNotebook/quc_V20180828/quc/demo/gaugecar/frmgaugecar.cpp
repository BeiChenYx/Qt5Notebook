#pragma execution_character_set("utf-8")

#include "frmgaugecar.h"
#include "ui_frmgaugecar.h"

frmGaugeCar::frmGaugeCar(QWidget *parent) : QWidget(parent), ui(new Ui::frmGaugeCar)
{
    ui->setupUi(this);
    this->initForm();
}

frmGaugeCar::~frmGaugeCar()
{
    delete ui;
}

void frmGaugeCar::initForm()
{
    ui->gaugeCar1->setAnimation(true);
    ui->gaugeCar1->setPieColorEnd(QColor(184, 190, 200));

    ui->gaugeCar2->setAnimation(true);
    ui->gaugeCar2->setShowOverlay(true);
    ui->gaugeCar2->setPointerColor(QColor(0, 255, 255));
    ui->gaugeCar2->setCenterCircleColor(QColor(255, 255, 255));

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->gaugeCar1, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->gaugeCar2, SLOT(setValue(int)));
    ui->horizontalSlider->setValue(88);
}
