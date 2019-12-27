/*!
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: chartview.h
\brief 示例头文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
\Date 2018/9
*/
#ifndef VIEW_H
#define VIEW_H
#include <QChartView>
//#include <QtCharts/QChartGlobal>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

class CTip;

QT_CHARTS_USE_NAMESPACE

class CChartView: public QChartView
{
    Q_OBJECT

public:
    CChartView(QWidget *parent = 0);

    void setSize(const QSize& sz);
protected:
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

public slots:
    void keepTip();
    void tooltip(const QPointF& point, bool state);

private:
    CTip *m_tooltip;
    QList<CTip *> m_tips;
};

#endif
