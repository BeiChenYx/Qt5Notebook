#include "canstatusbar.h"
#include "ui_canstatusbar.h"

CanStatusBar::CanStatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanStatusBar)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);
}

CanStatusBar::~CanStatusBar()
{
    delete ui;
}
