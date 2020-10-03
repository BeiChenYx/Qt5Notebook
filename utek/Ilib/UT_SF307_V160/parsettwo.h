#ifndef PARSETTWO_H
#define PARSETTWO_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include "handlemodbus.h"

namespace Ui {
class ParSetTwo;
}

class TwoTaskThread : public QThread
{
    Q_OBJECT
public:
    TwoTaskThread(){}
    ~TwoTaskThread(){}

    void setHandleModbus(std::weak_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
    }
    void exitThread(){
        this->m_isExit = true;
        this->quit();
        this->wait(1000);
    }
    void setPaused(bool paused){ this->m_isPaused = paused; }
signals:
    void buf42R(QVector<uint16_t> buf);
protected:
    void run();
private:
    bool m_isExit = false;
    bool m_isPaused = false;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};

class ParSetTwo : public QWidget
{
    Q_OBJECT

public:
    explicit ParSetTwo(QWidget *parent = nullptr);
    ~ParSetTwo();

    void initConnection();
    // 风机组风量设置
    void initAirVolConnect();
    // 风机组合的状态
    void initAirConbinationConnect();

    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
        m_pTwo->setPaused(true);
        m_pTwo->setHandleModbus(this->m_pHandbleModbus);
        m_pTwo->start();
    }

    // 从modbus中读取一次该页面内的数据, 并开启定时器间隔读取
    void updatePageData(bool flag=true);

private slots:

    // 更新组合风量, 更新数据需要调用，用户更新风机选择要调用
    bool calcAirVolSum();
    void on_spinBox_circle_time_editingFinished();

private:
    Ui::ParSetTwo *ui;

    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    std::shared_ptr<TwoTaskThread> m_pTwo;
};

#endif // PARSETTWO_H
