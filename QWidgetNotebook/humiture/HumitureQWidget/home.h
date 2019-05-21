#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include "singledisplay.h"
#include "singleconfig.h"
#include "doubledisplay.h"
#include "humiturerecord.h"


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

signals:
    void singleCurrentAddr(int addr);

public slots:
    void on_pushButton_single_left_clicked();

    void on_pushButton_single_right_clicked();

    void on_pushButton_double_left_clicked();

    void on_pushButton_double_right_clicked();

    void onSingleHumidity(int deviceAddr, double temperature, double humidity);
    void onDoubleHumidity(int deviceAddr, double temperature, double humidity);


private:
    Ui::Home *ui;

    SingleDisplay       *pSingleDisplay;
    SingleConfig        *pSingleConfig;
    HumitureRecordPage  *pHumitureRecord;
    DoubleDisplay       *pDoubleDisplay1;
    DoubleDisplay       *pDoubleDisplay2;

    // 两个页面当前的索引
    int             singleCurrentIndex;
    int             singleCounts;
    int             doubleCurrentIndex;
    int             doubleCounts;
};

#endif // HOME_H
