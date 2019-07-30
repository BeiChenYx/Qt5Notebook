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
#include <QGridLayout>
#include <QGraphicsItem>
#include <QTableWidget>
#include <QtCharts/QPieSeries>

#include "customchart.h"
#include "chartview.h"
#include "dialog.h"
#include "customchart.h"
#include "drilldownslice.h"
#include "tablemodel.h"

const int maxSize1 = 100;
CDialog::CDialog(QWidget* pParent) : QDialog(pParent),
m_series1(NULL),
m_series2(NULL)
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
	// 初始化各个配置参数控件
	initializeControls();

	m_tmLast = 0;

	// 初始化曲线
	initializeCurveSeries();
	
	// 初始化棒图
	initializeBarSeries();
	
	// 初始化饼图
	initializePieSeries();

	tabWidget->setCurrentIndex(0);
	m_chart = m_curveChart;
	m_pChartView = m_pCurveChartView;

	// 绑定信号槽
	connectSignalSlots();

	updateUI();

}
void CDialog::initializeCurveSeries()
{

	// 构建曲线图表
	m_curveChart = new CCustomChart;
	m_curveChart->setTitle("Curve Chart");
	// curve table
	m_curveTableView = new QTableView(tab_curve);
	m_curveModel = new CTableModel;
	m_curveTableView->setModel(m_curveModel);

	m_pCurveChartView = new CChartView(m_curveChart, tab_curve);
	m_pCurveChartView->setObjectName(QStringLiteral("CurveChartView"));

	// 构建系列1
	m_series1 = new QSplineSeries;
	m_series1->setName("series_1");
	sbPenWidth_Serie1->setValue(3);
	//for(int i=0;i<maxSize1;++i){
	//	m_series1->append(i, getData(((double)i) / maxSize1));
	//}
	QVXYModelMapper *mapper = new QVXYModelMapper(this);
	mapper->setXColumn(0);
	mapper->setYColumn(1);
	mapper->setSeries(m_series1);
	mapper->setModel(m_curveModel);
	QString seriesColorHex = "#000000";
	m_curveChart->addSeries(m_series1);	// 添加系列,该调用要在setModel()之后。
										// 用系列的颜色来显示模型中所映射的数据区域
	seriesColorHex = "#" + QString::number(m_series1->pen().color().rgb(), 16).right(6).toUpper();
	m_curveModel->addMapping(seriesColorHex, QRect(0, 0, 2, m_curveModel->rowCount()));

	// 构建系列2
	m_series2 = new QSplineSeries;
	m_series2->setName("series_2");
	sbPenWidth_Serie2->setValue(3);
	//for (int i = 0; i<maxSize1; ++i) {
	//	m_series2->append(i, getData(((double)i+30) / maxSize1));
	mapper = new QVXYModelMapper(this);
	mapper->setXColumn(2);
	mapper->setYColumn(3);
	mapper->setSeries(m_series2);
	mapper->setModel(m_curveModel);
	m_curveChart->addSeries(m_series2);	// 添加系列,该调用要在setModel()之后。
	seriesColorHex = "#" + QString::number(m_series2->pen().color().rgb(), 16).right(6).toUpper();
	m_curveModel->addMapping(seriesColorHex, QRect(2, 0, 2, m_curveModel->rowCount()));

	// 构建X坐标
	m_axisX = new QValueAxis;
	m_axisX->setRange(0, m_series1->points()[m_curveModel->rowCount() - 1].x());	// 设置坐标范围
	m_axisX->setLabelFormat("%g");	// 设置坐标文本的格式
	m_axisX->setTickCount(5);		// 刻度数
	m_axisX->setGridLineVisible(false);

	// 构建Y坐标
	m_axisY = new QValueAxis;
	m_axisY->setRange(0, 100);		// 设置坐标范围
	m_axisY->setTitleText("axisY");	// 设置坐标的标题
	m_curveChart->setAxisX(m_axisX, m_series1); // 为图表设置X坐标,并将坐标与系列1建立关联
										   //m_curveChart->setAxisX(m_axisX, m_series2); // 为图表设置X坐标,并将坐标与系列2建立关联
	m_curveChart->setAxisY(m_axisY, m_series1); // 为图表设置Y坐标,并将坐标与系列1建立关联
										   //m_curveChart->setAxisY(m_axisY, m_series2); // 为图表设置Y坐标,并将坐标与系列2建立关联
	m_axisY->setGridLineVisible(false);

	m_curveChart->setTheme(QChart::ChartThemeDark); // 设置主题
	m_pCurveChartView->setChart(m_curveChart);		// 将图表绑定到视图

													// 双坐标轴
	m_axisY3 = new QCategoryAxis;
	m_axisY3->append("Low", 30);
	m_axisY3->append("Medium", 50);
	m_axisY3->append("High", 90);
	m_axisY3->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

	m_curveChart->addAxis(m_axisY3, Qt::AlignRight);
	m_series2->attachAxis(m_axisX);
	m_series2->attachAxis(m_axisY3);

	// 隐藏图例
	m_curveChart->legend()->hide();
	QGridLayout* gridLaytoutBar = new QGridLayout(tab_curve);
	gridLaytoutBar->addWidget(m_curveTableView, 1, 0);
	gridLaytoutBar->addWidget(m_pCurveChartView, 1, 1);
	gridLaytoutBar->setColumnStretch(0, 0);
	gridLaytoutBar->setColumnStretch(1, 1);
}

