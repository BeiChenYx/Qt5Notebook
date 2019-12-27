/*
	木马查杀 包括快速扫描、全盘扫描、自定义扫描等
	作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/

#ifndef  KILLMUMMYWIDGET_H
#define  KILLMUMMYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>

class KillMummyWidget : public QWidget
{
	Q_OBJECT

public:

	explicit KillMummyWidget(QWidget *parent = 0);
	~KillMummyWidget();
	void translateLanguage();

private:

	QLabel *tip_icon_label;  //提示图标
	QLabel *tip_info_label;  //提示信息
	QLabel *tip_suggest_label;  //建议信息
	QToolButton *fast_scan_button; //快速扫描
	QToolButton *full_scan_button; //全盘扫描
	QToolButton *custom_scan_button; //自定义扫描
};

#endif  KILLMUMMYWIDGET_H