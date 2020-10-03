#include <QVBoxLayout>
#include <QDebug>
#include "datarecordwidget.h"
#include "homepage.h"
#include "ui_homepage.h"

extern const QString DB_NAME;
extern const QString CONNECT_NAME;
extern const QString ALARM_TABLE_NAME;


//void AlarmTaskTrhead::getPageAlarm(int page, int remainder, int &no, QDateTime &datetime, QString &info)
//{
//    auto handle = m_pHandbleModbus.lock();
//    if(!handle){return;}
//    uint16_t minIndex = static_cast<uint16_t>((page - 1) * ALARM_PAGE_COUNT);
//    uint16_t maxIndex = static_cast<uint16_t>(minIndex + remainder > 0 ? remainder : ALARM_PAGE_COUNT);
//    for (int i=minIndex; i<maxIndex; ++i) {
//        if(handle->writeSingleRegister(0x18D1, 0x01) > 0){
//            QThread::msleep(200);
//            uint16_t buf18D1[9]{0};
//            if(handle->readHoldingRegisters(0x18D1, 9, buf18D1) > 0){
//                datetime = QDateTime(QDate(buf18D1[2], buf18D1[3], buf18D1[4]), QTime(buf18D1[5], buf18D1[6], buf18D1[7]));
//                no = minIndex+i;
//                info = AlarmReadDB::getAlarmInfo(buf18D1[8];
////                emit alarmInfo(minIndex+i, datetime, AlarmReadDB::getAlarmInfo(buf18D1[8]));
//            }
//        }
//    }
//}

bool AlarmReadDB::insertAlarm(AlarmInfo info)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECT_NAME);
    db.setDatabaseName(DB_NAME);
    db.open();
    QString sql = QString("INSERT INTO %1 (mac_addr, ip_addr, hardware, date, info) VALUES \
    ('%2', '%3', '%4', '%5', %6);").arg(ALARM_TABLE_NAME).arg(info.mac).arg(info.ip)
            .arg(info.hardware).arg(info.datetime.toString("yyyy-MM-dd hh:mm:ss")).arg(info.info);
    QSqlQuery query(db);
    if(!query.exec(sql)){
        qDebug() << query.lastError().text();
    }
    db.close();
    QSqlDatabase::removeDatabase(CONNECT_NAME);
    return true;
}

bool AlarmReadDB::selectAlarm(QDate date, QString macAddr, QVector<AlarmInfo> &result)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECT_NAME);
    db.setDatabaseName(DB_NAME);
    db.open();
    QDateTime tmpS(date, QTime(0, 0, 0)), tmpE(date, QTime(23, 59, 59));
    QString sql = QString("SELECT * FROM %1 WHERE mac_addr='%2' AND date>=%3 AND date<=%4;")
            .arg(ALARM_TABLE_NAME).arg(macAddr).arg(tmpS.toSecsSinceEpoch()).arg(tmpE.toSecsSinceEpoch());
    QSqlQuery query(db);
    if(!query.exec(sql)){
        return false;
    }
    while (query.next()) {
        AlarmInfo info;
        info.mac = query.value(0).toString();
        info.ip = query.value(1).toString();
        info.hardware = query.value(2).toString();
        info.datetime = query.value(3).toDateTime();
        info.info = query.value(3).toUInt();
        result.append(info);
    }
    db.close();
    QSqlDatabase::removeDatabase(CONNECT_NAME);
    return true;
}

