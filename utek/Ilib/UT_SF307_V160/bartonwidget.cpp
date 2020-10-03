#include "bartonwidget.h"
#include "handlemodbus.h"
#include "ui_bartonwidget.h"
#include <QDebug>

BartonWidget::BartonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BartonWidget)
{
    ui->setupUi(this);
    this->initUi();
}

BartonWidget::~BartonWidget()
{
    delete ui;
}

void BartonWidget::initUi()
{
    m_pBartonMovie = new QMovie(":/images/1_1.gif");
    m_pBartonMovie->start();
    m_pBartonMovie->setPaused(true);
    ui->label_barton->setMovie(m_pBartonMovie);

    // 风扇
    for(int i = 0; i < 7; i++){
        auto pFanMovie = new QMovie(":/images/fan.gif");
        pFanMovie->start();
        pFanMovie->setPaused(true);
        m_pFansMovie.append(pFanMovie);

        QLabel* pFanLabel = new QLabel(this);
        pFanLabel->setMovie(pFanMovie);
        m_pFansLabel.append(pFanLabel);
        int fanXi = m_fanX0 + i * (m_fanXOffset + m_fanWidth);
        int fanYi = m_fanY0 + i * m_fanYOffset;
        pFanLabel->setGeometry(fanXi, fanYi, m_fanWidth, m_fanHeight);
        pFanLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
        pFanLabel->show();
    }

    // 添加热回收
    m_pHotMovie = new QMovie(":/images/hot.gif");
    m_pHotMovie->start();
    m_pHotMovie->setPaused(true);
    m_pHotLabel = new QLabel(this);
    m_pHotLabel->setMovie(m_pHotMovie);
    m_pHotLabel->setGeometry(m_bartonX0 + m_hotOffsetX0, m_bartonY0 + m_hotOffsetY0, m_hotWidth, m_hotHeight);
    m_pHotLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    m_pHotLabel->show();

    // 温度 Tn (舍内温度，设外温度 T1 T2 ...)
    // T1
    m_pT1Widget = new HoverInfoWidget(this, "T1");
    m_pT1Widget->setGeometry(m_bartonX0 + 400, m_bartonY0 - 230, m_pT1Widget->width(), m_pT1Widget->height());
    m_pT1Widget->show();
    // T2
    m_pT2Widget = new HoverInfoWidget(this, "T2");
    m_pT2Widget->setGeometry(m_bartonX0 + 280, m_bartonY0 - 100, m_pT2Widget->width(), m_pT2Widget->height());
    m_pT2Widget->show();
    // T3
    m_pT3Widget = new HoverInfoWidget(this, "T3");
    m_pT3Widget->setGeometry(m_bartonX0 + 110, m_bartonY0 + 55, m_pT3Widget->width(), m_pT3Widget->height());
    m_pT3Widget->show();
    // T4
    m_pT4Widget = new HoverInfoWidget(this, "T4");
    m_pT4Widget->setGeometry(m_bartonX0 - 10, m_bartonY0 - 70, m_pT4Widget->width(), m_pT4Widget->height());
    m_pT4Widget->show();

    // 二氧化碳
    m_pCO2Widget = new CO2Widget(this);
    m_pCO2Widget->setGeometry(m_bartonX0 - 10, m_bartonY0 - 110, m_pCO2Widget->width(), m_pCO2Widget->height());
    m_pCO2Widget->show();

    // 氨气
    m_pNH3Widget = new NH3Widget(this);
    m_pNH3Widget->setGeometry(m_bartonX0 - 10, m_bartonY0 - 150, m_pCO2Widget->width(), m_pCO2Widget->height());
    m_pNH3Widget->show();

    // 左上的室内温度，湿度，以及左上角的表格
    m_pLeftWidget = new LeftBartonWidget(this);
    m_pLeftWidget->setGeometry(10, 10, m_pLeftWidget->width(), m_pLeftWidget->height());
    m_pLeftWidget->show();
}

QWidget* BartonWidget::GenarateTnWidget()
{
    QWidget *pWidget = new QWidget(this);
    QHBoxLayout *phLayout = new QHBoxLayout();
    QLabel *pNameLabel = new QLabel(this);
    pNameLabel->setObjectName("name");
    QLabel *pValueLabel = new QLabel(this);
    pValueLabel->setObjectName("value");
    phLayout->addWidget(pNameLabel);
    phLayout->addWidget(pValueLabel);
    phLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(phLayout);
    return pWidget;
}

void BartonWidget::setBartonPaused(bool paused)
{
    this->m_pBartonMovie->setPaused(!paused);
}

void BartonWidget::setFanPaused(int index, bool paused)
{
    if(index > m_pFansMovie.size() || index < 0){return;}
    m_pFansMovie.at(index)->setPaused(!paused);
}

void BartonWidget::setHotPaused(bool paused)
{
    m_pHotMovie->setPaused(!paused);
    if(g_HotCount){
        if(!m_pHotLabel->isVisible()){m_pHotLabel->setVisible(true);}
    }else{
        m_pHotLabel->setVisible(false);
    }
}

void BartonWidget::setCO2(bool status)
{
    if(!status){
        m_pCO2Widget->setValue("***");
    }
}

void BartonWidget::setCO2Value(uint16_t value)
{
    m_pCO2Widget->setValue(QString::number(static_cast<int>(value)));
}

void BartonWidget::setT1ToT4Value(uint16_t t1, uint16_t t2, uint16_t t3, uint16_t t4)
{
    m_pT1Widget->setValue(QString("%1").arg(static_cast<int16_t>(t1) / 100.00));
    m_pT2Widget->setValue(QString("%1").arg(static_cast<int16_t>(t2) / 100.00));
    m_pT3Widget->setValue(QString("%1").arg(static_cast<int16_t>(t3) / 100.00));
    m_pT4Widget->setValue(QString("%1").arg(static_cast<int16_t>(t4) / 100.00));
}

void BartonWidget::setNH3Value(uint16_t nh3)
{
    if(g_NH3){
        m_pNH3Widget->setValue(QString::number(nh3));
    }else{
        m_pNH3Widget->setValue("***");
    }
}
