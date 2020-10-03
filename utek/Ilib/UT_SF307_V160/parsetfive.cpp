#include "parsetfive.h"
#include "ui_parsetfive.h"
#include "customdialog.h"

ParSetFive::ParSetFive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParSetFive),
    m_pFive(new FiveTaskThread)
{
    ui->setupUi(this);
    this->initConnection();
    for (int i=0; i<7; ++i) {
        auto doubleSpin = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_t%1").arg(i+1));
        doubleSpin->setRange(-99.99, 99.99);
    }
    ui->spinBox_wc_circle_time->setRange(60, 300);
    ui->doubleSpinBox_hot_o_d->setRange(-5, 0);
    ui->doubleSpinBox_feed_vol->setRange(0.1, 100);
    ui->spinBox_sw_no->setRange(0, 9);
    ui->spinBox_bp_no->setRange(0, 9);
    ui->spinBox_fp_no->setRange(0, 9);
    ui->spinBox_fpt_no->setRange(0, 9);
    ui->doubleSpinBox_wc_o_t->setRange(25, 35);
    ui->spinBox_wc_o_time->setRange(10, 300);
    ui->spinBox_wc_circle_time->setRange(60, 300);
    ui->spinBox_wc_min_level->setRange(8, 16);
    ui->doubleSpinBox_wc_deviation->setRange(2, 99);
}

ParSetFive::~ParSetFive()
{
    delete ui;
    if(m_pFive != nullptr && m_pFive->isRunning()){
        m_pFive->exitThread();
        if(m_pFive->isRunning()){
            m_pFive->terminate();
        }
    }
}

void ParSetFive::updatePageData(bool flag)
{
    m_pFive->setPaused(!flag);
    this->setFocus(Qt::MouseFocusReason);
}

