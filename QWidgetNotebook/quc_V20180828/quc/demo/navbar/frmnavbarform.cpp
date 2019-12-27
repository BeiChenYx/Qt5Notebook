#pragma execution_character_set("utf-8")

#include "frmnavbarform.h"
#include "ui_frmnavbarform.h"
#include "qtimer.h"
#include "qdebug.h"

frmNavBarForm::frmNavBarForm(QWidget *parent) : QWidget(parent), ui(new Ui::frmNavBarForm)
{
    ui->setupUi(this);
    this->initForm();
}

frmNavBarForm::~frmNavBarForm()
{
    delete ui;
}

void frmNavBarForm::initForm()
{
    ui->navBar->setBarColorStart(QColor(24, 189, 155));
    ui->navBar->setBarColorEnd(QColor(24, 189, 155));
    ui->navBar->setItems("主界面;系统设置;防区管理;警情查询;视频预");
    connect(ui->navBar, SIGNAL(currentItemChanged(int, QString)), this, SLOT(currentItemChanged(int, QString)));
}

void frmNavBarForm::currentItemChanged(int index, const QString &item)
{
    ui->stackedWidget->setCurrentIndex(index);
    qDebug() << "index" << index << "item" << item;
}
