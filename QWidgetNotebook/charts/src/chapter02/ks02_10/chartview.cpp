/*!
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: chartview.cpp
\brief 示例cpp文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
\Date 2018/9
*/

#include "chartview.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>
#include "tip.h"

CChartView::CChartView(QWidget *parent)
    : QChartView(parent),
      m_tooltip(0)
{
	setDragMode(QChartView::ScrollHandDrag);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setMouseTracking(true);
}
void CChartView::setSize(const QSize& sz) {
	if (scene() && chart()) {
		resize(sz);
		scene()->setSceneRect(QRect(QPoint(0, 0), sz));
		chart()->resize(sz);
		const auto tips = m_tips;
		for (CTip *tip : tips)
			tip->updateGeometry();
		viewport()->update();
	}
}
void CChartView::resizeEvent(QResizeEvent *event) {
	setSize(parentWidget()->geometry().size());
	QChartView::resizeEvent(event);
}

void CChartView::mousePressEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton) 	{
		m_ptLast = evt->globalPos();
	}
	QChartView::mousePressEvent(evt);
}
void CChartView::mouseMoveEvent(QMouseEvent *event)
{
	Qt::MouseButton btn = event->button();
	if ((btn == Qt::LeftButton) || (event->buttons()&Qt::LeftButton))
	{
		if (NULL != chart()) {
			QPoint pt = m_ptLast - event->globalPos();
			QPointF ptScene = mapToScene(pt);
			QPointF ptItem = chart()->mapFromScene(ptScene);
			qreal dx = ptItem.x();
			qreal dy = ptItem.y();
			chart()->scroll(dx, -dy);
			m_ptLast = event->globalPos();
		}
	}
	QChartView::mouseMoveEvent(event);
}

void CChartView::keepTip()
{
	m_tips.append(m_tooltip);
    m_tooltip = new CTip(chart());
	m_tooltip->hide();
}

void CChartView::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new CTip(chart());

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}