void HomeTaskThread::run()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    while (!m_isExit) {
        if(m_isPaused){
            QThread::msleep(100);
            continue;
        }else{
            // 舍内热回收器数量
            uint16_t buf12E[1]{0};
            if(handle->readHoldingRegisters(0x012E, 1, buf12E) > 0){
                g_HotCount = buf12E[0];
            }

            // 报警事件
            uint16_t buf2F[2]{0};
            if(handle->readInputRegisters(0x002F, 2, buf2F) > 0){
                emit buf2FR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf2F), std::end(buf2F))));
            }

            // 线圈状态23个寄存器
            uint8_t buf00[23]{0};
            if(handle->readCoilStatus(0, 23, buf00) > 0){
                emit buf00R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf00), std::end(buf00))));
            }

            // 离散寄存器3个
            uint8_t bufDis[3]{0};
            if(handle->readInputStatus(0, 3, bufDis) > 0){
                emit bufDisR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(bufDis), std::end(bufDis))));
            }

            // 二氧化碳开关
            uint16_t buf117[1]{0};
            if(handle->readHoldingRegisters(0x0117, 1, buf117) > 0){
                emit buf117R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf117), std::end(buf117))));
            }

            // 输入寄存器 87个
            uint16_t bufInp[87]{0};
            if(handle->readInputRegisters(0, 87, bufInp) > 0){
                emit bufInpR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(bufInp), std::end(bufInp))));
            }

            // 鸡群数量
            uint16_t buf0F[2]{0};
            if(handle->readHoldingRegisters(0x000F, 2, buf0F) > 0){
                emit buf0FR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf0F), std::end(buf0F))));
            }

            // // 鸡群当前日龄
            uint16_t buf3C[1]{0};
            if(handle->readHoldingRegisters(0x003C, 1, buf3C) > 0){
                emit buf3CR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf3C), std::end(buf3C))));
            }

            // 料线计时开关
            uint16_t buf13A[1]{0};
            if(handle->readHoldingRegisters(0x013A, 1, buf13A) > 0){
                emit buf13AR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf13A), std::end(buf13A))));
            }

            // 料线计时
            uint16_t buf12C[2]{0};
            if(handle->readHoldingRegisters(0x012C, 2, buf12C) > 0){
                emit buf12CR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf12C), std::end(buf12C))));
            }
            // 料线计量
            uint16_t buf133[2]{0};
            if(handle->readHoldingRegisters(0x033, 2, buf133) > 0){
                emit buf133R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf133), std::end(buf133))));
            }

            // 预热T7
            uint16_t buf58[1]{0};
            if(handle->readInputRegisters(0x0058, 1, buf58) > 0){
                emit buf58R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf58), std::end(buf58))));
            }

            // 风量,效率
            uint16_t buf5C[2]{0};
            if(handle->readInputRegisters(0x005C, 2, buf5C) > 0){
                emit buf5CR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf5C), std::end(buf5C))));
            }

            // 回收时间
            uint16_t buf13C[4]{0};
            if(handle->readHoldingRegisters(0x013C, 4, buf13C) > 0){
                emit buf13CR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf13C), std::end(buf13C))));
            }

            // INI0功能选择
            uint16_t buf174[1]{0};
            if(handle->readHoldingRegisters(0x0174, 1, buf174) > 0){
                emit buf174R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf174), std::end(buf174))));
            }

            QThread::msleep(TIMER_UPDATE_INTER);
        }
    }
}


HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage),
    m_pBartonWidget(new BartonWidget(this)),
    m_pRightBartonWidget(new RightBartonWidget(this)),
    m_pHome(new HomeTaskThread)
{
    ui->setupUi(this);

    this->initConnection();

    auto bartonVLayout = new QVBoxLayout();
    bartonVLayout->addWidget(m_pBartonWidget);
    bartonVLayout->addStretch();
    bartonVLayout->setMargin(0);
    ui->widget_barton_page->setLayout(bartonVLayout);
    auto rightVLayout = new QVBoxLayout();
    rightVLayout->addWidget(m_pRightBartonWidget);
    rightVLayout->setMargin(0);
    rightVLayout->setContentsMargins(0, 0, 0, 0);
    ui->widget_right_page->setLayout(rightVLayout);

    // 主页的警告信息滚动控件
    ui->widget_alarm->setMouseHoverStop(true);
    ui->widget_alarm->setText("");
    ui->widget_alarm->setBackground(QColor(0, 0, 0, 0));
    ui->widget_alarm->setForeground(QColor(240, 0, 0));
    ui->widget_alarm->setMoveStyle(TextMoveWidget::MoveStyle_RightToLeft);

    this->m_pRightBartonWidget->setLx(false);
}

HomePage::~HomePage()
{
    delete ui;
    if(m_pHome != nullptr && m_pHome->isRunning()){
        m_pHome->exitThread();
        if(m_pHome->isRunning()){
            m_pHome->terminate();
        }
    }
}
void HomePage::isLx(bool status)
{
    this->m_pRightBartonWidget->setLx(status);
}

