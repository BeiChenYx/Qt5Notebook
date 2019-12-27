#ifndef FRMPROGRESSBARWATER_H
#define FRMPROGRESSBARWATER_H

#include <QWidget>

namespace Ui {
class frmProgressBarWater;
}

class frmProgressBarWater : public QWidget
{
    Q_OBJECT

public:
    explicit frmProgressBarWater(QWidget *parent = 0);
    ~frmProgressBarWater();

private:
    Ui::frmProgressBarWater *ui;

private slots:
    void initForm();

private slots:
    void on_sliderBorderWidth_valueChanged(int value);
    void on_sliderWaterHeight_valueChanged(int value);
    void on_sliderWaterDensity_valueChanged(int value);
    void on_ckReverse_stateChanged(int arg1);
    void on_cboxPercentStyle_currentIndexChanged(const QString &arg1);
    void on_btnPause_clicked();
};

#endif // FRMPROGRESSBARWATER_H
