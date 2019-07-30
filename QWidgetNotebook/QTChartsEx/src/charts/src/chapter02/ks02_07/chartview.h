/*!
* Copyright (C) 2018 Ů�����ϰ�
* ��Ȩ���С�
* ��������ڿγ̡�Qt Charts��������ߡ���ѧϰ�����𴫲���
* ��������:���벻��֤�ȶ��ԣ�����������ҵ��;���������Ը���

\file: chartview.h
\brief ʾ��ͷ�ļ�

\author Ů�����ϰ�  �ǵ����: http://xingdianketang.cn/
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
