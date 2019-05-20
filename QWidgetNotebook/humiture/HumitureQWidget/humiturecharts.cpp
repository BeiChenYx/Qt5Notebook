#include "humiturecharts.h"
#include <QHBoxLayout>

HumitureCharts::HumitureCharts(QWidget *parent) : QWidget(parent)
{
    this->pChart = new QChart();
    this->pAxisX = new QDateTimeAxis();
    this->pAxisT = new QValueAxis();
    this->pAxisH = new QValueAxis();

    this->pChart->setTitle("温湿度图表");

    // 横坐标
    this->pAxisX->setTickCount(7);
    this->pAxisX->setFormat("hh:MM:ss");
    this->pAxisX->setTitleText("时间");
    this->pAxisX->setLabelsColor(QColor("#59BAF2"));
    this->pAxisX->setGridLineColor(QColor("#4A5877"));

    // 温度坐标
    this->pAxisT->setTickCount(5);
    this->pAxisT->setMax(45);
    this->pAxisT->setMin(-25);
    this->pAxisT->setTitleText("温度(℃)");
    this->pAxisT->setGridLineVisible(true);
    this->pAxisT->setLabelsColor(QColor("#59BAF2"));
    this->pAxisT->setGridLineColor(QColor("#4A5877"));

    // 湿度坐标
    this->pAxisH->setTickCount(5);
    this->pAxisH->setMax(100);
    this->pAxisH->setMin(0);
    this->pAxisH->setGridLineVisible(false);
    this->pAxisH->setLabelsColor(QColor("#59BAF2"));

    this->pChart->addAxis(this->pAxisX, Qt::AlignBottom);
    this->pChart->addAxis(this->pAxisT, Qt::AlignLeft);
    this->pChart->addAxis(this->pAxisH, Qt::AlignRight);

    this->pChartView = new QChartView(this->pChart);
    this->pChartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->pChartView);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
}