void ParSetFive::initConnection()
{
    connect(m_pFive.get(), &FiveTaskThread::bufB7R, this, [this](QVector<uint16_t> buf){
        ui->doubleSpinBox_sw0->setValue(buf[0] / 100.00);
        ui->doubleSpinBox_sw100->setValue(buf[1] / 100.00);
        ui->spinBox_sw_no->setValue(buf[2]);
        ui->doubleSpinBox_fp0->setValue(buf[3] / 100.00);
        ui->doubleSpinBox_fp100->setValue(buf[4] / 100.00);
        ui->spinBox_fp_no->setValue(buf[5]);
        ui->doubleSpinBox_fpt0->setValue(buf[6] / 100.00);
        ui->doubleSpinBox_fpt100->setValue(buf[7] / 100.00);
        ui->spinBox_fpt_no->setValue(buf[8]);
        ui->doubleSpinBox_bp0->setValue(buf[9] / 100.00);
        ui->doubleSpinBox_bp100->setValue(buf[10] / 100.00);
        ui->spinBox_bp_no->setValue(buf[11]);
    });
    connect(m_pFive.get(), &FiveTaskThread::bufC3R, this, [this](QVector<uint16_t> buf){
        ui->doubleSpinBox_wc_o_t->setValue(buf[0] / 100.00);
        ui->spinBox_wc_o_time->setValue(buf[1]);
        ui->spinBox_wc_circle_time->setValue(buf[2]);
        ui->doubleSpinBox_hot_o_d->setValue(static_cast<int16_t>(buf[3]) / 100.00);
        ui->doubleSpinBox_hot_c_d->setValue(static_cast<int16_t>(buf[4]) / 100.00);
        ui->doubleSpinBox_t1->setValue(static_cast<int16_t>(buf[5]) / 100.00);
        ui->doubleSpinBox_t2->setValue(static_cast<int16_t>(buf[6]) / 100.00);
        ui->doubleSpinBox_t3->setValue(static_cast<int16_t>(buf[7]) / 100.00);
        ui->doubleSpinBox_t4->setValue(static_cast<int16_t>(buf[8]) / 100.00);
        ui->doubleSpinBox_t5->setValue(static_cast<int16_t>(buf[9]) / 100.00);
        ui->doubleSpinBox_t6->setValue(static_cast<int16_t>(buf[10]) / 100.00);
        ui->doubleSpinBox_humidity->setValue(static_cast<int16_t>(buf[11]) / 10.0);
        ui->doubleSpinBox_pa->setValue(static_cast<int16_t>(buf[12]) / 10.00);
        ui->doubleSpinBox_co2->setValue(static_cast<int16_t>(buf[13]));
    });
    connect(m_pFive.get(), &FiveTaskThread::bufDFR, this, [this](QVector<uint16_t> buf){
        ui->spinBox_wc_min_level->setValue(buf[0]);
        ui->doubleSpinBox_wc_deviation->setValue(static_cast<int16_t>(buf[1]) / 100.00);
    });
    connect(m_pFive.get(), &FiveTaskThread::buf11DR, this, [this](QVector<uint16_t> buf){
        ui->timeEdit_1_s->setTime(QTime(buf[0] / 100, buf[0] % 100));
        ui->timeEdit_1_c->setTime(QTime(buf[1] / 100, buf[1] % 100));
        ui->timeEdit_2_s->setTime(QTime(buf[2] / 100, buf[2] % 100));
        ui->timeEdit_2_c->setTime(QTime(buf[3] / 100, buf[3] % 100));
        ui->timeEdit_3_s->setTime(QTime(buf[4] / 100, buf[4] % 100));
        ui->timeEdit_3_c->setTime(QTime(buf[5] / 100, buf[5] % 100));
        ui->timeEdit_4_s->setTime(QTime(buf[6] / 100, buf[6] % 100));
        ui->timeEdit_4_c->setTime(QTime(buf[7] / 100, buf[7] % 100));
    });
    connect(m_pFive.get(), &FiveTaskThread::buf135R, this, [this](QVector<uint16_t> buf){
        ui->doubleSpinBox_feed_vol->setValue(static_cast<int16_t>(buf[0]) / 100.00);
        ui->doubleSpinBox_T7_u->setValue(static_cast<int16_t>(buf[1]) / 100.00);
        ui->doubleSpinBox_T7_d->setValue(static_cast<int16_t>(buf[2]) / 100.00);
        ui->doubleSpinBox_t7->setValue(static_cast<int16_t>(buf[3]) / 100.00);
    });
    connect(m_pFive.get(), &FiveTaskThread::buf33R, this, [this](QVector<uint16_t> buf){
        ui->label_sw_f_v->setNum(static_cast<int16_t>(buf[0]) / 100.00);
        ui->label_fp_f_v->setNum(static_cast<int16_t>(buf[1]) / 100.00);
        ui->label_fpt_f_v->setNum(static_cast<int16_t>(buf[2]) / 100.00);
        ui->label_bp_f_v->setNum(static_cast<int16_t>(buf[3]) / 100.00);
    });
    connect(m_pFive.get(), &FiveTaskThread::buf16R, this, [this](QVector<uint16_t> buf){
        ui->label_sw_f_r->setNum(static_cast<int16_t>(buf[0]));
        ui->label_fp_f_r->setNum(static_cast<int16_t>(buf[1]));
        ui->label_fpt_f_r->setNum(static_cast<int16_t>(buf[2]));
        ui->label_bp_f_r->setNum(static_cast<int16_t>(buf[3]));
    });
    connect(m_pFive.get(), &FiveTaskThread::buf16ER, this, [this](QVector<uint16_t> buf){
        ui->doubleSpinBox_wc_c_t->setValue(static_cast<int16_t>(buf[0]) / 100.00);
        ui->spinBox_co2_d->setValue(buf[1]);
        ui->spinBox_co2_u->setValue(buf[2]);
        ui->spinBox_illumination->setValue(buf[3]);
        ui->spinBox_gradual_time->setValue(buf[4]);
    });
    connect(m_pFive.get(), &FiveTaskThread::buf175R, this, [this](QVector<uint16_t> buf){
        ui->spinBox_nh3_d->setValue(buf[0]);
        ui->spinBox_nh3_u->setValue(buf[1]);
    });
    this->initTConnect();
    this->initLightConnect();
}

