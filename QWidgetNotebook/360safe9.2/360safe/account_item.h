/*
	用户信息 包括用户图标、昵称、帐号、删除按钮等
	作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/

#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>

class AccountItem: public QWidget
{
	Q_OBJECT

public:

	explicit AccountItem(QWidget *parent = 0);
	~AccountItem();
	void setAccountNumber(QString account_text);
	QString getAccountNumber();

protected:

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

signals:

	void showAccount(QString account);
	void removeAccount(QString account_number_text);

private slots:

	void removeAccount();

private:

	QLabel *account_number;  //账号
	QPushButton *delede_button;  //删除账号按钮
	bool mouse_press;

};

#endif //ACCOUNTITEM_H
