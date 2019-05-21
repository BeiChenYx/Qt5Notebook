#include "humiturecharts.h"
#include <QHBoxLayout>
#include <QDebug>

HumitureCharts::HumitureCharts(QWidget *parent) : QWidget(parent)
{
    this->pChart = new QChart();
    this->pAxisX = new QDateTimeAxis();
    this->pAxisT = new QValueAxis();
    this->pAxisH = new QValueAxis();

    this->pChart->setBackgroundVisible(false);
    this->pChart->legend()->setAlignment(Qt::AlignRight);

    // 横坐标
    this->pAxisX->setTickCount(7);
    this->pAxisX->setFormat("hh:mm:ss");
    QString now = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd");
    QDateTime zero = QDateTime::fromString(now + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime end = QDateTime::fromString(now + " 23:59:59", "yyyy-MM-dd hh:mm:ss");
    this->pAxisX->setRange(zero, end);
    this->pAxisX->setLabelsColor(QColor("#3270E1"));
    this->pAxisX->setGridLineColor(QColor("#4A5877"));
    this->pAxisX->setGridLineVisible(true);

    // 温度坐标
    this->pAxisT->setTickCount(5);
    this->pAxisT->setMax(45);
    this->pAxisT->setMin(-25);
    this->pAxisT->setLabelFormat("%.2f");
    this->pAxisT->setTitleText("<span style=\"color:#3270E1;\">温度</span>");
    this->pAxisT->setGridLineVisible(true);
    this->pAxisT->setLabelsColor(QColor("#3270E1"));
    this->pAxisT->setLinePenColor(QColor("#3270E1"));
    this->pAxisT->setGridLineColor(QColor("#4A5877"));

    // 湿度坐标
    this->pAxisH->setTitleText("<span style=\"color:#3270E1;\">湿度</span>");
    this->pAxisH->setTickCount(5);
    this->pAxisH->setMax(100);
    this->pAxisH->setMin(0);
    this->pAxisH->setGridLineVisible(false);
    this->pAxisH->setLabelsColor(QColor("#3270E1"));
    this->pAxisH->setLinePenColor(QColor("#3270E1"));

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
    this->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
}

void HumitureCharts::setBackground(QString url)
{
    QString sheet = this->styleSheet();
    sheet += QString("border-image: url(%1);").arg(url);
    this->setStyleSheet(sheet);
}

void HumitureCharts::setChartTile(QString title)
{
    this->pChart->setTitle(QString("<span style=\"color:#3270E1;\">%1</span>").arg(title));
}

QLineSeries* HumitureCharts::createLineSerie(QString title, bool isT, QString color)
{
    QLineSeries *line = new QLineSeries(this);
    this->pChart->addSeries(line);
    if(isT){
        line->attachAxis(this->pAxisT);
    }else {
        line->attachAxis(this->pAxisH);
    }
    line->attachAxis(this->pAxisX);
    line->setName(title);
    if(!color.isEmpty()){
        line->setColor(QColor(color));
    }
    return line;
}
