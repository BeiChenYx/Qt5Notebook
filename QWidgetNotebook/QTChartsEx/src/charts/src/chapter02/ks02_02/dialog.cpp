/*! 
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: dialog.h
\brief 示例实现文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
\Date 2018/9
*/
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qglobal.h>
#include "dialog.h"


CDialog::CDialog(QWidget* pParent) : QDialog(pParent), m_pSeries(NULL),m_pChart(NULL), m_pScene(NULL)
{
	setupUi(this);
	
	initialize();

}

CDialog::~CDialog()
{
	m_pChart->removeAllSeries();
}
void CDialog::initialize()
{
	// 构建系列-折线
	m_pSeries = new QLineSeries();			
	for (quint32 i = 0; i < 100; i++) {
		m_pSeries->append(i, sin(0.6f*i)*100);
	}
	
	// 反走样-抗锯齿
	m_pGraphicsView->setRenderHint(QPainter::Antialiasing);
														   
	// 构建图表
	m_pChart = new QChart();					
	
	// 设置图元尺寸
	m_pChart->setGeometry(10, 10, 300, 260);	

	// 添加系列
	m_pChart->addSeries(m_pSeries);				

	// 隐藏图例
	m_pChart->legend()->hide();			

	// 基于已添加到图表的 series 来创轴
	m_pChart->createDefaultAxes();				

	// 设置主题
	m_pChart->setTheme(QChart::ChartThemeDark); 

	// 构建场景
	m_pScene = new QGraphicsScene();
	m_pScene->setBackgroundBrush(QBrush(QColor(245, 245, 245))); // 设置背景色
	
	// 为视图设置场景
	m_pGraphicsView->setScene(m_pScene);
	
	// 将图元添加到场景
	m_pScene->addItem(m_pChart);	
}