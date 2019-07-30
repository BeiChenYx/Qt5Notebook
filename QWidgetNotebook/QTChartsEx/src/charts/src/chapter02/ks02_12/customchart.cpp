/*!
* Copyright (C) 2018 Ů�����ϰ�
* ��Ȩ���С�
* ��������ڿγ̡�Qt Charts��������ߡ���ѧϰ�����𴫲���
* ��������:���벻��֤�ȶ��ԣ�����������ҵ��;���������Ը���

\file: customchart.cpp
\brief ʾ��cpp�ļ�

\author Ů�����ϰ�  �ǵ����: http://xingdianketang.cn/
\Date 2018/9
*/
#include "customchart.h"
#include <QtCharts/QValueAxis>
#include <QtCharts/QAbstractAxis>
#include <QtCore/QtMath>
static double c_Zero = 0.0001f;

CCustomChart::CCustomChart(QGraphicsItem *parent, Qt::WindowFlags wFlags, QLineSeries *series)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags), m_series(series)
{
    m_clicked = false;
}

CCustomChart::~CCustomChart()
{
}
void CCustomChart::setSelectedSerie(QLineSeries* pSerie)
{
	m_series = pSerie;
}
void CCustomChart::getNearestPoint(const QPointF& point)
{
	m_ptNew = QPoint();
    const auto points = m_series->points();
	if (m_clicked) {// �����޸�
		auto itePoint = points.begin();
		for (; itePoint != points.end(); itePoint++) {
			if ((itePoint + 1) == points.end()) {
				m_ptNew = *itePoint;
				m_ptNew.setY(point.y());
				break;
			}
			else if (fabs((*itePoint).x() - point.x()) < fabs((*(itePoint +1)).x() - point.x())) {
				m_ptNew = *itePoint;
				m_ptNew.setY(point.y());
				break;
			}
		}
	}
	else {
		for (QPointF p : points) {
			if (distanceOf(p, point) < distanceOf(m_ptNew, point)) {
				m_ptNew = p;
				m_clicked = true;
			}
		}
	}
}

void CCustomChart::clickOnPoint(const QPointF &point)
{
    // �õ������Ͼ������������ĵ�
    m_clicked = false;

	getNearestPoint(point);
}

qreal CCustomChart::distanceOf(const QPointF &p1, const QPointF &p2)
{
    return qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x())
                + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

void CCustomChart::setModifyFlag(bool clicked)
{
    m_clicked = clicked;
}

 bool CCustomChart::mapToSeries(const QPoint &ptInView, QPointF& ptInSeries)
{
    // ����ͼ����ϵ�ڵ�����ptInViewת����ͼ�����������ϵ
    QAbstractAxis *axisx = this->axisX();
    QValueAxis *haxis = 0;
    if (axisx->type() == QAbstractAxis::AxisTypeValue)
        haxis = qobject_cast<QValueAxis *>(axisx);

    QAbstractAxis *axisy = this->axisY();
    QValueAxis *vaxis = 0;
    if (axisy->type() == QAbstractAxis::AxisTypeValue)
        vaxis = qobject_cast<QValueAxis *>(axisy);

	if (haxis && vaxis) {
		// ������������ÿ������֮��ľ���
		double xUnit = this->plotArea().width() / (haxis->max() - haxis->min());
		double yUnit = this->plotArea().height() / (vaxis->max() - vaxis->min());

		// ��ChartView����ϵ�ڵ�ptInViewת����Chart����ϵ
		QPoint ptMapped = ptInView;
		ptMapped.setX(ptInView.x() - this->plotArea().x());
		ptMapped.setY(ptInView.y() - this->plotArea().y());

		// ������ӳ�䵽ͼ�����ߣ�
		double x = ptMapped.x() / xUnit;
		double y = vaxis->max() - ptMapped.y() / yUnit;
		ptInSeries = QPointF(x, y);
		return true;
	}
	return false;
}

void CCustomChart::modifyCurve(const QPoint &ptInView)
{
    if (m_clicked) {			
		QPointF ptLast = m_ptNew;
		QPointF ptInSeries;
		if (!mapToSeries(ptInView, ptInSeries))
			return;
		getNearestPoint(ptInSeries);
            
		// ��б�ʲ�ֵ�ķ�ʽ�����������ݣ�����������
		QPointF ptStart;
		QPointF ptEnd;
		if (ptLast.x() > m_ptNew.x()) {
			ptStart = m_ptNew;
			ptEnd = ptLast;
		}
		else {
			ptStart = ptLast;
			ptEnd = m_ptNew;
		}
		const auto points = m_series->points();
		qreal k = (ptEnd.y()-ptStart.y())/(ptEnd.x()-ptStart.x());
		qreal b = ptStart.y() - k * ptStart.x();

		for (QPointF p : points) {
			if ((p.x() >= ptStart.x()) && (p.x() <= ptEnd.x())) {
				m_series->replace(p, QPointF(p.x(), k*p.x()+b));
			}
		}

        // �����������
		m_ptNew = ptInSeries;
    }
}

