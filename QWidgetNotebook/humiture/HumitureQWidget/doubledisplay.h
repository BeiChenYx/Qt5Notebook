#ifndef DOUBLEDISPLAY_H
#define DOUBLEDISPLAY_H

#include <QWidget>

namespace Ui {
class DoubleDisplay;
}

class DoubleDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleDisplay(QWidget *parent = nullptr);
    ~DoubleDisplay();

private:
    Ui::DoubleDisplay *ui;
};

#endif // DOUBLEDISPLAY_H
