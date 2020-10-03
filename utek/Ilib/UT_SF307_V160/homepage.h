#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QDateTime>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMetaType>
#include "bartonwidget.h"
#include "rightbartonwidget.h"
#include "handlemodbus.h"

namespace Ui {
class HomePage;
}

// 零界点个数对应ErrorInfo的个数,TODO：需要改成列表索引的方式
const int LIMIT_RECOVER_CODE = 30;

const QVector<QString> ERROR_INFO{
    "",
    "舍内温度偏差超上限",
    "舍内温度偏差超下限",
    "舍内湿度超上限",
    "舍内湿度超下限",
    "二氧化碳浓度超上限",
    "1-2通风级别舍内压力超上限",
    "1-2通风级别舍内压力超下限",
    "3-16通风级别舍内压力上限",
    "3-16通风级别舍内压力超下限",
    "温度传感器1故障",
    "温度传感器2故障",
    "温度传感器3故障",
    "温度传感器4故障",
    "温度传感器5故障",
    "温度传感器6故障",
    "湿度传感器故障",
    "压力传感器故障",
    "二氧化碳传感器故障",
    "侧窗故障",
    "翻板1故障",
    "翻板2故障",
    "热回收机组故障",
    "热回收送风机故障",
    "热回收排风机故障",
    "热回收热水比例阀故障",
    "热回收机组旁通阀故障",
    "新风滤网异常",
    "消除报警",
    "模拟状态",
    "翻板三故障告警",
};

struct AlarmInfo{
    QString mac;
    QString ip;
    QString hardware;
    QDateTime datetime;
    uint  info;
};

// 报警记录的处理类
//class AlarmTaskTrhead : public QObject
//{
//    Q_OBJECT
//public:
//    AlarmTaskTrhead(QWidget *parent=nullptr) : QObject(parent){}
//    ~AlarmTaskTrhead(){}

//public slots:
//    // 获取指定页的报警信息, page: 页码, remainder: 报警总个数和分页个数的余数
//    void getPageAlarm(int page, int remainder, int &no, QDateTime &datetime, QString &info);
//    void setHandleModbus(std::weak_ptr<HandleModbus> handle){
//        this->m_pHandbleModbus = handle;
//    }


//signals:
//    void alarmInfo(int no, QDateTime datetime, QString info);
//private:
//    std::weak_ptr<HandleModbus> m_pHandbleModbus;
//};

// 报警信息入库和查询
class AlarmReadDB : public QObject
{
    Q_OBJECT
public:
    AlarmReadDB(QWidget *parent=nullptr) : QObject(parent){}
    ~AlarmReadDB(){}

    static bool insertAlarm(AlarmInfo info);

    static bool selectAlarm(QDate date, QString macAddr, QVector<AlarmInfo> &result);

    static QString getAlarmInfo(uint errorCode){
        if(errorCode > LIMIT_RECOVER_CODE){
            uint code = errorCode - 0x80;
            return ERROR_INFO[static_cast<int>(code)] + "恢复";
        }else {
            return ERROR_INFO[static_cast<int>(errorCode)];
        }
    }
};

// TODO: QVector<uint16_t > 为什么会重复定义
struct UInt16_T{
    UInt16_T(){}
    UInt16_T(QVector<uint16_t> info) : buf(info){ }
    QVector<uint16_t> buf;
};

Q_DECLARE_METATYPE(UInt16_T);
class HomeTaskThread : public QThread
{
    Q_OBJECT
public:
    HomeTaskThread(){
        qRegisterMetaType<UInt16_T>("UInt16_T");
    }
    ~HomeTaskThread(){}

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
        void buf2FR(UInt16_T buf);
        void buf00R(UInt16_T buf);
        void bufDisR(UInt16_T buf);
        void buf117R(UInt16_T buf);
        void bufInpR(UInt16_T buf);
        void buf0FR(UInt16_T buf);
        void buf3CR(UInt16_T buf);
        void buf13AR(UInt16_T buf);
        void buf12CR(UInt16_T buf);
        void buf133R(UInt16_T buf);
        void buf58R(UInt16_T buf);
        void buf5CR(UInt16_T buf);
        void buf13CR(UInt16_T buf);
        void buf174R(UInt16_T buf);
private:
    bool m_isExit = false;
    bool m_isPaused = false;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();
    void addAlarmInfo(QString info);
    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
        this->m_pHome->setPaused(true);
        this->m_pHome->setHandleModbus(this->m_pHandbleModbus);
        this->m_pHome->start();
    }

    void initConnection();

    void updatePageData(bool flag=true){
        m_pHome->setPaused(!flag);
    }

protected:
    void dumpAlarm(uint code1, uint code2);
    void isLx(bool status);

private:
    Ui::HomePage *ui;

    BartonWidget *m_pBartonWidget;
    RightBartonWidget *m_pRightBartonWidget;

    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    std::shared_ptr<HomeTaskThread> m_pHome;

    // 料线开关标志
    bool m_lxflag = false;
    // 二氧化碳开关
    bool m_co2flag = false;

    QString m_alarmInfo;
};

#endif // HOMEPAGE_H
