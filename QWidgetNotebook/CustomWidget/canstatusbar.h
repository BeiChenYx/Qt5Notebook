#ifndef CANSTATUSBAR_H
#define CANSTATUSBAR_H

#include <QWidget>

namespace Ui {
class CanStatusBar;
}

class CanStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit CanStatusBar(QWidget *parent = nullptr);
    ~CanStatusBar();

private:
    Ui::CanStatusBar *ui;
};

#endif // CANSTATUSBAR_H
