#include <QDebug>
#include "parsetfour.h"
#include "customdialog.h"
#include "ui_parsetfour.h"

ParSetFour::ParSetFour(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParSetFour),
    m_pFour(new FourTaskThread)
{
    ui->setupUi(this);
    this->initConnection();
    ui->doubleSpinBox_deviation_u->setRange(1, 20);
    ui->doubleSpinBox_deviation_d->setRange(-5, -1);
    ui->doubleSpinBox_humidity_u->setRange(60, 99);
    ui->doubleSpinBox_humidity_d->setRange(20, 99);
    ui->doubleSpinBox_pa_1_2_u->setRange(1, 99);
    ui->doubleSpinBox_pa_1_2_d->setRange(-99, -1);
    ui->doubleSpinBox_pa_3_16_u->setRange(-99, 99);
    ui->doubleSpinBox_pa_3_16_d->setRange(-99, -1);
    ui->spinBox_error_swtich_delay->setRange(10, 99);
    ui->spinBox_alarm_delay->setRange(10, 120);
}

ParSetFour::~ParSetFour()
{
    delete ui;
    if(m_pFour != nullptr && m_pFour->isRunning()){
        m_pFour->exitThread();
        if(m_pFour->isRunning()){
            m_pFour->terminate();
        }
    }
}

void ParSetFour::initConnection()
{
    connect(m_pFour.get(), &FourTaskThread::buf2ER, this, [this](QVector<uint16_t> buf){
        ui->spinBox_in_vol->setValue(static_cast<int16_t>(buf[0]));
        ui->spinBox_out_vol->setValue(static_cast<int16_t>(buf[1]));
    });
    connect(m_pFour.get(), &FourTaskThread::buf132R, this, [this](uint16_t buf){
        ui->spinBox_min_open_time->setValue(buf);
    });
    connect(m_pFour.get(), &FourTaskThread::buf41R, this, [this](uint16_t buf){
        ui->spinBox_control_time->setValue(buf);
    });
    connect(m_pFour.get(), &FourTaskThread::buf12AR, this, [this](uint16_t buf){
        ui->doubleSpinBox_heating_t->setValue(static_cast<int16_t>(buf) / 100.00);
    });
    connect(m_pFour.get(), &FourTaskThread::buf12ER, this, [this](uint16_t buf){
        ui->spinBox_hoter_count->setValue(buf);
        g_HotCount = static_cast<uint16_t>(ui->spinBox_hoter_count->value());
    });
    connect(m_pFour.get(), &FourTaskThread::buf11AR, this, [this](QVector<uint16_t> buf){
        ui->doubleSpinBox_antifreeze1->setValue(static_cast<int16_t>(buf[0]) / 100.00);
        ui->doubleSpinBox_antifreeze2->setValue(static_cast<int16_t>(buf[1]) / 100.00);
        ui->doubleSpinBox_antifreeze3->setValue(static_cast<int16_t>(buf[2]) / 100.00);
    });
    connect(m_pFour.get(), &FourTaskThread::bufD1R, this, [this](QVector<uint16_t> buf){
        ui->doubleSpinBox_deviation_u->setValue(static_cast<int16_t>(buf[0]) / 100.00);
        ui->doubleSpinBox_deviation_d->setValue(static_cast<int16_t>(buf[1]) / 100.00);
        ui->doubleSpinBox_humidity_u->setValue(static_cast<int16_t>(buf[2]) / 10.0);
        ui->doubleSpinBox_humidity_d->setValue(static_cast<int16_t>(buf[3]) / 10.0);
        ui->spinBox_co2_u->setValue(buf[4]);
        ui->doubleSpinBox_pa_1_2_u->setValue(static_cast<int16_t>(buf[5]) / 10.0);
        ui->doubleSpinBox_pa_1_2_d->setValue(static_cast<int16_t>(buf[6]) / 10.0);
        ui->doubleSpinBox_pa_3_16_u->setValue(static_cast<int16_t>(buf[7]) / 10.0);
        ui->doubleSpinBox_pa_3_16_d->setValue(static_cast<int16_t>(buf[8]) / 10.0);
        ui->spinBox_error_swtich_delay->setValue(buf[9]);
    });
    connect(m_pFour.get(), &FourTaskThread::bufFCR, this, [this](uint16_t buf){
        ui->spinBox_alarm_delay->setValue(buf);
    });
}

void ParSetFour::updatePageData(bool flag)
{
    m_pFour->setPaused(!flag);
    this->setFocus(Qt::MouseFocusReason);
}

