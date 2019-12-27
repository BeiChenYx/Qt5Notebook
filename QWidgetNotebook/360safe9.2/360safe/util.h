/*
	工具类
	作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/


#ifndef UTIL_H
#define UTIL_H

#include <QSettings>
#include <QFontMetrics>
#include "common.h"

class Util
{

public:

	static bool writeInit(QString path, QString user_key, QString user_value);
	static bool readInit(QString path, QString user_key, QString &user_value);
	static bool updateText(QString text, int max_width, QString &elided_text);
	static QString getSkinName();

};

#endif