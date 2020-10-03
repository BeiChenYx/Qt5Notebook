#ifndef PARSETTHREE_H
#define PARSETTHREE_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include "handlemodbus.h"

namespace Ui {
class ParSetThree;
}

class ThreeTaskThread : public QThread
{
    Q_OBJECT
public:
    ThreeTaskThread(){}
    ~ThreeTaskThread(){}

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
    void buf3FR(QVector<uint16_t> buf);
    void buf130R(QVector<uint16_t> buf);
    void buf57R(QVector<uint16_t> buf);
    void buf1BR(QVector<uint16_t> buf);
    void buf44R(QVector<uint16_t> buf);
protected:
    void run();
private:
    bool m_isExit = false;
    bool m_isPaused = false;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};


class ParSetThree : public QWidget
{
    Q_OBJECT

public:
    explicit ParSetThree(QWidget *parent = nullptr);
    ~ParSetThree();

    void initConnection();

    // 温度偏差
    void initDeviationConnect();
    // 侧窗
    void initSideWindowConnect();
    // 翻板
    void initFlipPlateConnect();

    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
        m_pThree->setPaused(true);
        m_pThree->setHandleModbus(this->m_pHandbleModbus);
        m_pThree->start();
    }

    // 从modbus中读取一次该页面内的数据, 并开启定时器间隔读取
    void updatePageData(bool flag=true);
private slots:
    void on_spinBox_add_delay_editingFinished();
    void on_spinBox_sub_delay_editingFinished();
    void on_comboBox_air_model_currentIndexChanged(int index);
    void on_spinBox_open_close_time_editingFinished();


private:
    Ui::ParSetThree *ui;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    std::shared_ptr<ThreeTaskThread> m_pThree;

    bool m_isAirModelClicked = false;
};

#endif // PARSETTHREE_H
