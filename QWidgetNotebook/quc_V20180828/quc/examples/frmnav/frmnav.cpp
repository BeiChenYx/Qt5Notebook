#include "frmnav.h"
#include "ui_frmnav.h"

#include "frmnavbutton.h"
#include "frmnavlabel.h"
#include "frmnavprogressbar.h"
#include "frmnavlistview.h"
#include "frmnavlistviewform.h"
#include "frmnavbar.h"
#include "frmnavbarform.h"

frmNav::frmNav(QWidget *parent) : QWidget(parent), ui(new Ui::frmNav)
{
    ui->setupUi(this);
    this->initForm();
}

frmNav::~frmNav()
{
    delete ui;
}

void frmNav::initForm()
{
    ui->stackedWidget->addWidget(new frmNavButton);
    ui->stackedWidget->addWidget(new frmNavLabel);    
    ui->stackedWidget->addWidget(new frmNavProgressBar);
    ui->stackedWidget->addWidget(new frmNavListView);
    ui->stackedWidget->addWidget(new frmNavListViewForm);
    ui->stackedWidget->addWidget(new frmNavBar);
    ui->stackedWidget->addWidget(new frmNavBarForm);
}

void frmNav::setIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
