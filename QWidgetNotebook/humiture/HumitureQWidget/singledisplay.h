#ifndef SINGLEDISPLAY_H
#define SINGLEDISPLAY_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "humiturecharts.h"

namespace Ui {
class SingleDisplay;
}

class SingleHumitureChart : public HumitureCharts
{
public:
    SingleHumitureChart(HumitureCharts *parent = nullptr);

};

class SingleDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit SingleDisplay(QWidget *parent = nullptr);
    ~SingleDisplay();

signals:
    void currentAddr(int addr);

public slots:
    void onHumitureDisplay(int deviceAddr, double temperature, double humidity);

    void onCurrentDeviceAddr(int deviceAddr);

private:
    Ui::SingleDisplay *ui;

    SingleHumitureChart     *pHumitureCharts;
    QLineSeries             *plineT; // 温度
    QLineSeries             *plineH; // 湿度

    // 当前显示的设备地址;
    int                     deviceAddr;
};

#endif // SINGLEDISPLAY_H
