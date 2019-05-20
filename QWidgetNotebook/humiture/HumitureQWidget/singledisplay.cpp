#include "singledisplay.h"
#include "ui_singledisplay.h"

SingleDisplay::SingleDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleDisplay)
{
    ui->setupUi(this);
    this->pHumitureCharts = new HumitureCharts();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->pHumitureCharts);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

}

SingleDisplay::~SingleDisplay()
{
    delete ui;
}
