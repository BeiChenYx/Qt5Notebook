#ifndef DOUBLEDISPLAY_H
#define DOUBLEDISPLAY_H

#include <QWidget>
#include <QMap>
#include <QHBoxLayout>
#include "humiturecharts.h"

namespace Ui {
class DoubleDisplay;
}

class DoubleHumitureChart : public HumitureCharts
{
public:
    DoubleHumitureChart(HumitureCharts *parent = nullptr);

};

class DoubleDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleDisplay(QWidget *parent = nullptr);
    ~DoubleDisplay();
    void initChart(int deviceAddr);
    void onHumitureDisplay(int deviceAddr, double temperature, double humidity);


public:
    QMap<QString, QLineSeries*>  deviceNoHandle;
private:
    Ui::DoubleDisplay *ui;
    DoubleHumitureChart *pHumitureCharts;
};

#endif // DOUBLEDISPLAY_H
