#include <QDebug>
#include <QDate>
#include "humiturerecord.h"
#include "ui_humiturerecord.h"

HumitureRecordPage::HumitureRecordPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HumitureRecordPage)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    this->pHumitureCharts = new RecordHumitureChart();
    ui->vlayout_chart->addWidget(this->pHumitureCharts);

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(onQueryBtn()));
}

HumitureRecordPage::~HumitureRecordPage()
{
    delete ui;
}

void HumitureRecordPage::onQueryBtn()
{
    QString date = ui->dateEdit->text();
    qDebug() << "date: " << date;

}
void HumitureRecordPage::onHumitureRecord(QVariant msg)
{

}

RecordHumitureChart::RecordHumitureChart(HumitureCharts *parent) : HumitureCharts(parent)
{

}
