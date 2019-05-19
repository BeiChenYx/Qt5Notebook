#include "modbustest.h"
#include "ui_modbustest.h"

ModbusTest::ModbusTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModbusTest)
{
    ui->setupUi(this);
}

ModbusTest::~ModbusTest()
{
    delete ui;
}
