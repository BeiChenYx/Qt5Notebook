#pragma execution_character_set("utf-8")

#include "frmdemo.h"
#include "ui_frmdemo.h"

#include "xform.h"
#include "composition.h"
#include "gradients.h"
#include "pathdeform.h"
#include "pathstroke.h"

frmDemo::frmDemo(QWidget *parent) : QWidget(parent), ui(new Ui::frmDemo)
{
    ui->setupUi(this);
    this->initForm();
}

frmDemo::~frmDemo()
{
    delete ui;
}

void frmDemo::initForm()
{
    ui->tabWidget->addTab(new XFormWidget,"企鹅矢量图");
    ui->tabWidget->addTab(new CompositionWidget,"颜色拾取");
    ui->tabWidget->addTab(new GradientWidget,"矢量颜色");
    ui->tabWidget->addTab(new PathDeformWidget,"放大镜");
    ui->tabWidget->addTab(new PathStrokeWidget,"矢量线条");
}

void frmDemo::setIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}
