#ifndef FRMGAUGECOLOR_H
#define FRMGAUGECOLOR_H

#include <QWidget>

namespace Ui {
class frmGaugeColor;
}

class frmGaugeColor : public QWidget
{
    Q_OBJECT

public:
    explicit frmGaugeColor(QWidget *parent = 0);
    ~frmGaugeColor();

private:
    Ui::frmGaugeColor *ui;

private slots:
    void initForm();
};

#endif // FRMGAUGECOLOR_H
