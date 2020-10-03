#include "handlemodbus.h"
#include "rightbartonwidget.h"
#include "ui_rightbartonwidget.h"

RightBartonWidget::RightBartonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightBartonWidget)
{
    ui->setupUi(this);

    m_wet = new QMovie(":/images/wet.gif");
    ui->label_wet->setMovie(m_wet);
    m_wet->start();
    m_wet->setPaused(true);
}

RightBartonWidget::~RightBartonWidget()
{
    delete ui;
}

void RightBartonWidget::swIndex(int index)
{
    ui->label_sw->clear();
    int no = index / 25 + 1;
    if(index == 0){ no = 0; }
    if(index == 100){ no = 4; }
    switch (no){
    case 0:
        ui->label_sw->setPixmap(QPixmap(":/images/cechuang-AA.png"));
        break;
    case 1:
        ui->label_sw->setPixmap(QPixmap(":/images/cechuang-BB.png"));
        break;
    case 2:
        ui->label_sw->setPixmap(QPixmap(":/images/cechuang-CC.png"));
        break;
    case 3:
        ui->label_sw->setPixmap(QPixmap(":/images/cechuang-DD.png"));
        break;
    case 4:
        ui->label_sw->setPixmap(QPixmap(":/images/cechuang-EE.png"));
        break;
    default:
        ui->label_sw->setPixmap(QPixmap(":/images/cechuang-EE.png"));
        break;
    }

}

void RightBartonWidget::fbIndex(int index)
{
    ui->label_fb->clear();
    int no = index / 25 + 1;
    if(index == 0){ no = 0; }
    if(index == 100){ no = 4; }
    switch (no){
    case 0:
        ui->label_fb->setPixmap(QPixmap(":/images/FB-A.png"));
        break;
    case 1:
        ui->label_fb->setPixmap(QPixmap(":/images/FB-B.png"));
        break;
    case 2:
        ui->label_fb->setPixmap(QPixmap(":/images/FB-C.png"));
        break;
    case 3:
        ui->label_fb->setPixmap(QPixmap(":/images/FB-D.png"));
        break;
    case 4:
        ui->label_fb->setPixmap(QPixmap(":/images/FB-E.png"));
        break;
    default:
        ui->label_fb->setPixmap(QPixmap(":/images/FB-E.png"));
        break;
    }
}

void RightBartonWidget::setHotValue(bool s, bool m, bool e)
{
   ui->checkBox__beginHot->setChecked(!s);
   ui->checkBox_middleHot->setChecked(!m);
   ui->checkBox_endHot->setChecked(!e);
}

void RightBartonWidget::setLightValue(bool light)
{
    ui->checkBox_light->setChecked(!light);
}
void RightBartonWidget::setWetPaused(bool status)
{
    m_wet->setPaused(!status);
}

void RightBartonWidget::setSendHotWater(bool status)
{
    ui->label_SendHotWater->setText(status ? "ON" : "OFF");
}

void RightBartonWidget::setNewWindValue(bool status)
{
    ui->label_NewFeng->setText(status ? "ON" : "OFF");
}

void RightBartonWidget::setIndoorT(uint16_t value)
{
    ui->label_indoor_t_display->setText(QString("%1°C").arg(static_cast<int16_t>(value) / 100.00));
}

void RightBartonWidget::setHumidityValue(uint16_t value)
{
    if(g_Wet){
        ui->label_humidity_display->setText(QString("%1%").arg(static_cast<int16_t>(value) / 100.00));
    }else{
        ui->label_humidity_display->setText("***%");
    }
}

void RightBartonWidget::setT5ToT6Value(uint16_t t5, uint16_t t6)
{
    ui->T5_V->setText(QString("%1°C").arg(static_cast<int16_t>(t5) / 100.00));
    ui->T6_V->setText(QString("%1°C").arg(static_cast<int16_t>(t6) / 100.00));
}

