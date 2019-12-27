/*
	标题  包括：最小化、关闭等
	作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/

#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QVBoxLayout>
#include "push_button.h"
#include "tool_button.h"
#include "main_menu.h"

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
	void translateLanguage();

signals:

	void showSkin();
    void showMin();
	void showMainMenu();
	void closeWidget();
	void turnPage(int current_page);

public slots:

	void turnPage(QString current_page);

private:

    QPoint press_point;//鼠标按下去的点
    bool is_move;

	QLabel *version_title; //标题
	PushButton *medal_button; //勋章墙
    PushButton *skin_button; //换肤
    PushButton *main_menu_button; //主菜单
    PushButton *min_button; //最小化
    PushButton *feedback_button; //反馈意见
    PushButton *close_button; //关闭

	QLabel *safe_360_label;  //360安全卫士
	QLabel *safe_label;  //360安全卫士

	QList<ToolButton *> button_list;
};

#endif // TITLEWIDGET_H
