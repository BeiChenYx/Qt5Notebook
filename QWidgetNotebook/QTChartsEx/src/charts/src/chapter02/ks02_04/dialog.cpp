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
#include <QRadioButton>
#include <QCheckBox>

#include "dialog.h"

bool g_bCardiogram = 1; // 1表示心电图，2表示动态曲线图

const int maxSize1 = 1000;
const int maxSize2 = 10;
CDialog::CDialog(QWidget* pParent) : QDialog(pParent),
m_series1(NULL),
m_series2(NULL),
m_x(7), // 通过调整m_x来调整cpu曲线头部的起始位置
m_y(1)
{
    setupUi(this);

    initialize();
}

CDialog::~CDialog()
{
	m_chart->removeAllSeries();
}
void CDialog::initialize()
{
	m_tmLast = 0;

	// 构建图表
    m_chart = new QChart;			

	// 构建X坐标
    m_axisX = new QValueAxis;
	if (g_bCardiogram)
	{
		m_series2 = new QLineSeries;	// 构建系列
		m_series2->append(m_x, m_y);
		m_chart->addSeries(m_series2);	// 添加系列
		m_axisX->setRange(0, maxSize2);	// 设置坐标范围
	}
	else {
		m_series1 = new QSplineSeries;	// 构建系列
		m_chart->addSeries(m_series1);	// 添加系列
		for(int i=0;i<maxSize1;++i){
			m_series1->append(i,0);
		}
		m_axisX->setRange(0, maxSize1);	// 设置坐标范围
	}
//	m_axisX->setLabelFormat("%g");	// 设置坐标文本的格式
	m_axisX->setLabelFormat("%.1f");	// 设置坐标文本的格式，不能用%g。
	m_axisX->setTickCount(5);		// 刻度数  

 	// 构建Y坐标
	m_axisY = new QValueAxis;
    m_axisY->setRange(-10, 10);	// 设置坐标范围
    m_axisY->setTitleText("axisY");	// 设置坐标的标题
	if (g_bCardiogram) {
		m_chart->setAxisX(m_axisX,m_series2); // 为图表设置X坐标
		m_chart->setAxisY(m_axisY,m_series2); // 为图表设置Y坐标
	}
	else {
		m_chart->setAxisX(m_axisX, m_series1); // 为图表设置X坐标
		m_chart->setAxisY(m_axisY, m_series1); // 为图表设置Y坐标
	}

    m_chart->setTheme(QChart::ChartThemeDark); // 设置主题
    chartView->setChart(m_chart);		// 将图表绑定到视图

    m_chart->legend()->hide();			// 隐藏图例

	// 绑定信号槽
    connect(cbshowLegend, &QCheckBox::clicked, this, &CDialog::on_cbshowLegend_clicked);

    connect(leTextX, &QLineEdit::editingFinished, this, &CDialog::on_leTextX_editingFinished);
    connect(leTextY, &QLineEdit::editingFinished, this, &CDialog::on_leTextY_editingFinished);
	
	connect(&m_timer, &QTimer::timeout, this, &CDialog::onTimeout);
	if (g_bCardiogram)
	{
		m_timer.setInterval(1000); // 1秒
	}
	else {
		m_timer.setInterval(100);  // 100毫秒
	}
	m_timer.start();
}

void CDialog::on_cbshowLegend_clicked(bool checked)
{
    if (checked){
        m_chart->legend()->show();
    }
    else {
        m_chart->legend()->hide();
    }
}

void CDialog::on_leTextX_editingFinished()
{
    QString str = leTextX->text();
    m_axisX->setTitleText(str);
}

void CDialog::on_leTextY_editingFinished()
{
    QString str = leTextY->text();
    m_axisY->setTitleText(str);
}

double CDialog::getData(double time)
{

    double s = qSin( time * M_PI * 2 )*7 ;
    return s;
}
qreal  CDialog::appendCardDiagramData()
{
	// 模拟每次追加不同个数的数据。
	int nCount = QRandomGenerator::global()->bounded(10, 15);
	qreal xOffset = m_x;
	for (int idx = 0; idx < nCount; idx++)	{
		m_x += 0.1;
		m_series2->append( m_x, QRandomGenerator::global()->bounded(-9, 9));
	}
	xOffset = m_x - xOffset;
	return xOffset;
}
void CDialog::onTimeout()
{
	if (g_bCardiogram)	{
		if (false) {
			// 此处只能采用模拟心电图数据，核心功能请关注scroll()调用。
			// 计算每一格的宽度
			//qreal x = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();
			//m_x += x;
			//int nDataCount = 0;
			//m_y = QRandomGenerator::global()->bounded(-8, 0);
			//m_series2->append(m_x-0.1f, m_y);
			//nDataCount++;
			//m_y = QRandomGenerator::global()->bounded(0, 8);
			//m_series2->append(m_x, m_y);
			//nDataCount++;
			//for (qreal dVal=0; dVal < x-0.1f; dVal+=0.1f)
			//{ 
			//	m_series2->append(m_x + dVal, QRandomGenerator::global()->bounded(0, 4)/5.f);
			//	nDataCount++;
			//}
			//qreal w = m_chart->plotArea().width();
			//qreal xOffset =  w / m_axisX->tickCount(); // 每次滚动一个格
			//m_chart->scroll(xOffset, 0);
		}
		qreal xOffset = appendCardDiagramData();
		qreal xUnit = m_chart->plotArea().width() / (m_axisX->max() - m_axisX->min());
		qreal xScroll = xOffset * xUnit;
		m_chart->scroll(xScroll, 0);
	}
	else	{
		
		static QTime tmBegin(QTime::currentTime());
		long int tmElapsed = tmBegin.elapsed();
		int nSkip = (tmElapsed - m_tmLast);// 数据个数
		qDebug() << "skip data count:" << nSkip;
		if (isVisible()) {
			QVector<QPointF> ptsOld = m_series1->pointsVector();
			QVector<QPointF> ptsNew;

			// 跳过一些数据。两次定时之间流失的时间作为跳过的数据个数。
			for (int i = nSkip; i<ptsOld.count(); ++i) {
				// 新数据的第0个坐标，存放的是 ptsOld 的第nSkip坐标处的数据。
				ptsNew.append(QPointF(i - nSkip, ptsOld.at(i).y()));// 跳过nSkip个数据
			}

			// 补充新数据：从跳过后的数据点开始，获取新数据。
			qint64 sizePoints = ptsOld.count() - nSkip;
			for (int k = 0; k<nSkip; ++k) {
				// 追加的第一个坐标：0+sizePoints=旧数据个数-跳过的个数=剩余数据起始点坐标
				ptsNew.append(QPointF(k + sizePoints, getData((((double)m_tmLast + k + 1) / maxSize1))));
			}
			m_series1->replace(ptsNew);
			m_tmLast = tmElapsed;
		}
	}
}

