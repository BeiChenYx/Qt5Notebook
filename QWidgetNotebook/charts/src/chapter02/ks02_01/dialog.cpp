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
	QLineSeries *series = new QLineSeries();
	// 为系列设置数据
	for (quint32 i = 0; i < 100; i++) {
		series->append(i, sin(0.6f*i));
	}
	// 构建图表
	m_chart = new QChart();
	
	// 添加系列
	m_chart->addSeries(series);
	//chart->legend()->hide();		// 隐藏图例

	m_chart->createDefaultAxes();		// 基于已添加到图表的 series 来创建默认的坐标轴

									// 设置主题
	m_chart->setTheme(QChart::ChartThemeDark);
	
	// 将图表绑定到视图
	widget->setChart(m_chart);
}

CDialog::~CDialog()
{
	m_chart->removeAllSeries();
}

