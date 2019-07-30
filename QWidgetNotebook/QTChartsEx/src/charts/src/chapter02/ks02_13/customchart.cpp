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
    for (QPointF p : points) {
        if (distanceOf(p, point) < distanceOf(m_ptNew, point)) {
            m_ptNew = p;
            m_clicked = true;
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

void CCustomChart::modifyCurve(const QPoint &point)
{
    if (m_clicked) {

        //Get the x- and y axis to be able to convert the mapped
        //coordinate point to the charts scale.
        QAbstractAxis *axisx = this->axisX();
        QValueAxis *haxis = 0;
        if (axisx->type() == QAbstractAxis::AxisTypeValue)
            haxis = qobject_cast<QValueAxis *>(axisx);

        QAbstractAxis *axisy = this->axisY();
        QValueAxis *vaxis = 0;
        if (axisy->type() == QAbstractAxis::AxisTypeValue)
            vaxis = qobject_cast<QValueAxis *>(axisy);

        if (haxis && vaxis) {
            //Calculate the "unit" between points on the x
            //y axis.
            double xUnit = this->plotArea().width() / (haxis->max()-haxis->min());
            double yUnit = this->plotArea().height() /(vaxis->max()-vaxis->min());
			
			//Map the point clicked from the ChartView
			//to the area occupied by the chart.
			QPoint ptMapped = point;
			ptMapped.setX(point.x() - this->plotArea().x());
			ptMapped.setY(point.y() - this->plotArea().y());

            // ������ӳ�䵽ͼ��
            double x = ptMapped.x() / xUnit;
            double y = vaxis->max() - ptMapped.y() / yUnit;
			
			QPointF ptLast = m_ptNew;
			getNearestPoint(QPointF(x, y));
            
			// ������������
			QPointF ptStart;
			QPointF ptEnd;
			if (ptLast.x() > m_ptNew.x())	{
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
			m_ptNew.setX(x);
			m_ptNew.setY(y);
        }
    }
}