void RightBartonWidget::setRH11Value(uint16_t value)
{
    ui->label_RH11->setText(QString("%1%").arg(static_cast<int16_t>(value)));
}

void RightBartonWidget::setRH03RH13Value(uint16_t rh03, uint16_t rh13)
{
    if(g_Hot){
        ui->label_RH03->setText(QString("%1%").arg(static_cast<int16_t>(rh03)));
        ui->label_RH13->setText(QString("%1%").arg(static_cast<int16_t>(rh13)));
    }else {
        ui->label_RH03->setText("***");
        ui->label_RH13->setText("***");
    }
}

void RightBartonWidget::setRH02RH12Value(uint16_t rh02, uint16_t rh12)
{
    ui->label_RH02->setText(QString("%1%").arg(static_cast<int16_t>(rh02)));
    ui->label_RH12->setText(QString("%1%").arg(static_cast<int16_t>(rh12)));
}

void RightBartonWidget::setSWValue(uint16_t control, uint16_t fead)
{
    ui->label_swControl->setText(QString("%1%").arg(static_cast<int16_t>(control)));
    ui->label_swFeadback->setText(QString("%1%").arg(static_cast<int16_t>(fead)));
    this->swIndex(static_cast<int>(control));
}
void RightBartonWidget::setFBlValue(uint16_t control, uint16_t fb1, uint16_t fb2)
{
    ui->label_fbFeedback1->setText(QString("%1%").arg(static_cast<int16_t>(fb1)));
    ui->label_fbFeedback2->setText(QString("%1%").arg(static_cast<int16_t>(fb2)));
    ui->label_fbControl->setText(QString("%1%").arg(static_cast<int16_t>(control)));
    this->fbIndex(static_cast<int>(control));
}

void RightBartonWidget::setWetTime(uint16_t open, uint16_t close)
{
    ui->label_startTime->setText(QString("%1S").arg(static_cast<int16_t>(open)));
    ui->label_closeTime->setText(QString("%1S").arg(static_cast<int16_t>(close)));
}

void RightBartonWidget::setT7Value(uint16_t value)
{
    if(g_Hot){
        ui->T7_V->setText(QString("%1°C").arg(static_cast<int16_t>(value) / 100.00));
    }else{
        ui->T7_V->setText("***°C");
    }
}

void RightBartonWidget::setFLLVValue(uint16_t fl, uint16_t lv)
{
    ui->label_feng_liang->setText(QString("%1km3/h").arg(static_cast<int16_t>(fl)));
    ui->label_xiao_lv->setText(QString("%1%").arg(static_cast<int16_t>(lv)));
}

void RightBartonWidget::setLXTValue(uint16_t lxH, uint16_t lxL)
{
    uint lx = lxH;
    lx = (lx << 16) | lxL;
    ui->label_lx_t->setText(QString("%1min").arg(lx));
}
void RightBartonWidget::setLXVValue(uint16_t lxvH, uint16_t lxvL)
{
    uint lxv = lxvH;
    lxv = (lxv << 16) | lxvL;
    ui->label_lx_v->setText(QString("%1L").arg(lxv));
}

void RightBartonWidget::setLx(bool status)
{
//    ui->label_lx_t_str->setVisible(status);
//    ui->label_lx_v_str->setVisible(status);
//    ui->label_lx_t->setVisible(status);
//    ui->label_lx_v->setVisible(status);
    if(!status){
        ui->label_lx_t->setText("***");
        ui->label_lx_v->setText("***");
    }
}

void RightBartonWidget::setRecoveryTimeVol(uint16_t tH, uint16_t tL, uint16_t vH, uint16_t vL)
{
    uint t = tH;
    t = (t << 16) | tL;
    uint v = vH;
    v = (v << 16) | vL;
    ui->label_xinzeng1->setText(QString("%1h").arg(static_cast<int>(t) / 10.0));
    ui->label_xinzeng2->setText(QString("%1mj").arg(static_cast<int>(v) / 10.0));
}



