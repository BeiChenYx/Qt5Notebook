/*! 
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: dialog.cpp
\brief 示例实现文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
\Date 2018/9
*/
#include <qglobal.h>
#include "dialog.h"


CDialog::CDialog(QWidget* pParent) : QDialog(pParent)
{
	setupUi(this);
	
	
	// 构建系列-折线
	m_pSeries = new QLineSeries();
	// 为系列设置数据
	//for (quint32 i = 0; i < 100; i++) {
	//	m_pSeries->append(i, sin(0.6f*i));
	//}
	// 构建图表
	m_chart = new QChart();
	
	// 添加系列
	m_chart->addSeries(m_pSeries);
	//chart->legend()->hide();		// 隐藏图例

	m_chart->createDefaultAxes();	// 基于已添加到图表的 series 来创轴

									// 设置主题
	m_chart->setTheme(QChart::ChartThemeDark);
	
	// 将图表绑定到视图
	widget->setChart(m_chart);

	connect(btnRun, &QPushButton::clicked, this, &CDialog::onRun);
	connect(btnReset, &QPushButton::clicked, this, &CDialog::onReset);
}

CDialog::~CDialog()
{
	m_chart->removeAllSeries();
}

void CDialog::onReset()
{
	plainTextEdit->setPlainText("0,0.3 20,0.5 30,0.2 40,-0.6 50,0.6 60,0.4");
}
void CDialog::onRun()
{
	QVector<QPointF> points;
	QString str = plainTextEdit->toPlainText();
	QStringList strList = str.split(" ");
	QStringList::iterator iteStrList;
	QStringList strListPoint = str.split(" ");
	QPointF pt;
	qreal dMin = 0.f;
	qreal dMax = 0.f;
	for (iteStrList = strList.begin(); iteStrList != strList.end(); iteStrList++)
	{
		strListPoint = (*iteStrList).split(",");
		if (2 != strListPoint.size()) {
			continue;
		}
		pt.setX(strListPoint[0].toDouble());
		pt.setY(strListPoint[1].toDouble());
		if (dMin > pt.y()) {
			dMin = pt.y();
		}
		if (dMax < pt.y()) {
			dMax = pt.y();
		}
		points.push_back(pt);
	}
	m_pSeries->replace(points);
	QList<QAbstractAxis*> horizontalAxesList = m_chart->axes(Qt::Horizontal, m_pSeries);
	QList<QAbstractAxis*> verticalAxesList = m_chart->axes(Qt::Vertical, m_pSeries);
	QAbstractAxis* pAxisX = ((horizontalAxesList.size()>0) ? horizontalAxesList[0] : NULL);
	QAbstractAxis* pAxisY = ((verticalAxesList.size()>0) ? verticalAxesList[0] : NULL);
	if (NULL != pAxisX) {
		pAxisX->setRange(points[0].x(), points[points.size() - 1].x());
	}
	if (NULL != pAxisY) {
		pAxisY->setRange(dMin, dMax);
	}
}