/*!
* Copyright (C) 2018 Ů�����ϰ�
* ��Ȩ���С�
* ��������ڿγ̡�Qt Charts��������ߡ���ѧϰ�����𴫲���
* ��������:���벻��֤�ȶ��ԣ�����������ҵ��;���������Ը���

\file: chartview.cpp
\brief ʾ��cpp�ļ�

\author Ů�����ϰ�  �ǵ����: http://xingdianketang.cn/
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
	setSize(parentWidget()->geometry().size());
	QChartView::resizeEvent(event);
}


void CChartView::mouseMoveEvent(QMouseEvent *event)
{
	QChartView::mouseMoveEvent(event);
}

void CChartView::keepTip()
{
	m_tips.append(m_tooltip);
    m_tooltip = new CTip(chart());
	m_tooltip->hide();
}

void CChartView::tooltip(const QPointF& point, bool state)
{
    if (NULL == m_tooltip )
        m_tooltip = new CTip(chart());

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(1);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}
