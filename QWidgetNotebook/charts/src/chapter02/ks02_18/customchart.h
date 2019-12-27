
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
	/**
	* @brief 根据鼠标在视图中的坐标修改曲线。
	* @param[in] pointInView 鼠标在视图中的坐标
	* @retval void
	*/
	void modifyCurve(const QPoint &pointInView);
	void setModifyFlag(bool clicked);
	void setSelectedSerie(QLineSeries*);
private:
	/**
	* @brief 将视图坐标转换为曲线坐标
	* @param[in] pointInView 鼠标在视图中的坐标
	* @param[out] ptInSeries 鼠标在曲线坐标系内的坐标
	* @retval 转换成功
	*/
	bool mapToSeries(const QPoint &pointInView, QPointF& ptInSeries);

	qreal distanceOf(const QPointF &p1, const QPointF &p2);
	/**
	* @brief 根据坐标获取曲线上对应的坐标。
	* @param[in] point鼠标坐标，已经转换到图表中的曲线坐标系
	* @retval void
	*/
	void getNearestPoint(const QPointF& point);
private:
	QLineSeries * m_series;
	QPointF m_ptNew;

	bool m_clicked;
};

#endif // CHART_H