void ParSetFive::initTConnect()
{
    for (int i=0; i<6; ++i) {
        auto doubleSpin = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_t%1").arg(i+1));
        connect(doubleSpin, &QDoubleSpinBox::editingFinished, this, [this, doubleSpin, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            double vol = doubleSpin->value() * 100;
            handle->writeSingleRegister(0x00C8 + i, static_cast<uint16_t>(vol));
        });
    }
}
void ParSetFive::initLightConnect()
{
    for (int i=0; i<4; ++i) {
        auto start = this->findChild<QTimeEdit*>(QString("timeEdit_%1_s").arg(i+1));
        auto close = this->findChild<QTimeEdit*>(QString("timeEdit_%1_c").arg(i+1));
        connect(start, &QTimeEdit::editingFinished, this, [this, start, i](){
            double vol = start->text().replace(2, 1, '.').toDouble() * 100;
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            handle->writeSingleRegister(0x011D + i*2, static_cast<uint16_t>(vol));
        });
        connect(close, &QTimeEdit::editingFinished, this, [this, close, i](){
            double vol = close->text().replace(2, 1, '.').toDouble() * 100;
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            handle->writeSingleRegister(0x011E + i*2, static_cast<uint16_t>(vol));
        });
    }
}

void FiveTaskThread::run()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    while (!m_isExit) {
        if(m_isPaused){
            QThread::msleep(100);
            continue;
        }else{
            // 旁通，侧窗，翻板设置
            uint16_t bufB7[12]{0};
            if(handle->readHoldingRegisters(0x00B7, 12, bufB7) > 0){
                emit bufB7R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(bufB7), std::end(bufB7))));
            }

            // 传感器补偿校准, 湿帘启动温度, 湿帘最短开启时间, 湿帘循环时间
            uint16_t bufC3[14]{0};
            if(handle->readHoldingRegisters(0x00C3, 14, bufC3) > 0){
                emit bufC3R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(bufC3), std::end(bufC3))));
            }

            // 湿帘设置, 加热设置
            uint16_t bufDF[2]{0};
            if(handle->readHoldingRegisters(0x00DF, 2, bufDF) > 0){
                emit bufDFR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(bufDF), std::end(bufDF))));
            }

            // 光照设置
            uint16_t buf11D[8]{0};
            if(handle->readHoldingRegisters(0x011D, 8, buf11D) > 0){
                emit buf11DR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf11D), std::end(buf11D))));
            }

            // 料线设置， 传感器T7补偿， 量程
            uint16_t buf135[4]{0};
            if(handle->readHoldingRegisters(0x0135, 4, buf135) > 0){
                emit buf135R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf135), std::end(buf135))));
            }

            // 旁通侧窗翻板反馈电阻值
            uint16_t buf33[4]{0};
            if(handle->readInputRegisters(0x0033, 4, buf33) > 0){
                emit buf33R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf33), std::end(buf33))));
            }

            // 旁通侧窗翻板反馈
            uint16_t buf16[4]{0};
            if(handle->readInputRegisters(0x0016, 4, buf16) > 0){
                emit buf16R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf16), std::end(buf16))));
            }

            // 湿帘关闭舍内温度, 二氧化碳量程下限，上限, 光照强度，光照渐变时间
            uint16_t buf16E[5]{0};
            if(handle->readHoldingRegisters(0x016E, 5, buf16E) > 0){
                emit buf16ER(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf16E), std::end(buf16E))));
            }

            // NH3量程上下限
            uint16_t buf175[2]{0};
            if(handle->readHoldingRegisters(0x0175, 2, buf175) > 0){
                emit buf175R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf175), std::end(buf175))));
            }

            QThread::msleep(TIMER_UPDATE_INTER);
        }
    }
}

