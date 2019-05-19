#include "home.h"
#include "ui_home.h"

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    this->pSingleDisplay = new SingleDisplay(this);
//    this->pSingleConfig = new SingleConfig(this);
    this->pDoubleDisplay = new DoubleDisplay(this);
}

Home::~Home()
{
    delete ui;
}

void Home::initUI()
{
    ui->stackedWidget_single->insertWidget(0, this->pSingleDisplay);
//    ui->stackedWidget_single->insertWidget(1, this->pSingleConfig);

    ui->stackedWidget_double->insertWidget(0, this->pDoubleDisplay);

    ui->stackedWidget_single->setCurrentIndex(0);
    ui->stackedWidget_double->setCurrentIndex(0);
}

void Home::initConnections()
{

}

void Home::on_pushButton_single_left_clicked()
{

}

void Home::on_pushButton_single_right_clicked()
{

}

void Home::on_pushButton_double_left_clicked()
{

}

void Home::on_pushButton_double_right_clicked()
{

}
