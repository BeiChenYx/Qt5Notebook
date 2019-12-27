#include "frmpainter.h"
#include "ui_frmpainter.h"

#include "frmlightbutton.h"
#include "frmlightpoint.h"
#include "frmmagicfish.h"
#include "frmmagicmouse.h"
#include "frmmagicpool.h"
#include "frmmagicpoolfish.h"
#include "frmroundcircle.h"
#include "frmroundwidget.h"
#include "frmswitchbutton.h"
#include "frmoventimer.h"
#include "frmtumbler.h"
#include "frmtumblerdatetime.h"
#include "frmlunarcalendarinfo.h"
#include "frmlunarcalendarwidget.h"
#include "frmtiledbg.h"

frmPainter::frmPainter(QWidget *parent) : QWidget(parent), ui(new Ui::frmPainter)
{
    ui->setupUi(this);
    this->initForm();
}

frmPainter::~frmPainter()
{
    delete ui;
}

void frmPainter::initForm()
{
    ui->stackedWidget->addWidget(new frmLightButton);
    ui->stackedWidget->addWidget(new frmLightPoint);
    ui->stackedWidget->addWidget(new frmMagicFish);
    ui->stackedWidget->addWidget(new frmMagicMouse);
    ui->stackedWidget->addWidget(new frmMagicPool);
    ui->stackedWidget->addWidget(new frmMagicPoolFish);
    ui->stackedWidget->addWidget(new frmRoundCircle);
    ui->stackedWidget->addWidget(new frmRoundWidget);
    ui->stackedWidget->addWidget(new frmSwitchButton);
    ui->stackedWidget->addWidget(new frmOvenTimer);
    ui->stackedWidget->addWidget(new frmTumbler);
    ui->stackedWidget->addWidget(new frmTumblerDateTime);
    ui->stackedWidget->addWidget(new frmLunarCalendarInfo);
    ui->stackedWidget->addWidget(new frmLunarCalendarWidget);
    ui->stackedWidget->addWidget(new frmTiledBg);
}

void frmPainter::setIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