void HomePage::initConnection()
{
    connect(m_pHome.get(), &HomeTaskThread::buf2FR, this, [this](UInt16_T buf){
        this->dumpAlarm(buf.buf.at(0), buf.buf.at(1));
    });
    connect(m_pHome.get(), &HomeTaskThread::buf00R, this, [this](UInt16_T buf){
        // 7个风机
        for (int i=0; i<7; ++i) {
            m_pBartonWidget->setFanPaused(i, static_cast<bool>(buf.buf[i]));
        }
        m_pBartonWidget->setHotPaused(static_cast<bool>(buf.buf[7]));
        m_pBartonWidget->setBartonPaused(static_cast<bool>(buf.buf[7]));
        m_pRightBartonWidget->setHotValue(static_cast<bool>(buf.buf[10]), static_cast<bool>(buf.buf[11]), static_cast<bool>(buf.buf[12]));
        m_pRightBartonWidget->setWetPaused(static_cast<bool>(buf.buf[13]));
        m_pRightBartonWidget->setLightValue(static_cast<bool>(buf.buf[21]));
        m_pRightBartonWidget->setSendHotWater(static_cast<bool>(buf.buf[22]));
    });
    connect(m_pHome.get(), &HomeTaskThread::bufDisR, this, [this](UInt16_T buf){
        m_pRightBartonWidget->setNewWindValue(static_cast<bool>(buf.buf[2]));
    });
    connect(m_pHome.get(), &HomeTaskThread::buf117R, this, [this](UInt16_T buf){
        m_co2flag = static_cast<bool>(buf.buf[0]);
        m_pBartonWidget->setCO2(m_co2flag);
    });
    connect(m_pHome.get(), &HomeTaskThread::bufInpR, this, [this](UInt16_T buf){
        m_pRightBartonWidget->setIndoorT(buf.buf[0]);
        m_pBartonWidget->m_pLeftWidget->setAimT(buf.buf[1]);
        m_pRightBartonWidget->setHumidityValue(buf.buf[2]);
        m_pBartonWidget->m_pLeftWidget->setInPa(buf.buf[3]);
        if(m_co2flag){
            m_pBartonWidget->setCO2Value(buf.buf[4]);
        }
        m_pBartonWidget->m_pLeftWidget->setRunStatus(static_cast<bool>(buf.buf[5]));
        m_pBartonWidget->setT1ToT4Value(buf.buf[6], buf.buf[7], buf.buf[8], buf.buf[9]);
        m_pRightBartonWidget->setT5ToT6Value(buf.buf[10], buf.buf[11]);
        m_pBartonWidget->m_pLeftWidget->setAirLevel(buf.buf[12]);
        m_pBartonWidget->m_pLeftWidget->setAirLevelLimit(buf.buf[13], buf.buf[14]);
        m_pRightBartonWidget->setRH11Value(buf.buf[17]);
        m_pRightBartonWidget->setRH03RH13Value(buf.buf[20], buf.buf[19]);
        m_pRightBartonWidget->setRH02RH12Value(buf.buf[21], buf.buf[56]);
        m_pRightBartonWidget->setSWValue(buf.buf[55], buf.buf[22]);
        m_pRightBartonWidget->setFBlValue(buf.buf[86], buf.buf[23], buf.buf[24]);
        m_pRightBartonWidget->setWetTime(buf.buf[59], buf.buf[60]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf0FR, this, [this](UInt16_T buf){
        m_pBartonWidget->m_pLeftWidget->setBreadNums(buf.buf[0], buf.buf[1]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf3CR, this, [this](UInt16_T buf){
        m_pBartonWidget->m_pLeftWidget->setCurrntAge(buf.buf[0]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf13AR, this, [this](UInt16_T buf){
        m_lxflag = static_cast<bool>(buf.buf[0]);
        this->isLx(m_lxflag);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf12CR, this, [this](UInt16_T buf){
        if(!m_lxflag){return ;}
        m_pRightBartonWidget->setLXTValue(buf.buf[0], buf.buf[1]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf133R, this, [this](UInt16_T buf){
        if(!m_lxflag){return ;}
        m_pRightBartonWidget->setLXVValue(buf.buf[0], buf.buf[1]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf58R, this, [this](UInt16_T buf){
        m_pRightBartonWidget->setT7Value(buf.buf[0]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf5CR, this, [this](UInt16_T buf){
        m_pRightBartonWidget->setFLLVValue(buf.buf[0], buf.buf[1]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf13CR, this, [this](UInt16_T buf){
        m_pRightBartonWidget->setRecoveryTimeVol(buf.buf[0], buf.buf[1], buf.buf[2], buf.buf[3]);
    });
    connect(m_pHome.get(), &HomeTaskThread::buf174R, this, [this](UInt16_T buf){
        m_pBartonWidget->setNH3Value(buf.buf[0]);
    });
}

void HomePage::addAlarmInfo(QString info)
{
    QString value = ui->widget_alarm->getText();
    if(value == info){ return; }
    ui->widget_alarm->setPaused(true);
    ui->widget_alarm->setText(value + info);
    ui->widget_alarm->setPaused(false);
}
void HomePage::dumpAlarm(uint code1, uint code2)
{
    if(code1 == 0 || code2 == 0){
        addAlarmInfo("");
    }
    QStringList buf;
    for(int i=0; i < 16; i++){
        if (code1 & (1<<i)){
            buf.append(ERROR_INFO.at(i));
        }
        if (code2 & (1<<i)){
            buf.append(ERROR_INFO.at(i+16));
        }

    }
    if(m_alarmInfo != buf.join(" ")){
        m_alarmInfo = buf.join(" ");
        addAlarmInfo(m_alarmInfo);
    }
}
