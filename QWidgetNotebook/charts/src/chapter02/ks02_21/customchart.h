
#ifndef CUSTOMCHART_H
#define CUSTOMCHART_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE
QT_CHARTS_BEGIN_NAMESPACE
class QPieSlice;
QT_CHARTS_END_NAMESPACE

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
	/**
	* @brief 设置修改标志
	* @param[in] clicked true:曲线已修改 ,false:曲线未修改
	* @retval void
	*/
	void setModifyFlag(bool clicked);
	/**
	* @brief 设置当前选中系列
	* @param[in] pSeries 当前选中系列
	* @retval void
	*/
	void setSelectedSerie(QLineSeries* pSeries);

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
class CDrillDownChart : public QChart
{
	Q_OBJECT
public:
	explicit CDrillDownChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
	~CDrillDownChart();

public:
	/**
	* @brief 更换系列
	* @param[in] pSeries 新的系列
	* @retval void
	*/
	void changeSeries(QAbstractSeries *pSeries);

public slots:

	void handleSliceClicked(QPieSlice * slice);

private:
	QAbstractSeries *m_currentSeries;
};

#endif // CHART_H