void CDialog::initializeBarSeries()
{
	// 构建棒图图表
	m_barChart = new QChart;
	m_barChart->setTitle("Bar Chart");
	// bar table
	m_barTableView = new QTableView(widget);
	m_barModel = new CTableModel;
	m_barTableView->setModel(m_barModel);

	m_pBarChartView = new QChartView(m_barChart, widget);
	m_pBarChartView->setObjectName(QStringLiteral("CurveChartView"));

	m_seriesBar = new QBarSeries;

	int first = 1;
	int count = 10;
	m_barModelMapper = new QVBarModelMapper(this);
	m_barModelMapper->setFirstBarSetColumn(0);
	m_barModelMapper->setLastBarSetColumn(3);
	m_barModelMapper->setFirstRow(first);
	m_barModelMapper->setRowCount(count);
	m_barModelMapper->setSeries(m_seriesBar);
	m_barModelMapper->setModel(m_barModel);
	m_barChart->addSeries(m_seriesBar);
	m_pBarChartView->setChart(m_barChart);

	QGridLayout* gridLaytoutBar = new QGridLayout(widget);
	gridLaytoutBar->addWidget(m_barTableView, 0, 0);
	gridLaytoutBar->addWidget(m_pBarChartView,0, 1);
	gridLaytoutBar->setColumnStretch(0, 0);
	gridLaytoutBar->setColumnStretch(1, 1);
}


