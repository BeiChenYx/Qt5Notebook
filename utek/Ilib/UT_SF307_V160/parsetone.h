#ifndef PARSETONE_H
#define PARSETONE_H

#include <QWidget>
#include <QMetaType>
#include <QThread>
#include "handlemodbus.h"


namespace Ui {
class ParSetOne;
}

class OneTaskThread : public QThread
{
    Q_OBJECT
public:
    OneTaskThread(){}
    ~OneTaskThread(){}

    void setHandleModbus(std::weak_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
    }
    void exitThread(){
        this->m_isExit = true;
        this->quit();
        this->wait(1000);
    }
    void setPaused(bool paused){ this->m_isPaused = paused; }

protected:
        void run();

signals:
        void buf005FR(QVector<uint16_t> buf);
        void buf00R(QVector<uint16_t> buf);
        void buf12FR(uint16_t buf);
        void currentDayR(uint16_t day);
        void buf1ER(QVector<uint16_t> buf);
        void buf109R(QVector<uint16_t> buf);
        void buf30R(QVector<uint16_t> buf);
        void buf166R(QVector<uint16_t> buf);
        void calcWeightModelR(uint16_t weight);
private:
    bool m_isExit = false;
    bool m_isPaused = false;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};


class ParSetOne : public QWidget
{
    Q_OBJECT

public:
    explicit ParSetOne(QWidget *parent = nullptr);
    ~ParSetOne();

    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
        m_pOne->setPaused(true);
        m_pOne->setHandleModbus(this->m_pHandbleModbus);
        m_pOne->start();
    }

    void initConnection();
    // 初始化目标温度和目标日龄的信号槽
    void initAimDayTConnect();
    // 最小通风比例里面的七个日龄， 单只体重和单位体积通风量
    void initUnitAirConnect();
    // 通风级别
    void initAirLevelConnect();
    // 折扣时间(最小夜间通风设置)
    void initDiscountConnect();

    // 从modbus中读取一次该页面内的数据, 并开启定时器间隔读取
    void updatePageData(bool flag=true);

private slots:
    void on_spinBox_current_days_editingFinished();
    void on_comboBox_matrix_category_currentIndexChanged(int index);
    void on_doubleSpinBox_discount_coefficient_editingFinished();
    void on_spinBox_matrix_count_editingFinished();
    void on_checkBox_clicked();

private:
    Ui::ParSetOne *ui;

    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    std::shared_ptr<OneTaskThread> m_pOne;

    bool m_isCategoryClicked = false;
};

#endif // PARSETONE_H
