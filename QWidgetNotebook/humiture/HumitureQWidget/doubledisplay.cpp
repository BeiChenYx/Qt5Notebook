#include "doubledisplay.h"
#include "ui_doubledisplay.h"

DoubleDisplay::DoubleDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleDisplay)
{
    ui->setupUi(this);
}

DoubleDisplay::~DoubleDisplay()
{
    delete ui;
}
