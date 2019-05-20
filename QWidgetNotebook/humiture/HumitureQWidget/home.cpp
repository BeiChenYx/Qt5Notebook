#include "home.h"
#include "ui_home.h"
#include <QDebug>

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    this->pSingleDisplay = new SingleDisplay(this);
    this->pSingleConfig = new SingleConfig(this);
    this->pDoubleDisplay = new DoubleDisplay(this);
    this->singleCurrentIndex = 0;
    this->doubleCurrentIndex = 0;

    initUI();
    initConnections();
}

Home::~Home()
{
    delete ui;
}

void Home::initUI()
{
    ui->stackedWidget_single->insertWidget(0, this->pSingleDisplay);
    ui->stackedWidget_single->insertWidget(1, this->pSingleConfig);
    this->singleCounts = 2;

    ui->stackedWidget_double->insertWidget(0, this->pDoubleDisplay);
    this->doubleCounts = 1;

    ui->stackedWidget_single->setCurrentIndex(0);
    ui->stackedWidget_double->setCurrentIndex(0);
}

void Home::initConnections()
{

}

void Home::on_pushButton_single_left_clicked()
{
    if(this->singleCurrentIndex > 0){
        this->singleCurrentIndex -= 1;
        ui->stackedWidget_single->setCurrentIndex(this->singleCurrentIndex);
    }
}

void Home::on_pushButton_single_right_clicked()
{
    if(this->singleCurrentIndex + 1 < this->singleCounts){
        this->singleCurrentIndex += 1;
        ui->stackedWidget_single->setCurrentIndex(this->singleCurrentIndex);
    }
}

void Home::on_pushButton_double_left_clicked()
{
    if(this->doubleCurrentIndex> 0){
        this->doubleCurrentIndex-= 1;
        ui->stackedWidget_double->setCurrentIndex(this->doubleCurrentIndex);
    }
}

void Home::on_pushButton_double_right_clicked()
{
    if(this->doubleCurrentIndex + 1 < this->doubleCounts){
        this->doubleCurrentIndex += 1;
        ui->stackedWidget_double->setCurrentIndex(this->doubleCurrentIndex);
    }
}
