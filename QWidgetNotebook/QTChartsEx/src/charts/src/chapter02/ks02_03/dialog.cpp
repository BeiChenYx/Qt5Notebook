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

const int maxSize1 = 1000;
CDialog::CDialog(QWidget* pParent) : QDialog(pParent),
m_series1(NULL)
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
	// 构建系列-曲线
	m_series1 = new QSplineSeries;	
	for(int i=0;i<maxSize1;++i){
		m_series1->append(i, getData(i*1.f/ maxSize1));
	}
	
	// 构建图表
    m_chart = new QChart;			
	m_chart->addSeries(m_series1);	// 添加系列

	// 构建X坐标
    m_axisX = new QValueAxis;
	m_axisX->setRange(0, maxSize1);	// 设置坐标范围	  
	m_axisX->setLabelFormat("%g");	// 设置坐标文本的格式
	m_axisX->setTickCount(5);		// 刻度数

 	// 构建Y坐标
	m_axisY = new QValueAxis;
    m_axisY->setRange(-10, 10);		// 设置坐标范围
    m_axisY->setTitleText("axisY");	// 设置坐标的标题

	// 为图表设置坐标
	m_chart->setAxisX(m_axisX, m_series1); 
	m_chart->setAxisY(m_axisY, m_series1); 
	
	// 设置主题
    m_chart->setTheme(QChart::ChartThemeDark); 
	
	// 隐藏图例
    m_chart->legend()->hide();		
	
	// 将图表绑定到视图
    chartView->setChart(m_chart);		

    connect(cbshowLegend, &QCheckBox::clicked, this, &CDialog::on_cbshowLegend_clicked);
    connect(cbAntialiasing, &QCheckBox::clicked, this, &CDialog::on_cbAntialiasing_clicked);

    connect(leTextX, &QLineEdit::editingFinished, this, &CDialog::on_leTextX_editingFinished);
    connect(leTextY, &QLineEdit::editingFinished, this, &CDialog::on_leTextY_editingFinished);
	
	connect(&m_timer, &QTimer::timeout, this, &CDialog::onTimeout);
	m_timer.setInterval(100); // 毫秒
	m_timer.start();
}
void CDialog::on_cbAntialiasing_clicked(bool checked)
{
	chartView->setRenderHint(QPainter::Antialiasing, checked);
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

void CDialog::onTimeout()
{
	static QTime tmBegin(QTime::currentTime());
	qint64 tmElapsed = tmBegin.elapsed();	// 自从构造tmBegin对象开始经过的毫秒数。
	int nSkip = (tmElapsed - m_tmLast);		// 跳过的数据个数
	qDebug() << "data count skipped:" << nSkip;
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
		double dt = 0.f;
		for (int k = 0; k < nSkip; ++k) {
			// 追加的第一个坐标：0+sizePoints=旧数据个数-跳过的个数=剩余数据起始点坐标
			dt = ((double)m_tmLast + k + 1) / maxSize1;
			ptsNew.append(QPointF(k + sizePoints, getData(dt)));
		}
		m_series1->replace(ptsNew);
		m_tmLast = tmElapsed;
	}
}

