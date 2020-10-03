#ifndef RIGHTBARTONWIDGET_H
#define RIGHTBARTONWIDGET_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class RightBartonWidget;
}

class RightBartonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RightBartonWidget(QWidget *parent = nullptr);
    ~RightBartonWidget();

    void setHotValue(bool s, bool m, bool e);
    void setWetPaused(bool status);
    void setLightValue(bool light);
    void setSendHotWater(bool status);
    void setNewWindValue(bool status);
    void setIndoorT(uint16_t value);
    void setHumidityValue(uint16_t value);
    void setT5ToT6Value(uint16_t t5, uint16_t t6);
    void setRH11Value(uint16_t value);
    void setRH03RH13Value(uint16_t rh03, uint16_t rh13);
    void setRH02RH12Value(uint16_t rh02, uint16_t rh12);
    void setSWValue(uint16_t control, uint16_t fead);
    void setFBlValue(uint16_t control, uint16_t fb1, uint16_t fb2);
    void setT7Value(uint16_t value);
    void setFLLVValue(uint16_t fl, uint16_t lv);
    void setLXTValue(uint16_t lxH, uint16_t lxL);
    void setLXVValue(uint16_t lxvH, uint16_t lxvL);
    void setRecoveryTimeVol(uint16_t tH, uint16_t tL, uint16_t vH, uint16_t vL);
    void setWetTime(uint16_t open, uint16_t close);

    void setLx(bool status);

protected:
    void swIndex(int index);
    void fbIndex(int index);

private:
    Ui::RightBartonWidget *ui;

    QMovie *m_wet;
};

#endif // RIGHTBARTONWIDGET_H
