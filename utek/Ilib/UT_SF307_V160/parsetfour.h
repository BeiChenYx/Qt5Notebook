#ifndef PARSETFOUR_H
#define PARSETFOUR_H

#include <QWidget>
#include <QThread>
#include "handlemodbus.h"

namespace Ui {
class ParSetFour;
}

class FourTaskThread : public QThread
{
    Q_OBJECT
public:
    FourTaskThread(){}
    ~FourTaskThread(){}

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
    void buf2ER(QVector<uint16_t> buf);
    void buf132R(uint16_t buf);
    void buf41R(uint16_t buf);
    void buf12AR(uint16_t buf);
    void buf12ER(uint16_t buf);
    void buf11AR(QVector<uint16_t> buf);
    void bufD1R(QVector<uint16_t> buf);
    void bufFCR(uint16_t buf);

private:
    bool m_isExit = false;
    bool m_isPaused = false;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};


class ParSetFour : public QWidget
{
    Q_OBJECT

public:
    explicit ParSetFour(QWidget *parent = nullptr);
    ~ParSetFour();
    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
        m_pFour->setPaused(true);
        m_pFour->setHandleModbus(this->m_pHandbleModbus);
        m_pFour->start();
    }

    void initConnection();

    // 从modbus中读取一次该页面内的数据, 并开启定时器间隔读取
    void updatePageData(bool flag=true);
private slots:
    void on_spinBox_in_vol_editingFinished();
    void on_spinBox_out_vol_editingFinished();
    void on_spinBox_min_open_time_editingFinished();
    void on_spinBox_control_time_editingFinished();
    void on_doubleSpinBox_heating_t_editingFinished();
    void on_spinBox_hoter_count_editingFinished();
    void on_doubleSpinBox_antifreeze1_editingFinished();
    void on_doubleSpinBox_antifreeze2_editingFinished();
    void on_doubleSpinBox_antifreeze3_editingFinished();

    void on_pushButton_vol_clear_clicked();

    void on_doubleSpinBox_deviation_u_editingFinished();

    void on_doubleSpinBox_deviation_d_editingFinished();

    void on_doubleSpinBox_humidity_u_editingFinished();

    void on_doubleSpinBox_humidity_d_editingFinished();

    void on_spinBox_co2_u_editingFinished();

    void on_doubleSpinBox_pa_1_2_u_editingFinished();

    void on_doubleSpinBox_pa_1_2_d_editingFinished();

    void on_doubleSpinBox_pa_3_16_u_editingFinished();

    void on_doubleSpinBox_pa_3_16_d_editingFinished();

    void on_spinBox_alarm_delay_editingFinished();

    void on_spinBox_error_swtich_delay_editingFinished();

private:
    Ui::ParSetFour *ui;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    std::shared_ptr<FourTaskThread> m_pFour;
};

#endif // PARSETFOUR_H
