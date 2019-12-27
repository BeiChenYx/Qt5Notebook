/*! 
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: dialog.h
\brief 示例头文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
\Date 2018/9
*/
#ifndef DIALOG_H
#define DIALOG_H

#include <QtCharts>		// xingdianketang
QT_CHARTS_USE_NAMESPACE// xingdianketang

#include "ui_dialogbase.h" // 头文件名称来自: dialogbase.ui  ---> ui_dialogbase.h


// 第一个父类的名称来自ui文件中对话框的类名：对象查看器中的类名
// 第二个父类的名称来自ui文件中对话框基类的名称(objeceName属性)： CDialogBase
class CDialog : public QDialog, public Ui::CDialogBase
{
	Q_OBJECT
public:
	CDialog(QWidget* pParent);
	~CDialog();
private slots:
	void onTimeout();

    void on_leTextX_editingFinished();
	;
    void on_leTextY_editingFinished();
	void updateUI();
	void serie1Hovered(const QPointF&,bool);
	void serie2Hovered(const QPointF&,bool);
private:
	/**
	* @brief 初始化。
	*/
	void initialize();
	/**
	* @brief 获取曲线数据。
	* @param[in] time 时间点。
	* @return 对应的曲线数据
	*/
    double getData(double time);
	/**
	* @brief 初始化主题。
	* @return void
	*/
	void populateThemeBox();
	/**
	* @brief 初始化主题动画选项。
	* @return void
	*/
	void populateAnimationBox();
	/**
	* @brief 初始化图例选项。
	* @return void
	*/
	void populateLegendBox();
	/**
	* @brief 绑定信号槽。
	* @return void
	*/
	void connectSignalSlots();
	/**
	* @brief 根据主题设置整个对话框窗体的调色板。
	* @return void
	*/
	void setDialogPalette();

	/**
	* @brief 初始化各个配置参数控件。
	* @return void
	*/
	void initializeControls();

private:
	QSplineSeries *m_series1;
	QSplineSeries *m_series2;
    QChart      *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
	QTimer		m_timer;
	int			m_tmLast;
};

#endif
