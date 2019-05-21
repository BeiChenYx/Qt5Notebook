#include "home.h"
#include "ui_home.h"
#include <QDebug>

// 外部全局变量有问题....
extern const int DEVICE_COUNTS;
const int DEVICE_COUNTS_HOME = 10;

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    this->pSingleDisplay = new SingleDisplay(this);
    this->pSingleConfig = new SingleConfig(this);
    this->pHumitureRecord = new HumitureRecordPage(this);
    this->pDoubleDisplay1 = new DoubleDisplay(this);
    this->pDoubleDisplay2 = new DoubleDisplay(this);
    int mid = DEVICE_COUNTS_HOME / 2;
    for (int i = 1; i <= DEVICE_COUNTS_HOME; i++) {
        if(i <= mid){
            this->pDoubleDisplay1->initChart(i);
        }else {
            this->pDoubleDisplay2->initChart(i);
        }
    }

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
    ui->stackedWidget_single->insertWidget(2, this->pHumitureRecord);
    this->singleCounts = 3;
    this->singleCurrentIndex = 0;

    ui->stackedWidget_double->insertWidget(0, this->pDoubleDisplay1);
    ui->stackedWidget_double->insertWidget(1, this->pDoubleDisplay2);
    this->doubleCounts = 2;
    this->doubleCurrentIndex = 0;

    ui->stackedWidget_single->setCurrentIndex(0);
    ui->stackedWidget_double->setCurrentIndex(0);
}

void Home::initConnections()
{
    connect(this->pSingleDisplay, SIGNAL(currentAddr(int)),
            this, SIGNAL(singleCurrentAddr(int)));
    connect(this->pSingleConfig, SIGNAL(currentDeviceAddr(int)),
            this->pSingleDisplay, SLOT(onCurrentDeviceAddr(int)));
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

void Home::onSingleHumidity(int deviceAddr, double temperature, double humidity)
{
    this->pSingleDisplay->onHumitureDisplay(deviceAddr, temperature, humidity);
}

void Home::onDoubleHumidity(int deviceAddr, double temperature, double humidity)
{
    qDebug() << "deviceAddr: " << deviceAddr;
    int mid = DEVICE_COUNTS_HOME / 2;
    if(deviceAddr <= mid){
        this->pDoubleDisplay1->onHumitureDisplay(deviceAddr, temperature, humidity);
    }else {
        this->pDoubleDisplay2->onHumitureDisplay(deviceAddr, temperature, humidity);
    }
}
