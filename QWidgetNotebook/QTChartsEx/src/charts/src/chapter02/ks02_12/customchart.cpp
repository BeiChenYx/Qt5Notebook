/*!
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: customchart.cpp
\brief 示例cpp文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
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
	if (m_clicked) {// 正在修改
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
    // 得到曲线上距离鼠标点击最近的点
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
    // 把视图坐标系内的坐标ptInView转换到图表的曲线坐标系
    QAbstractAxis *axisx = this->axisX();
    QValueAxis *haxis = 0;
    if (axisx->type() == QAbstractAxis::AxisTypeValue)
        haxis = qobject_cast<QValueAxis *>(axisx);

    QAbstractAxis *axisy = this->axisY();
    QValueAxis *vaxis = 0;
    if (axisy->type() == QAbstractAxis::AxisTypeValue)
        vaxis = qobject_cast<QValueAxis *>(axisy);

	if (haxis && vaxis) {
		// 计算坐标轴上每两个点之间的距离
		double xUnit = this->plotArea().width() / (haxis->max() - haxis->min());
		double yUnit = this->plotArea().height() / (vaxis->max() - vaxis->min());

		// 将ChartView坐标系内的ptInView转换到Chart坐标系
		QPoint ptMapped = ptInView;
		ptMapped.setX(ptInView.x() - this->plotArea().x());
		ptMapped.setY(ptInView.y() - this->plotArea().y());

		// 把坐标映射到图表（曲线）
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
            
		// 用斜率插值的方式计算曲线数据，并更新曲线
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

        // 更新鼠标坐标
		m_ptNew = ptInSeries;
    }
}

