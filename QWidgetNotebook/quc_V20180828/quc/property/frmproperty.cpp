#pragma execution_character_set("utf-8")

#include "frmproperty.h"
#include "ui_frmproperty.h"

#include "gaugecar.h"
#include "gaugecolor.h"
#include "qpainter.h"
#include "qevent.h"
#include "qdebug.h"

frmProperty::frmProperty(QWidget *parent) : QWidget(parent), ui(new Ui::frmProperty)
{
    ui->setupUi(this);
    this->initForm();
}

frmProperty::~frmProperty()
{
    delete ui;
}

bool frmProperty::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        ui->objectController->setObject(watched);
    }

    return QWidget::eventFilter(watched, event);
}

void frmProperty::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawTiledPixmap(rect(), bgPix);
}

void frmProperty::initForm()
{
    bgPix = QPixmap(":/bg.png");

    GaugeColor *gg1 = new GaugeColor;
    gg1->installEventFilter(this);
    ui->widgetMain->layout()->addWidget(gg1);
}

void frmProperty::on_btnOpen_clicked()
{

}

void frmProperty::on_btnSave_clicked()
{

}
