#include "frmprogress.h"
#include "ui_frmprogress.h"

#include "frmprogressbarwater.h"
#include "frmprogressbarcolor.h"
#include "frmprogressbarpercent.h"
#include "frmprogressbarround.h"
#include "frmprogressbarwait.h"

frmProgress::frmProgress(QWidget *parent) : QWidget(parent), ui(new Ui::frmProgress)
{
    ui->setupUi(this);
    this->initForm();
}

frmProgress::~frmProgress()
{
    delete ui;
}

void frmProgress::initForm()
{
    ui->stackedWidget->addWidget(new frmProgressBarWater);
    ui->stackedWidget->addWidget(new frmProgressBarPercent);
    ui->stackedWidget->addWidget(new frmProgressBarRound);
    ui->stackedWidget->addWidget(new frmProgressBarWait);
    ui->stackedWidget->addWidget(new frmProgressBarColor);
}

void frmProgress::setIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