void CDialog::initializePieSeries()
{
	// 构建饼图图表
	m_pieChart = new CDrillDownChart;
	m_pieChart->setTitle("DilldDown Pie Chart");

	// 构建饼图的视图对象
	m_pPieChartView = new QChartView(m_pieChart, widgetChart);
	m_pPieChartView->setObjectName(QStringLiteral("PieChartView"));

	// 构建饼图系列
	m_seriesPie = new QPieSeries;

	// 将饼图添加到图表
	//m_pieChart->addSeries(m_seriesPie);

	// 构建钻进时展示的棒图系列
	QBarSeries* seriesBar0 = new QBarSeries;
	QBarSeries* seriesBar1 = new QBarSeries;
	QBarSeries* seriesBar2 = new QBarSeries;
	QBarSeries* seriesBar3 = new QBarSeries;
	QBarSeries* seriesBar4 = new QBarSeries;
	
	// 每个棒图对应的棒
	set0 = new QBarSet("Jane");
	set1 = new QBarSet("Joe");
	set2 = new QBarSet("Andy");
	set3 = new QBarSet("Barbara");
	set4 = new QBarSet("Axel");

	*set0 << 1 << 2 << 3 << 4 << 5 << 6;
	*set1 << 5 << 0 << 0 << 4 << 0 << 7;
	*set2 << 3 << 5 << 8 << 13 << 8 << 5;
	*set3 << 5 << 6 << 7 << 3 << 4 << 5;
	*set4 << 9 << 7 << 5 << 3 << 1 << 2;
	// 将棒添加到各个棒图系列
	seriesBar0->append(set0);
	seriesBar1->append(set1);
	seriesBar2->append(set2);
	seriesBar3->append(set3);
	seriesBar4->append(set4);
	
	// 构建饼图各个切块对象，并将钻进时展示的棒图对象传入切块
	*m_seriesPie << new CDrilldownSlice("Jane", 1, seriesBar1);
	*m_seriesPie << new CDrilldownSlice("Joe", 2, seriesBar0);
	*m_seriesPie << new CDrilldownSlice("Andy", 3, seriesBar2);
	*m_seriesPie << new CDrilldownSlice("Barbara", 4, seriesBar3);
	*m_seriesPie << new CDrilldownSlice("Axel", 5, seriesBar4);

	m_pieChart->changeSeries(m_seriesPie);

	// 将饼图切块的鼠标单击信号绑定到槽函数
	connect(m_seriesPie, &QPieSeries::clicked, 
		m_pieChart, &CDrillDownChart::handleSliceClicked);

	// 单击返回按钮时，重新显示饼图。
	connect(btnReturn, &QPushButton::clicked, this, &CDialog::resetPieChart);

	QPieSlice *slice = m_seriesPie->slices().at(1);
	//slice->setExploded();
	slice->setLabelVisible();
	slice->setLabelColor(Qt::white);
	slice->setPen(QPen(Qt::darkGreen, 2));
	slice->setBrush(Qt::green);

	QGridLayout* gridLaytoutPie = new QGridLayout(widgetChart);
	gridLaytoutPie->addWidget(m_pPieChartView, 0, 0);
	gridLaytoutPie->setColumnStretch(0, 1);

	qDebug() <<"slice0:"<< m_seriesPie->slices().at(0)->brush().color().rgba();
	qDebug() <<"slice1:"<< m_seriesPie->slices().at(1)->brush().color().rgba();
	qDebug() <<"slice2:"<< m_seriesPie->slices().at(2)->brush().color().rgba();
	qDebug() <<"slice3:"<< m_seriesPie->slices().at(3)->brush().color().rgba();
}
void CDialog::resetPieChart() {
	m_pieChart->changeSeries(m_seriesPie);
}

void CDialog::initializeControls() {
	populateThemeBox();
	populateAnimationBox();
	populateLegendBox();

	QStringList strLst;
	strLst << "8" << "12" << "16" << "20";
	// 标题-字号
	cbFontSize_Title->insertItems(0, strLst);
	// 将下拉列表当前展示的字号设置为最后一个字号，也就是20号。
	cbFontSize_Title->setCurrentIndex(cbFontSize_Title->count() - 1);

	// X轴坐标-字号
	cbLabelFontSize_axisX->insertItems(0, strLst);
	// Y轴坐标-字号
	cbLabelFontSize_axisY->insertItems(0, strLst);

	// 标题-颜色
	strLst.clear();
	cbBrush_Title->addItem("red", (int)Qt::red);
	cbBrush_Title->addItem("green", (int)Qt::green);
	cbBrush_Title->addItem("black", (int)Qt::black);

	cbPenColor_Serie1->addItem("red", (int)Qt::red);
	cbPenColor_Serie1->addItem("green", (int)Qt::green);
	cbPenColor_Serie1->addItem("black", (int)Qt::black);

	cbPenColor_Serie2->addItem("red", (int)Qt::red);
	cbPenColor_Serie2->addItem("green", (int)Qt::green);
	cbPenColor_Serie2->addItem("black", (int)Qt::black);
	cbPenColor_Serie2->setCurrentIndex(1);

	antialiasCheckBox->setChecked(true);

	// add items to theme combobox
	cbZoom->addItem("NoRubberBand", QChartView::NoRubberBand);
	cbZoom->addItem("VerticalRubberBand", QChartView::VerticalRubberBand);
	cbZoom->addItem("HorizontalRubberBand", QChartView::HorizontalRubberBand);
	cbZoom->addItem("HorizontalRubberBand", QChartView::RectangleRubberBand);
}

