#include "singledisplay.h"
#include "ui_singledisplay.h"

SingleDisplay::SingleDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleDisplay)
{
    ui->setupUi(this);
}

SingleDisplay::~SingleDisplay()
{
    delete ui;
}
