/*
	阴影边框
	作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/

#ifndef SHADOWWIDGET_H
#define SHADOWWIDGET_H

#include <QDialog>
#include <QPainter>
#include "common.h"

class ShadowWidget : public QDialog
{
	Q_OBJECT

public:

	explicit ShadowWidget(QWidget *parent = 0);
	~ShadowWidget();

protected:

	virtual void paintEvent(QPaintEvent *event);

private:

	void drawWindowShadow(QPainter &painter);

};


#endif //SHADOWWIDGET_H