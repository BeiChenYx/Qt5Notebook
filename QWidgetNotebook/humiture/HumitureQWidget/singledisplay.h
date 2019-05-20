#ifndef SINGLEDISPLAY_H
#define SINGLEDISPLAY_H

#include <QWidget>
#include <QHBoxLayout>
#include "humiturecharts.h"

namespace Ui {
class SingleDisplay;
}

class SingleDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit SingleDisplay(QWidget *parent = nullptr);
    ~SingleDisplay();

private:
    Ui::SingleDisplay *ui;

    HumitureCharts  *pHumitureCharts;
};

#endif // SINGLEDISPLAY_H
