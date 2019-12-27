
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
	* @brief �����������ͼ�е������޸����ߡ�
	* @param[in] pointInView �������ͼ�е�����
	* @retval void
	*/
	void modifyCurve(const QPoint &pointInView);
	/**
	* @brief �����޸ı�־
	* @param[in] clicked true:�������޸� ,false:����δ�޸�
	* @retval void
	*/
	void setModifyFlag(bool clicked);
	/**
	* @brief ���õ�ǰѡ��ϵ��
	* @param[in] pSeries ��ǰѡ��ϵ��
	* @retval void
	*/
	void setSelectedSerie(QLineSeries* pSeries);

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
class CDrillDownChart : public QChart
{
	Q_OBJECT
public:
	explicit CDrillDownChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
	~CDrillDownChart();

public:
	/**
	* @brief ����ϵ��
	* @param[in] pSeries �µ�ϵ��
	* @retval void
	*/
	void changeSeries(QAbstractSeries *pSeries);

public slots:

	void handleSliceClicked(QPieSlice * slice);

private:
	QAbstractSeries *m_currentSeries;
};

#endif // CHART_H
