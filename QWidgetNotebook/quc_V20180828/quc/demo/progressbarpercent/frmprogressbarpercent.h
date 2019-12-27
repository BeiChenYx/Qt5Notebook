#ifndef FRMPROGRESSBARPERCENT_H
#define FRMPROGRESSBARPERCENT_H

#include <QWidget>

class ProgressBarPercent;

namespace Ui {
class frmProgressBarPercent;
}

class frmProgressBarPercent : public QWidget
{
    Q_OBJECT

public:
    explicit frmProgressBarPercent(QWidget *parent = 0);
    ~frmProgressBarPercent();

private:
    Ui::frmProgressBarPercent *ui;
    QList<ProgressBarPercent *> bars;

private slots:
    void initForm();
    void rbtnClick();

private slots:
    void on_ckClockWise_stateChanged(int arg1);
    void on_ckBorderOut_stateChanged(int arg1);
};

#endif // FRMPROGRESSBARPERCENT_H
