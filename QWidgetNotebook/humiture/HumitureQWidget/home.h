#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include "singledisplay.h"
#include "singleconfig.h"
#include "doubledisplay.h"


namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

    void initUI();
    void initConnections();

private slots:
    void on_pushButton_single_left_clicked();

    void on_pushButton_single_right_clicked();

    void on_pushButton_double_left_clicked();

    void on_pushButton_double_right_clicked();

private:
    Ui::Home *ui;

    SingleDisplay   *pSingleDisplay;
    SingleConfig    *pSingleConfig;
    DoubleDisplay   *pDoubleDisplay;

    // 两个页面当前的索引
    int             singleCurrentIndex;
    int             doubleCurrentIndex;
};

#endif // HOME_H
