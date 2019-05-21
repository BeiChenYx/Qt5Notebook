#include "singledisplay.h"
#include "ui_singledisplay.h"

SingleDisplay::SingleDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleDisplay)
{
    ui->setupUi(this);
    this->deviceAddr = 1;

    this->pHumitureCharts = new SingleHumitureChart();
    this->pHumitureCharts->setBackground(":/image/single.png");
//    this->pHumitureCharts->setChartTile(QString("设备%1的温湿度单点测试").arg(this->deviceAddr));
    this->plineT = this->pHumitureCharts->createLineSerie("温度", true);
    this->plineH = this->pHumitureCharts->createLineSerie("湿度", false);
    this->plineT->setColor(QColor("red"));
    this->plineH->setColor(QColor("green"));

    // 增加温湿度实时显示的 QLabel
    ui->label_T->setStyleSheet("color: \"red\"");
    ui->label_H->setStyleSheet("color: \"green\"");

    ui->vlayout_chart->addWidget(this->pHumitureCharts);
    ui->vlayout_chart->setSpacing(0);
    ui->vlayout_chart->setContentsMargins(0, 40, 0, 0);
}

SingleDisplay::~SingleDisplay()
{
    delete ui;
}
void SingleDisplay::onHumitureDisplay(int deviceAddr, double temperature, double humidity)
{
    this->plineT->append(QDateTime::currentMSecsSinceEpoch(), temperature);
    this->plineH->append(QDateTime::currentMSecsSinceEpoch(), humidity);
    ui->label_T->setText(QString("%1").arg(temperature, 0, 'f', 2));
    ui->label_H->setText(QString("%1").arg(humidity, 0, 'f', 2));
    ui->label_addr->setText(QString("%1").arg(deviceAddr));
}

void SingleDisplay::onCurrentDeviceAddr(int deviceAddr)
{
    this->deviceAddr = deviceAddr;
    this->pHumitureCharts->setChartTile(QString("设备%1的温湿度单点测试").arg(deviceAddr));
    this->plineH->clear();
    this->plineT->clear();
    emit currentAddr(deviceAddr);
}

SingleHumitureChart::SingleHumitureChart(HumitureCharts *parent) : HumitureCharts(parent)
{

}
