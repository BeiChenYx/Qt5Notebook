#ifndef FRMGAUGEPROGRESSBAR_H
#define FRMGAUGEPROGRESSBAR_H

#include <QWidget>

class GaugeProgressBar;

namespace Ui {
class frmGaugeProgressBar;
}

class frmGaugeProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit frmGaugeProgressBar(QWidget *parent = 0);
    ~frmGaugeProgressBar();

private:
    Ui::frmGaugeProgressBar *ui;
    QList<GaugeProgressBar *> gauges;

private slots:
    void initForm();
    void on_ckShowPointer_stateChanged(int arg1);
    void on_ckShowValue_stateChanged(int arg1);
    void on_cboxPointerStyle_currentIndexChanged(int index);
};

#endif // FRMGAUGEPROGRESSBAR_H
