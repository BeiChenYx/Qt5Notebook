#ifndef PARSETSIX_H
#define PARSETSIX_H

#include <QWidget>
#include <QThread>
#include "handlemodbus.h"

namespace Ui {
class ParSetSix;
}

class SixTaskThread : public QThread
{
    Q_OBJECT
public:
    SixTaskThread(){}
    ~SixTaskThread(){}

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
    void buf139R(QVector<uint16_t> buf);
    void buf125R(QVector<uint16_t> buf);
    void buf12BR(uint16_t buf);
    void buf117R(QVector<uint16_t> buf);
    void buf141R(uint16_t buf);
    void buf128R(QVector<uint16_t> buf);
    void buf173R(QVector<uint16_t> buf);

private:
    bool m_isExit = false;
    bool m_isPaused = false;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};


class ParSetSix : public QWidget
{
    Q_OBJECT

public:
    explicit ParSetSix(QWidget *parent = nullptr);
    ~ParSetSix();
    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
        m_pSix->setPaused(true);
        m_pSix->setHandleModbus(this->m_pHandbleModbus);
        m_pSix->start();
    }

    void initConnection();
    // 从modbus中读取一次该页面内的数据, 并开启定时器间隔读取
    void updatePageData(bool flag=true);
private slots:
    void on_checkBox_CC_clicked();
    void on_checkBox_Lx_clicked();
    void on_checkBox_model_clicked();
    void on_checkBox_FB1_clicked();
    void on_checkBox_FB2_clicked();
    void on_checkBox_select_clicked();
    void on_checkBox_Pa_clicked();
    void on_checkBox_Wet_clicked();
    void on_checkBox_Alarm_clicked();
    void on_checkBox_CO2_clicked();
    void on_checkBox_Hot_clicked();
    void on_checkBox_control_clicked();

    void on_spinBox_sw_time_editingFinished();

    void on_checkBox_in10_clicked();

private:
    Ui::ParSetSix *ui;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    std::shared_ptr<SixTaskThread> m_pSix;
};

#endif // PARSETSIX_H