void CDialog::connectSignalSlots() {
	// 主题
	connect(themeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// 动画
	connect(animatedComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// 图例
	connect(legendComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// 反走样
	connect(antialiasCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateUI()));
	
	// 标题-字体
	connect(ckLegendInPlotArea, SIGNAL(clicked()), this, SLOT(updateUI()));

	// 标题-字体
	connect(fontComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// 标题-字号
	connect(cbFontSize_Title, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// 标题-画刷
	connect(cbBrush_Title, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	
	// 图表-画刷1
	connect(rbChartBrush_1, SIGNAL(clicked()), this, SLOT(updateUI()));
	// 图表-画刷2
	connect(rbChartBrush_2, SIGNAL(clicked()), this, SLOT(updateUI()));
	
	// 曲线区域-画刷1
	connect(rbPlotAreaBrush_1, SIGNAL(clicked()), this, SLOT(updateUI()));
	// 曲线区域-画刷2
	connect(rbPlotAreaBrush_2, SIGNAL(clicked()), this, SLOT(updateUI()));
	
	// X轴-title
    connect(leTextX, &QLineEdit::editingFinished, this, &CDialog::on_leTextX_editingFinished);
	// X轴-是否显示网格
	connect(cbGridLineVisibleX, SIGNAL(clicked()), this, SLOT(updateUI()));
	// X轴-坐标文本的字号
	connect(cbLabelFontSize_axisX, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// Y轴-title
    connect(leTextY, &QLineEdit::editingFinished, this, &CDialog::on_leTextY_editingFinished);
	// Y轴-是否显示网格
	connect(cbGridLineVisibleY, SIGNAL(clicked()), this, SLOT(updateUI()));
	// Y轴-坐标文本的字号
	connect(cbLabelFontSize_axisY, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));

	// 系列1-线色
  	connect(cbPenColor_Serie1, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// 系列1-线宽
  	connect(sbPenWidth_Serie1, SIGNAL(valueChanged(int)), this, SLOT(updateUI()));
	// 系列2-线色
  	connect(cbPenColor_Serie2, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	// 系列2-线宽
  	connect(sbPenWidth_Serie2, SIGNAL(valueChanged(int)), this, SLOT(updateUI()));
	
	//connect(&m_timer, &QTimer::timeout, this, &CDialog::onTimeout);
 //	m_timer.setInterval(100);  // 100毫秒
	//m_timer.start();
	connect(m_series1, &QSplineSeries::hovered, this, &CDialog::serie1Hovered);
	connect(m_series2, &QSplineSeries::hovered, this, &CDialog::serie2Hovered);

	connect(m_series1, &QSplineSeries::pressed, this, &CDialog::serie1Pressed);
	connect(m_series2, &QSplineSeries::pressed, this, &CDialog::serie2Pressed);
	connect(m_series1, &QSplineSeries::pressed, m_curveChart, &CCustomChart::clickOnPoint);
	connect(m_series2, &QSplineSeries::pressed, m_curveChart, &CCustomChart::clickOnPoint);

	if (true) // 人工调整曲线时，暂时禁用tip功能
	{
		connect(m_series1, &QSplineSeries::clicked, m_pCurveChartView, &CChartView::keepTip);
		connect(m_series1, &QSplineSeries::hovered, m_pCurveChartView, &CChartView::tooltip);

		connect(m_series2, &QSplineSeries::clicked, m_pCurveChartView, &CChartView::keepTip);
		connect(m_series2, &QSplineSeries::hovered, m_pCurveChartView, &CChartView::tooltip);
	}

	connectMarkers();
	// zoom
  	connect(cbZoom, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
	
	connect(addSetButton, &QPushButton::clicked, this, &CDialog::addBarset);
	connect(removeBarsetButton, &QPushButton::clicked, this, &CDialog::removeBarset);
	
	connect(tabWidget, &QTabWidget::currentChanged, this, &CDialog::tabChanged);

}
void CDialog::tabChanged()
{
	int idx = tabWidget->currentIndex();
	switch (idx)
	{
	case 0:
		m_chart = m_curveChart;
		m_pChartView = m_pCurveChartView;
		break;
	case 1:
		m_chart = m_barChart;
		m_pChartView = m_pBarChartView;
		break;
	default:
		m_chart = m_pieChart;
		m_pChartView = m_pPieChartView;
		break;
	}
	disconnectMarkers();
	connectMarkers();
	
	updateUI();

}
void CDialog::addBarset()
{
	m_barModel->insertColumns(m_barModel->columnCount()-1, 1);
	disconnectMarkers();
	connectMarkers();

	m_barModelMapper->setFirstBarSetColumn(0);
	m_barModelMapper->setLastBarSetColumn(m_barModel->columnCount()-1);

}

void CDialog::removeBarset()
{
	m_barModel->removeColumns(0, 1);
	disconnectMarkers();
	connectMarkers();

	m_barModelMapper->setFirstBarSetColumn(0);
	m_barModelMapper->setLastBarSetColumn(m_barModel->columnCount() - 1);
}
void CDialog::serie1Pressed(const QPointF& /*pt*/)
{
	m_curveChart->setSelectedSerie(m_series1);
}

void CDialog::serie2Pressed(const QPointF& /*pt*/)
{
	m_curveChart->setSelectedSerie(m_series2);
}

void CDialog::serie1Hovered(const QPointF& /*pt*/, bool bState)
{
	QPen penHighLight(Qt::white, 5);
	if (bState) {
		m_series1->setPen(penHighLight);
	}
	else {
		QVariant varColor = cbPenColor_Serie1->itemData(cbPenColor_Serie1->currentIndex());
		Qt::GlobalColor clr = static_cast<Qt::GlobalColor>(varColor.toInt());
		QPen pn2(clr, sbPenWidth_Serie1->value());
		m_series1->setPen(pn2);
	}
}

void CDialog::serie2Hovered(const QPointF& /*pt*/, bool bState)
{
	QPen penHighLight(Qt::white, 5);
	if (bState) {
		m_series2->setPen(penHighLight);
	}
	else{
		Qt::GlobalColor clr = static_cast<Qt::GlobalColor>(cbPenColor_Serie2->itemData(cbPenColor_Serie2->currentIndex()).toInt());
		QPen pn2(clr, sbPenWidth_Serie2->value());
		m_series2->setPen(pn2);
	}
}
void CDialog::setDialogPalette() {
	QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
		themeComboBox->itemData(themeComboBox->currentIndex()).toInt());

	// Set palette colors based on selected theme
	QPalette pal = window()->palette();
	if (theme == QChart::ChartThemeLight) {
		pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else if (theme == QChart::ChartThemeDark) {
		pal.setColor(QPalette::Window, QRgb(0x121218));
		pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
	}
	else if (theme == QChart::ChartThemeBlueCerulean) {
		pal.setColor(QPalette::Window, QRgb(0x40434a));
		pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
	}
	else if (theme == QChart::ChartThemeBrownSand) {
		pal.setColor(QPalette::Window, QRgb(0x9e8965));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else if (theme == QChart::ChartThemeBlueNcs) {
		pal.setColor(QPalette::Window, QRgb(0x018bba));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else if (theme == QChart::ChartThemeHighContrast) {
		pal.setColor(QPalette::Window, QRgb(0xffab03));
		pal.setColor(QPalette::WindowText, QRgb(0x181818));
	}
	else if (theme == QChart::ChartThemeBlueIcy) {
		pal.setColor(QPalette::Window, QRgb(0xcee7f0));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else {
		pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	window()->setPalette(pal);
}


void CDialog::updateUI()
{
	// 设置主题
	QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
		themeComboBox->itemData(themeComboBox->currentIndex()).toInt());
	m_chart->setTheme(theme);

	// 根据主题设置整个窗体的调色板
	setDialogPalette();
	

	// Update antialiasing
	bool checked = antialiasCheckBox->isChecked();
	m_pChartView->setRenderHint(QPainter::Antialiasing, checked);

	// Update animation options
	QChart::AnimationOptions options(
		animatedComboBox->itemData(animatedComboBox->currentIndex()).toInt());
	if (m_chart->animationOptions() != options) {
		m_chart->setAnimationOptions(options);
	}

	// Update legend alignment
	Qt::Alignment alignment(legendComboBox->itemData(legendComboBox->currentIndex()).toInt());

	QLegend *legend = m_chart->legend();
	if (!alignment) {
		if (ckLegendInPlotArea->isChecked()) {
			if (legend->isAttachedToChart()) {
				legend->detachFromChart();
				legend->setBackgroundVisible(true);
				legend->setBrush(QBrush(QColor(128, 128, 128, 128)));
				legend->setPen(QPen(QColor(192, 192, 192, 192)));
				legend->setGeometry(m_chart->geometry().center().x()+100, 0, 100, 60);
			}
			legend->show();
			legend->setFlags(QGraphicsItem::GraphicsItemFlags(QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsMovable));
		}
		else {
			legend->hide();
		}
	}
	else {
		legend->attachToChart();
		//legend->detachFromChart();

		legend->setBackgroundVisible(false);
		legend->setAlignment(alignment);
		legend->show();
	}
	legend->update();
	update();

	QString strFamily = fontComboBox->currentText();
	QFont font;
	font.setPixelSize(cbFontSize_Title->currentText().toInt());
	font.setFamily(strFamily);
	m_chart->setTitleFont(font);


	// chart brush
	if (rbChartBrush_1->isChecked()) {
		m_chart->setBackgroundBrush(Qt::gray);
	}
	else {
		m_chart->setBackgroundBrush(Qt::yellow);
	}

	// plotarea brush
	m_chart->setPlotAreaBackgroundVisible(true);
	if (rbPlotAreaBrush_1->isChecked()) {
		m_chart->setPlotAreaBackgroundBrush(Qt::darkBlue);
	}
	else {
		m_chart->setPlotAreaBackgroundBrush(Qt::darkMagenta);
	}

	// 是否显示刻度线
	m_axisX->setGridLineVisible(cbGridLineVisibleX->isChecked());
	m_axisY->setGridLineVisible(cbGridLineVisibleY->isChecked());

	// 设置坐标轴字体 
	QFont labelsFont;
	labelsFont.setPixelSize(cbLabelFontSize_axisX->currentText().toInt()); 
	m_axisX->setLabelsFont(labelsFont); // X轴
	labelsFont.setPixelSize(cbLabelFontSize_axisY->currentText().toInt());
	m_axisY->setLabelsFont(labelsFont);	// Y轴

	// 主题-字体颜色
	Qt::GlobalColor clr = static_cast<Qt::GlobalColor>(cbBrush_Title->itemData(cbBrush_Title->currentIndex()).toInt());
	m_chart->setTitleBrush(clr);
	
	// 设置系列1的线色
	clr = static_cast<Qt::GlobalColor>(cbPenColor_Serie1->itemData(cbPenColor_Serie1->currentIndex()).toInt());
	QPen pn1(clr, sbPenWidth_Serie1->value());
	m_series1->setPen(pn1);

	// 设置系列2的线色
	clr = static_cast<Qt::GlobalColor>(cbPenColor_Serie2->itemData(cbPenColor_Serie2->currentIndex()).toInt());
	QPen pn2(clr, sbPenWidth_Serie2->value());
	m_series2->setPen(pn2);

	QChartView::RubberBand rubberBand = static_cast<QChartView::RubberBand>(cbZoom->itemData(cbZoom->currentIndex()).toInt());
	m_pCurveChartView->setRubberBand(rubberBand);
	if (QChartView::NoRubberBand == rubberBand) {
		m_chart->zoomReset();
	}
	// 双坐标轴
	QPen pn(m_series2->pen());
	pn.setWidth(1);
	m_axisY3->setGridLinePen(pn);
	m_axisY3->setLinePenColor(m_series2->pen().color());

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
void CDialog::resizeEvent(QResizeEvent *event)
{
	QDialog::resizeEvent(event);
}
void CDialog::onTimeout()
{
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


void CDialog::populateThemeBox()
{
	// add items to theme combobox
	themeComboBox->addItem("Light", QChart::ChartThemeLight);
	themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
	themeComboBox->addItem("Dark", QChart::ChartThemeDark);
	themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
	themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
	themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
	themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
	themeComboBox->addItem("Qt", QChart::ChartThemeQt);
}

void CDialog::populateAnimationBox()
{
	// add items to animation combobox
	animatedComboBox->addItem("No Animations", QChart::NoAnimation);
	animatedComboBox->addItem("GridAxis Animations", QChart::GridAxisAnimations);
	animatedComboBox->addItem("Series Animations", QChart::SeriesAnimations);
	animatedComboBox->addItem("All Animations", QChart::AllAnimations);
}

void CDialog::populateLegendBox()
{
	// add items to legend combobox
	legendComboBox->addItem("No Legend ", 0);
	legendComboBox->addItem("Legend Top", Qt::AlignTop);
	legendComboBox->addItem("Legend Bottom", Qt::AlignBottom);
	legendComboBox->addItem("Legend Left", Qt::AlignLeft);
	legendComboBox->addItem("Legend Right", Qt::AlignRight);
}


void CDialog::connectMarkers()
{
	// Connect all markers to handler
	const auto markers = m_chart->legend()->markers();
	for (QLegendMarker *marker : markers) {
		// 断开已有信号槽的连接，防止重复连接。
		QObject::disconnect(marker, &QLegendMarker::clicked,
			this, &CDialog::handleMarkerClicked);
		QObject::connect(marker, &QLegendMarker::clicked, this, &CDialog::handleMarkerClicked);
	}
}

void CDialog::disconnectMarkers()
{
	const auto markers = m_chart->legend()->markers();
	for (QLegendMarker *marker : markers) {
		QObject::disconnect(marker, &QLegendMarker::clicked,
			this, &CDialog::handleMarkerClicked);
	}
}

void CDialog::handleMarkerClicked()
{
	QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
	Q_ASSERT(marker);

	switch (marker->type())
	{
	case QLegendMarker::LegendMarkerTypeXY:
	{
		
		// Toggle visibility of series
		marker->series()->setVisible(!marker->series()->isVisible());

		// Turn legend marker back to visible, since hiding series also hides the marker
		// and we don't want it to happen now.
		marker->setVisible(true);

		// Dim the marker, if series is not visible
		qreal alpha = 1.0;

		if (!marker->series()->isVisible())
			alpha = 0.5;

		QColor color;
		QBrush brush = marker->labelBrush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setLabelBrush(brush);

		brush = marker->brush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setBrush(brush);

		QPen pen = marker->pen();
		color = pen.color();
		color.setAlphaF(alpha);
		pen.setColor(color);
		marker->setPen(pen);
		break;
	}
	default:
	{
		qDebug() << "Unknown marker type";
		break;
	}
	}
}
