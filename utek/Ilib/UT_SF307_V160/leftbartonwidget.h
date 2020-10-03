#ifndef LEFTBARTONWIDGET_H
#define LEFTBARTONWIDGET_H

#include <QWidget>

namespace Ui {
class LeftBartonWidget;
}

class LeftBartonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeftBartonWidget(QWidget *parent = nullptr);
    ~LeftBartonWidget();
    void setAimT(uint16_t value);
    void setInPa(uint16_t value);
    void setRunStatus(bool status);
    void setAirLevel(uint16_t value);
    void setAirLevelLimit(uint16_t min, uint16_t max);
    void setBreadNums(uint16_t heigh, uint16_t low);
    void setCurrntAge(uint16_t age);

private:
    Ui::LeftBartonWidget *ui;
};

#endif // LEFTBARTONWIDGET_H
