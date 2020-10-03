#include "hoverinfowidget.h"
#include "ui_hoverinfowidget.h"

HoverInfoWidget::HoverInfoWidget(QWidget *parent, QString name, QString value, QString symbol) :
    QWidget(parent),
    ui(new Ui::HoverInfoWidget)
{
    ui->setupUi(this);
    this->setName(name);
    this->setValue(value);
    this->setSymbol(symbol);
    ui->label_name->setStyleSheet("color: #000000;}");
    ui->label_value->setStyleSheet("color: #000000;}");
    ui->label_symbol->setStyleSheet("color: #000000;}");
}

HoverInfoWidget::~HoverInfoWidget()
{
    delete ui;
}

void HoverInfoWidget::setValue(QString value)
{
    ui->label_value->setText(value);
}
void HoverInfoWidget::setName(QString name)
{
    ui->label_name->setText(name);
}
void HoverInfoWidget::setSymbol(QString value)
{
    ui->label_symbol->setText(value);
}
