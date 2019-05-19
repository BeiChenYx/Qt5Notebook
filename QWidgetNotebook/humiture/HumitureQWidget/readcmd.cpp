#include "readcmd.h"
#include "ui_readcmd.h"

ReadCmd::ReadCmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadCmd)
{
    ui->setupUi(this);
}

ReadCmd::~ReadCmd()
{
    delete ui;
}
