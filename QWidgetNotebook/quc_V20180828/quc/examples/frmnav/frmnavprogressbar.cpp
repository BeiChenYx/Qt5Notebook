#pragma execution_character_set("utf-8")

#include "frmnavprogressbar.h"
#include "ui_frmnavprogressbar.h"
#include "navprogressbar.h"
#include "qtimer.h"

frmNavProgressBar::frmNavProgressBar(QWidget *parent) : QWidget(parent), ui(new Ui::frmNavProgressBar)
{
	ui->setupUi(this);
    QTimer::singleShot(0, this, SLOT(initForm()));
}

frmNavProgressBar::~frmNavProgressBar()
{
	delete ui;
}

void frmNavProgressBar::initForm()
{
	QStringList topInfo;
	QStringList bottomInfo;

	topInfo.clear();
    topInfo << "创建订单" << "审核订单" << "生产" << "配送" << "签收";

    ui->navProgressBar1->setTopInfo(topInfo);
    ui->navProgressBar1->setMaxStep(5);
    ui->navProgressBar1->setCurrentStep(3);

    ui->navProgressBar2->setTopInfo(topInfo);
    ui->navProgressBar2->setMaxStep(5);
    ui->navProgressBar2->setCurrentStep(5);
    ui->navProgressBar2->setCurrentBackground(QColor(24,189,155));

	topInfo.clear();
	topInfo << "拍下商品" << "付款到支付宝" << "卖家发货" << "确认收货" << "评价";

	bottomInfo.clear();
	bottomInfo << "2016-11-24 20:58:59" << "2016-11-24 21:25:26" << "2016-11-25 10:25:26" << "2016-11-25 15:26:58" << "2016-11-25 20:36:39";

    ui->navProgressBar3->setTopInfo(topInfo);
    ui->navProgressBar3->setBottomInfo(bottomInfo);
    ui->navProgressBar3->setMaxStep(5);
    ui->navProgressBar3->setCurrentStep(3);
    ui->navProgressBar3->setNavStyle(NavProgressBar::NavStyle_TB);

    ui->navProgressBar4->setTopInfo(topInfo);
    ui->navProgressBar4->setBottomInfo(bottomInfo);
    ui->navProgressBar4->setMaxStep(5);
    ui->navProgressBar4->setCurrentStep(5);
    ui->navProgressBar4->setCurrentBackground(QColor(24,189,155));
    ui->navProgressBar4->setNavStyle(NavProgressBar::NavStyle_TB);

	topInfo.clear();
	topInfo << "已发货" << "运输中" << "派件中" << "已签收" << "已评价";

	bottomInfo.clear();
	bottomInfo << "深圳市" << "上海市";

    ui->navProgressBar5->setTopInfo(topInfo);
    ui->navProgressBar5->setBottomInfo(bottomInfo);
    ui->navProgressBar5->setMaxStep(5);
    ui->navProgressBar5->setCurrentStep(3);
    ui->navProgressBar5->setNavStyle(NavProgressBar::NavStyle_ZFB);

    ui->navProgressBar6->setTopInfo(topInfo);
    ui->navProgressBar6->setBottomInfo(bottomInfo);
    ui->navProgressBar6->setMaxStep(5);
    ui->navProgressBar6->setCurrentStep(5);
    ui->navProgressBar6->setCurrentBackground(QColor(24,189,155));
    ui->navProgressBar6->setNavStyle(NavProgressBar::NavStyle_ZFB);
}
