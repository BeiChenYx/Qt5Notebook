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
#include "customchart.h"
#include "chartview.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>
#include "tip.h"

CChartView::CChartView(CCustomChart* pChart, QWidget *parent)
    : QChartView(parent),
	  m_chart(pChart),
      m_tooltip(0)
{
    setDragMode(QChartView::NoDrag);
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
	//setSize(parentWidget()->geometry().size());
	QChartView::resizeEvent(event);
}
void CChartView::mousePressEvent(QMouseEvent *event)
{
	m_ptMousePress = event->pos();
	QChartView::mousePressEvent(event);
}

void CChartView::mouseMoveEvent(QMouseEvent *event)
{
	m_chart->modifyCurve(event->pos());
	QChartView::mouseMoveEvent(event);
}

void CChartView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->pos() != m_ptMousePress) {
		m_chart->modifyCurve(event->pos());
	}
	m_chart->setModifyFlag(false);
	QChartView::mouseReleaseEvent(event);
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