void FourTaskThread::run()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    while (!m_isExit) {
        if(m_isPaused){
            QThread::msleep(100);
            continue;
        }else{
            // 热回收机组送风机风量, 热回收机组排风机风量
            uint16_t buf2E[2]{0};
            if(handle->readHoldingRegisters(0x002E, 2, buf2E) > 0){
                emit buf2ER(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf2E), std::end(buf2E))));
            }

            // 热回收送风机最短开启时间
            uint16_t buf132{0};
            if(handle->readHoldingRegisters(0x0132, 1, &buf132) > 0){
                emit buf132R(buf132);
            }

            // 变频器和比例阀控制时间间隔
            uint16_t buf41{0};
            if(handle->readHoldingRegisters(0x0041, 1, &buf41) > 0){
                emit buf41R(buf41);
            }

            // 排水电加热启动温度
            uint16_t buf12A{0};
            if(handle->readHoldingRegisters(0x012A, 1, &buf12A) > 0){
                emit buf12AR(buf12A);
            }

            // 舍内热回收器数量
            uint16_t buf12E{0};
            if(handle->readHoldingRegisters(0x012E, 1, &buf12E) > 0){
                emit buf12ER(buf12E);
            }

            // 防冻温度1,2,3
            uint16_t buf11A[3]{0};
            if(handle->readHoldingRegisters(0x011A, 3, buf11A) > 0){
                emit buf11AR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf11A), std::end(buf11A))));
            }

            // 舍内温度报警偏差上限下限, 舍内湿度报警上限下限, 二氧化碳浓度上限
            // 通风级别1-2下舍内压力报警上限下限, 通风级别3-16下舍内压力报警上限下限
            // 热回收机组运行故障后切换为纵向风机的延时时间
            uint16_t bufD1[10]{0};
            if(handle->readHoldingRegisters(0x00D1, 10, bufD1) > 0){
                emit bufD1R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(bufD1), std::end(bufD1))));
            }

            // 报警延时时间限制
            uint16_t bufFC{0};
            if(handle->readHoldingRegisters(0x00FC, 1, &bufFC) > 0){
                emit bufFCR(bufFC);
            }
            QThread::msleep(TIMER_UPDATE_INTER);
        }
    }
}

void ParSetFour::on_spinBox_in_vol_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x002E, static_cast<uint16_t>(ui->spinBox_in_vol->value()));
}

void ParSetFour::on_spinBox_out_vol_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x002F, static_cast<uint16_t>(ui->spinBox_out_vol->value()));
}

void ParSetFour::on_spinBox_min_open_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0132, static_cast<uint16_t>(ui->spinBox_min_open_time->value()));
}

void ParSetFour::on_spinBox_control_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0041, static_cast<uint16_t>(ui->spinBox_control_time->value()));
}

void ParSetFour::on_doubleSpinBox_heating_t_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_heating_t->value() * 100;
    handle->writeSingleRegister(0x012A, static_cast<uint16_t>(vol));
}

void ParSetFour::on_spinBox_hoter_count_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x012E, static_cast<uint16_t>(ui->spinBox_hoter_count->value()));
    g_HotCount = static_cast<uint16_t>(ui->spinBox_hoter_count->value());
}

void ParSetFour::on_doubleSpinBox_antifreeze1_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_antifreeze1->value() * 100;
    handle->writeSingleRegister(0x011A, static_cast<uint16_t>(vol));
}

void ParSetFour::on_doubleSpinBox_antifreeze2_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_antifreeze2->value() * 100;
    handle->writeSingleRegister(0x011B, static_cast<uint16_t>(vol));
}

void ParSetFour::on_doubleSpinBox_antifreeze3_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_antifreeze3->value() * 100;
    handle->writeSingleRegister(0x011C, static_cast<uint16_t>(vol));
}

void ParSetFour::on_pushButton_vol_clear_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x1CB8, 0x0005);
}

void ParSetFour::on_doubleSpinBox_deviation_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_deviation_u->value() * 100;
    handle->writeSingleRegister(0x0D1, static_cast<uint16_t>(vol));
}

void ParSetFour::on_doubleSpinBox_deviation_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_deviation_d->value() * 100;
    handle->writeSingleRegister(0x0D2, static_cast<uint16_t>(vol));
}

void ParSetFour::on_doubleSpinBox_humidity_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_humidity_u->value() * 10;
    handle->writeSingleRegister(0x0D3, static_cast<uint16_t>(vol));
}

void ParSetFour::on_doubleSpinBox_humidity_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_humidity_d->value() * 10;
    handle->writeSingleRegister(0x0D4, static_cast<uint16_t>(vol));
}

void ParSetFour::on_spinBox_co2_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0D5, static_cast<uint16_t>(ui->spinBox_co2_u->value()));
}

void ParSetFour::on_doubleSpinBox_pa_1_2_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0D6, static_cast<uint16_t>(ui->doubleSpinBox_pa_1_2_u->value()));
}

void ParSetFour::on_doubleSpinBox_pa_1_2_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0D7, static_cast<uint16_t>(ui->doubleSpinBox_pa_1_2_d->value()));
}

void ParSetFour::on_doubleSpinBox_pa_3_16_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0D8, static_cast<uint16_t>(ui->doubleSpinBox_pa_3_16_u->value()));
}

void ParSetFour::on_doubleSpinBox_pa_3_16_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0D9, static_cast<uint16_t>(ui->doubleSpinBox_pa_3_16_d->value()));
}

void ParSetFour::on_spinBox_alarm_delay_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x00FC, static_cast<uint16_t>(ui->spinBox_alarm_delay->value()));
}

void ParSetFour::on_spinBox_error_swtich_delay_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0DA, static_cast<uint16_t>(ui->spinBox_error_swtich_delay->value()));
}