void ParSetFive::on_doubleSpinBox_sw0_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_sw0->value() * 100;
    handle->writeSingleRegister(0x00B7, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_sw100_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_sw100->value() * 100;
    handle->writeSingleRegister(0x00B8, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_sw_no_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_sw_no->value();
    handle->writeSingleRegister(0x00B9, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_fp0_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_fp0->value() * 100;
    handle->writeSingleRegister(0x00BA, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_fp100_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_fp100->value() * 100;
    handle->writeSingleRegister(0x00BB, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_fp_no_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_fp_no->value();
    handle->writeSingleRegister(0x00BC, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_fpt0_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_fpt0->value() * 100;
    handle->writeSingleRegister(0x00BD, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_fpt100_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_fpt100->value() * 100;
    handle->writeSingleRegister(0x00BE, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_fpt_no_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_fpt_no->value();
    handle->writeSingleRegister(0x00BF, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_bp0_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_bp0->value() * 100;
    handle->writeSingleRegister(0x00C0, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_bp100_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_bp100->value() * 100;
    handle->writeSingleRegister(0x00C1, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_bp_no_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_bp_no->value();
    handle->writeSingleRegister(0x00C2, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_t7_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_t7->value() * 100;
    handle->writeSingleRegister(0x0138, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_humidity_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_humidity->value() * 10;
    handle->writeSingleRegister(0x00CE, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_pa_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_pa->value() * 10;
    handle->writeSingleRegister(0x00CF, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_co2_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_co2->value();
    handle->writeSingleRegister(0x00D0, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_T7_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_T7_u->value() * 100;
    handle->writeSingleRegister(0x0136, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_T7_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_T7_d->value() * 100;
    handle->writeSingleRegister(0x0137, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_co2_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_co2_u->value();
    handle->writeSingleRegister(0x016F, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_co2_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_co2_d->value();
    handle->writeSingleRegister(0x0170, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_wc_o_t_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    double vol = ui->doubleSpinBox_wc_o_t->value() * 100;
    handle->writeSingleRegister(0x00C3, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_wc_o_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_wc_o_time->value();
    if(vol > ui->spinBox_wc_circle_time->value()){
        // 参数验证不合格
        CustomInfoBox::customInfoBox(tr("湿帘设置"), tr("湿帘最短开启时间不能大于湿帘循环时间"));
        return ;
    }
    handle->writeSingleRegister(0x00C4, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_wc_circle_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_wc_circle_time->value();
    handle->writeSingleRegister(0x00C5, static_cast<uint16_t>(vol));
}

void ParSetFive::on_spinBox_wc_min_level_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int vol = ui->spinBox_wc_min_level->value();
    handle->writeSingleRegister(0x00DF, static_cast<uint16_t>(vol));
}

void ParSetFive::on_doubleSpinBox_wc_c_t_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        double vol = ui->doubleSpinBox_wc_c_t->value() * 100;
        handle->writeSingleRegister(0x016E, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_doubleSpinBox_wc_deviation_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        double vol = ui->doubleSpinBox_wc_deviation->value() * 100;
        handle->writeSingleRegister(0x00E0, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_doubleSpinBox_hot_o_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        double vol = ui->doubleSpinBox_hot_o_d->value() * 100;
        handle->writeSingleRegister(0x00C6, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_doubleSpinBox_hot_c_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        double vol = ui->doubleSpinBox_hot_c_d->value() * 100;
        double volO = ui->doubleSpinBox_hot_o_d->value() * 100;
        if(vol < volO + 10){
            // 参数验证不合格
            CustomInfoBox::customInfoBox(tr("加热设置"), tr("加热关闭偏差不能小于等于加热启动偏差"));
            return ;
        }
        handle->writeSingleRegister(0x00C7, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_spinBox_illumination_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        int vol = ui->spinBox_illumination->value();
        handle->writeSingleRegister(0x0171, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_doubleSpinBox_feed_vol_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        double vol = ui->doubleSpinBox_feed_vol->value() * 100;
        handle->writeSingleRegister(0x0135, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_pushButton_feed_clear_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x1CB8, 0x0004);
    }
}

void ParSetFive::on_spinBox_gradual_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        int vol = ui->spinBox_gradual_time->value();
        handle->writeSingleRegister(0x0172, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_spinBox_nh3_d_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        int vol = ui->spinBox_nh3_d->value();
        handle->writeSingleRegister(0x0175, static_cast<uint16_t>(vol));
    }
}

void ParSetFive::on_spinBox_nh3_u_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        int vol = ui->spinBox_nh3_u->value();
        handle->writeSingleRegister(0x0176, static_cast<uint16_t>(vol));
    }
}
