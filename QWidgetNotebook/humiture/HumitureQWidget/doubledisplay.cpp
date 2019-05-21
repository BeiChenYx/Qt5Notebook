#include "doubledisplay.h"
#include "ui_doubledisplay.h"

DoubleDisplay::DoubleDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleDisplay)
{
    ui->setupUi(this);
    this->pHumitureCharts = new DoubleHumitureChart();
    this->pHumitureCharts->setBackground(":/image/double.png");
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->pHumitureCharts);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 20, 0, 0);
    this->setLayout(layout);
}

DoubleDisplay::~DoubleDisplay()
{
    delete ui;
}

void DoubleDisplay::initChart(int deviceAddr)
{
    QLineSeries *plineT = this->pHumitureCharts->createLineSerie(QString("设备%1温度").arg(deviceAddr), true);
    QLineSeries *plineH = this->pHumitureCharts->createLineSerie(QString("设备%1湿度").arg(deviceAddr), false);
    this->deviceNoHandle.insert(QString(QString("%1T").arg(deviceAddr)), plineT);
    this->deviceNoHandle.insert(QString(QString("%1H").arg(deviceAddr)), plineH);
}

void DoubleDisplay::onHumitureDisplay(int deviceAddr, double temperature, double humidity)
{
    QLineSeries *plineT = this->deviceNoHandle.value(QString("%1T").arg(deviceAddr), nullptr);
    QLineSeries *plineH = this->deviceNoHandle.value(QString("%1H").arg(deviceAddr), nullptr);
    if(plineT){
        plineT->append(QDateTime::currentMSecsSinceEpoch(), temperature);
    }
    if(plineH){
        plineH->append(QDateTime::currentMSecsSinceEpoch(), humidity);
    }
}

DoubleHumitureChart::DoubleHumitureChart(HumitureCharts *parent) : HumitureCharts(parent)
{

}
