
#ifndef CUSTOMCHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class CCustomChart : public QChart
{
    Q_OBJECT
public:
    explicit CCustomChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0, QLineSeries *series = 0);
    ~CCustomChart();

public slots:
    void clickOnPoint(const QPointF &point);

public:
    void modifyCurve(const QPoint &point);
    void setModifyFlag(bool clicked);
	void setSelectedSerie(QLineSeries*);
private:
    qreal distanceOf(const QPointF &p1, const QPointF &p2);
	void getNearestPoint(const QPointF& point);
private:
    QLineSeries *m_series;
    QPointF m_ptNew;

    bool m_clicked;
};

#endif // CHART_H
