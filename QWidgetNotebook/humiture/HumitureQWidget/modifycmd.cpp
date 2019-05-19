#include "modifycmd.h"
#include "ui_modifycmd.h"

ModifyCmd::ModifyCmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyCmd)
{
    ui->setupUi(this);
}

ModifyCmd::~ModifyCmd()
{
    delete ui;
}
