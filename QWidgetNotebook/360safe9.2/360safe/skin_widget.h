﻿/*
	换肤  换肤按钮、上一页、下一页等
	作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/

#ifndef SKIN_WIDGET_H
#define SKIN_WIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include "common.h"
#include "push_button.h"
#include "change_skin_widget.h"
#include "drop_shadow_widget.h"

class SkinWidget : public DropShadowWidget
{
	Q_OBJECT

public:

	explicit SkinWidget(QWidget *parent = 0);
	void translateLanguage();

protected:

	void paintEvent(QPaintEvent *event);

signals:

	void changeSkin(QString skin_name);

private slots:

	void showSkin(QString page_count);
	void varyfySkin();

private:

	void initTitle();
	void initCenter();
	void initBottom();

private:

	QString skin_name; //背景图片
	bool is_change; //是否改变背景
	QStringList skin_list;
	QStringList tip_list;

	QHBoxLayout *title_layout;
	QGridLayout *center_layout;
	QHBoxLayout *bottom_layout;

	QLabel *title_label; //标题
	QLabel *title_icon_label; //标题图标
	PushButton *close_button; //关闭按钮
	int page_count; //总页数
	int page_count_point; //最后一页的显示个数
	int current_page; //当前为第几页

	QPushButton *first_page_button;
	QPushButton *previous_page_button;
	QPushButton *next_page_button;
	QPushButton *last_page_button;

	QList<ChangeSkinWidget *> change_skin_list;

};

#endif // SKIN_WIDGET_H
