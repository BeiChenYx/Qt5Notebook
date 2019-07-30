
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
	* @brief �����������ͼ�е������޸����ߡ�
	* @param[in] pointInView �������ͼ�е�����
	* @retval void
	*/
	void modifyCurve(const QPoint &pointInView);
	void setModifyFlag(bool clicked);
	void setSelectedSerie(QLineSeries*);
private:
	/**
	* @brief ����ͼ����ת��Ϊ��������
	* @param[in] pointInView �������ͼ�е�����
	* @param[out] ptInSeries �������������ϵ�ڵ�����
	* @retval ת���ɹ�
	*/
	bool mapToSeries(const QPoint &pointInView, QPointF& ptInSeries);

	qreal distanceOf(const QPointF &p1, const QPointF &p2);
	/**
	* @brief ���������ȡ�����϶�Ӧ�����ꡣ
	* @param[in] point������꣬�Ѿ�ת����ͼ���е���������ϵ
	* @retval void
	*/
	void getNearestPoint(const QPointF& point);
private:
	QLineSeries * m_series;
	QPointF m_ptNew;

	bool m_clicked;
};

#endif // CHART_H
