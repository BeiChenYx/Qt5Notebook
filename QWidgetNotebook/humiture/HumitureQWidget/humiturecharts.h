#ifndef HUMITURECHARTS_H
#define HUMITURECHARTS_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>


// QChart 的命名空间
QT_CHARTS_USE_NAMESPACE


/*
* 创建温湿度的折线图，横坐标提供时间坐标，左边温度坐标，右边湿度坐标
* 单点测试和多点测试，以及历史记录可以继承此类；
*/
class HumitureCharts : public QWidget
{
    Q_OBJECT
public:
    explicit HumitureCharts(QWidget *parent = nullptr);

signals:

public slots:

private:
    // 图表视图
    QChartView        *pChartView;
    // 图表
    QChart            *pChart;
    // 坐标轴
    QDateTimeAxis     *pAxisX;
    QValueAxis        *pAxisT; // 温度
    QValueAxis        *pAxisH; // 湿度
    // 曲线
//    QLineSeries       *pTemperature;
//    QLineSeries       *pHumidity;

};

#endif // HUMITURECHARTS_H
