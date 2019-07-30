/*!
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: tip.cpp
\brief 示例cpp文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
\Date 2018/9
*/

#include "tip.h"
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>
const int c_distince = 6;

CTip::CTip(QChart *chart):
    QGraphicsItem(chart),
    m_chart(chart)
{
}

QRectF CTip::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_pt));//m_pt:(400,2.5)
    QRectF rect;
    rect.setLeft(qMin(m_rctTip.left(), anchor.x())-2);
    rect.setRight(qMax(m_rctTip.right(), anchor.x())+2);
    rect.setTop(qMin(m_rctTip.top(), anchor.y())-2);
    rect.setBottom(qMax(m_rctTip.bottom(), anchor.y())+2);
    return rect;
}

void CTip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path;
  //  path.addRoundedRect(m_rect, c_distince, c_distince);
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_pt));
    if (!m_rect.contains(anchor)) {
        QPointF point1, point2;

        // 根据m_rect建立锚点坐标
        bool above = anchor.y() <= m_rect.top();
        bool aboveCenter = anchor.y() > m_rect.top() && anchor.y() <= m_rect.center().y();
        bool belowCenter = anchor.y() > m_rect.center().y() && anchor.y() <= m_rect.bottom();
        bool below = anchor.y() > m_rect.bottom();

        bool onLeft = anchor.x() <= m_rect.left();
        bool leftOfCenter = anchor.x() > m_rect.left() && anchor.x() <= m_rect.center().x();
        bool rightOfCenter = anchor.x() > m_rect.center().x() && anchor.x() <= m_rect.right();
        bool onRight = anchor.x() > m_rect.right();

        // 找到离m_rect最近的拐角.
        qreal x = (onRight + rightOfCenter) * m_rect.width();
        qreal y = (below + belowCenter) * m_rect.height();
        bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
        bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

        qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
        qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);;
        point1.setX(x1);
        point1.setY(y1);

        qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);;
        qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);;
        point2.setX(x2);
        point2.setY(y2);

        path.moveTo(point1);
        path.lineTo(anchor);
        path.lineTo(point2);
        path = path.simplified();
    }
	QPen pn(Qt::NoPen);
	painter->setPen(pn);
	QColor clr(Qt::white);
	//clr.setAlphaF(0.5);
    painter->setBrush(clr);
    painter->drawPath(path);
	painter->drawEllipse(m_rctTip);
	painter->setPen(Qt::black);
    painter->drawText(m_strRect, m_str);
}

void CTip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->setAccepted(true);
}

void CTip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        setPos(mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton)));
        event->setAccepted(true);
    } else {
        event->setAccepted(false);
    }
}

void CTip::setText(const QString &text)
{
	m_str = text;
    QFontMetrics metrics(m_font);
	m_strRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_str);
	m_strRect.translate(c_distince, c_distince);
    prepareGeometryChange();
    m_rect = m_strRect.adjusted(-c_distince, -c_distince, c_distince, c_distince);
	qreal a = 0.f;
	qreal b = 0.f;
	qreal c = m_rect.width() / 2;
	qreal h = m_rect.height();
	a = h / 4 + sqrt(h*h / 16 + c * c);
	b = sqrt(a*a - c * c);
	QRectF rctEllipse(m_rect.left() - (a - c), m_rect.top() - (b - h / 2), 2 * a, 2 * b);
	//path.addEllipse(rctEllipse);
	m_rctTip = rctEllipse;
}

void CTip::setAnchor(QPointF point)
{
	m_pt = point;
}

void CTip::updateGeometry()
{
    prepareGeometryChange();
    setPos(m_chart->mapToPosition(m_pt) + QPoint(10, -50));
}
