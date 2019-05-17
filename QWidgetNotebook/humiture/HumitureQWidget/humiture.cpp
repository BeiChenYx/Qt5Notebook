#include "humiture.h"
#include "ui_humiture.h"

Humiture::Humiture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Humiture)
{
    ui->setupUi(this);

}

Humiture::~Humiture()
{
    delete ui;
}

void Humiture::initUI()
{

}
