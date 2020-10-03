#ifndef BARTONWIDGET_H
#define BARTONWIDGET_H

#include <QWidget>
#include <QMovie>
#include <QLabel>
#include <QList>
#include <QHBoxLayout>
#include "leftbartonwidget.h"
#include "hoverinfowidget.h"

namespace Ui {
class BartonWidget;
}


// 温度 Tn (舍内温度，设外温度 T1 T2 ...)

class CO2Widget : public HoverInfoWidget{
    Q_OBJECT
public:
    explicit CO2Widget(QWidget *parent=nullptr,
                       QString name="<p>CO<span style=\" vertical-align:sub;\">2</span></p>",
                       QString value="***",
                       QString symbol="ppm")
        : HoverInfoWidget(parent, name, value, symbol){}
};

class NH3Widget : public HoverInfoWidget{
    Q_OBJECT
public:
    explicit NH3Widget(QWidget *parent=nullptr,
                       QString name="<p>NH<span style=\" vertical-align:sub;\">3</span></p>",
                       QString value="***",
                       QString symbol="ppm")
        : HoverInfoWidget(parent, name, value, symbol){}
};

class BartonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BartonWidget(QWidget *parent = nullptr);
    ~BartonWidget();

    void setBartonPaused(bool paused);
    void setFanPaused(int index, bool paused);
    void setHotPaused(bool paused);
    void setCO2Value(uint16_t value);
    void setT1ToT4Value(uint16_t t1, uint16_t t2, uint16_t t3, uint16_t t4);
    void setNH3Value(uint16_t nh3);

    void setCO2(bool status);

protected:
    void initUi();
    QWidget* GenarateTnWidget();

public:
    // 左上的室内温度，湿度，以及左上角的表格
    LeftBartonWidget *m_pLeftWidget;

private:
    Ui::BartonWidget *ui;
    QList<QLabel*> m_pFansLabel;
    QList<QMovie*> m_pFansMovie;
    QMovie *m_pBartonMovie;

    // 初始参考值
    const int m_bartonWidth = 604;
    const int m_bartonHeight = 504;
    const int m_fanWidth = 20;
    const int m_fanHeight = 32;

    // 畜禽舍左下屋顶的坐标 10*300
    const int m_bartonX0 = 10;
    const int m_bartonY0 = 300;

    // 畜禽舍左下屋顶到图片左边和底 11px*125px, 图片固定 604px*504px
    const int m_fanX0 = 23;
    const int m_fanY0 = m_bartonHeight - 120;
    // 每张图间隔 4px * 9px
    const int m_fanXOffset = 4;
    const int m_fanYOffset = 9;

    // 热回收距离 畜禽舍参考点 300 * 30, 宽高 280px*150px
    QLabel *m_pHotLabel;
    QMovie *m_pHotMovie;
    const int m_hotWidth = 280;
    const int m_hotHeight = 150;
    const int m_hotOffsetX0 = 300;
    const int m_hotOffsetY0 = 30;

    // 温度 Tn (舍内温度，设外温度 T1 T2 ...)
    HoverInfoWidget *m_pT1Widget;
    HoverInfoWidget *m_pT2Widget;
    HoverInfoWidget *m_pT3Widget;
    HoverInfoWidget *m_pT4Widget;
    // 二氧化碳
    CO2Widget *m_pCO2Widget;
    // 氨气
    NH3Widget *m_pNH3Widget;
};

#endif // BARTONWIDGET_H
