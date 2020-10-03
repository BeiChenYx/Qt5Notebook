#ifndef PARSETFIVE_H
#define PARSETFIVE_H

#include <QWidget>
#include <QThread>
#include "handlemodbus.h"

namespace Ui {
class ParSetFive;
}

class FiveTaskThread : public QThread
{
    Q_OBJECT
public:
    FiveTaskThread(){}
    ~FiveTaskThread(){}

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
    void bufB7R(QVector<uint16_t> buf);
    void bufC3R(QVector<uint16_t> buf);
    void bufDFR(QVector<uint16_t> buf);
    void buf11DR(QVector<uint16_t> buf);
    void buf135R(QVector<uint16_t> buf);
    void buf33R(QVector<uint16_t> buf);
    void buf16R(QVector<uint16_t> buf);
    void buf16ER(QVector<uint16_t> buf);
    void buf175R(QVector<uint16_t> buf);

private:
    bool m_isExit = false;
    bool m_isPaused = false;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};


class ParSetFive : public QWidget
{
    Q_OBJECT

public:
    explicit ParSetFive(QWidget *parent = nullptr);
    ~ParSetFive();
    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
        m_pFive->setPaused(true);
        m_pFive->setHandleModbus(this->m_pHandbleModbus);
        m_pFive->start();
    }

    void initConnection();

    // 温度传感器1-6
    void initTConnect();
    // 光照时间
    void initLightConnect();

    // 从modbus中读取一次该页面内的数据, 并开启定时器间隔读取
    void updatePageData(bool flag=true);
private slots:
    void on_doubleSpinBox_sw0_editingFinished();
    void on_doubleSpinBox_sw100_editingFinished();
    void on_spinBox_sw_no_editingFinished();
    void on_doubleSpinBox_fp0_editingFinished();
    void on_doubleSpinBox_fp100_editingFinished();
    void on_spinBox_fp_no_editingFinished();
    void on_doubleSpinBox_fpt0_editingFinished();
    void on_doubleSpinBox_fpt100_editingFinished();
    void on_spinBox_fpt_no_editingFinished();
    void on_doubleSpinBox_bp0_editingFinished();
    void on_doubleSpinBox_bp100_editingFinished();
    void on_spinBox_bp_no_editingFinished();
    void on_doubleSpinBox_t7_editingFinished();
    void on_doubleSpinBox_humidity_editingFinished();
    void on_doubleSpinBox_pa_editingFinished();
    void on_doubleSpinBox_co2_editingFinished();
    void on_doubleSpinBox_T7_u_editingFinished();
    void on_doubleSpinBox_T7_d_editingFinished();
    void on_spinBox_co2_u_editingFinished();
    void on_spinBox_co2_d_editingFinished();
    void on_doubleSpinBox_wc_o_t_editingFinished();
    void on_spinBox_wc_o_time_editingFinished();
    void on_spinBox_wc_circle_time_editingFinished();
    void on_spinBox_wc_min_level_editingFinished();
    void on_doubleSpinBox_wc_c_t_editingFinished();
    void on_doubleSpinBox_wc_deviation_editingFinished();
    void on_doubleSpinBox_hot_o_d_editingFinished();
    void on_doubleSpinBox_hot_c_d_editingFinished();
    void on_spinBox_illumination_editingFinished();

    void on_doubleSpinBox_feed_vol_editingFinished();

    void on_pushButton_feed_clear_clicked();

    void on_spinBox_gradual_time_editingFinished();

    void on_spinBox_nh3_d_editingFinished();

    void on_spinBox_nh3_u_editingFinished();

private:
    Ui::ParSetFive *ui;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    std::shared_ptr<FiveTaskThread> m_pFive;
};

#endif // PARSETFIVE_H
