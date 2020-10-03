#include <QDebug>
#include "parsetthree.h"
#include "ui_parsetthree.h"
#include "customdialog.h"

ParSetThree::ParSetThree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParSetThree),
    m_pThree(new ThreeTaskThread)
{
    ui->setupUi(this);
    this->initConnection();
    ui->spinBox_add_delay->setRange(30, 200);
    ui->spinBox_sub_delay->setRange(30, 200);
    for (int i=1; i<16; ++i) {
        auto deviation = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_deviation%1").arg(i+1));
        if(deviation){
            deviation->setRange(0.2, 9.9);
        }
    }
    for (int i=3; i<=10; ++i) {
        auto sw = this->findChild<QSpinBox*>(QString("spinBox_sw%1").arg(i));
        if(sw){
            sw->setRange(0, 99);
        }
    }
    for (int i=5; i<=15; ++i) {
        auto fp = this->findChild<QSpinBox*>(QString("spinBox_fp%1").arg(i));
        if(fp){
            fp->setRange(0, 100);
        }
    }
}

ParSetThree::~ParSetThree()
{
    delete ui;
    if(m_pThree != nullptr && m_pThree->isRunning()){
        m_pThree->exitThread();
        if(m_pThree->isRunning()){
            m_pThree->terminate();
        }
    }
}

void ParSetThree::initConnection()
{
    connect(m_pThree.get(), &ThreeTaskThread::buf3FR, this, [this](QVector<uint16_t> buf){
        ui->spinBox_add_delay->setValue(buf[0]);
        ui->spinBox_sub_delay->setValue(buf[1]);
    });
    connect(m_pThree.get(), &ThreeTaskThread::buf130R, this, [this](QVector<uint16_t> buf){
        ui->comboBox_air_model->setCurrentIndex(buf[0]);
        ui->spinBox_open_close_time->setValue(buf[1]);
    });
    connect(m_pThree.get(), &ThreeTaskThread::buf57R, this, [this](QVector<uint16_t> buf){
        for (int i=0; i<16; ++i) {
            // 温度偏差
            if(i==0){
                ui->label_deviation->setNum(buf[0] / 100.00);
            }else{
                auto offset = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_deviation%1").arg(i+1));
                offset->setValue(buf[i*6] / 100.00);
            }
            auto sw = this->findChild<QSpinBox*>(QString("spinBox_sw%1").arg(i+1));
            if(sw != nullptr){
                sw->setValue(buf[i*6 + 4]);
            }
            auto fp = this->findChild<QSpinBox*>(QString("spinBox_fp%1").arg(i+1));
            if(fp != nullptr){
                fp->setValue(buf[i*6 + 5]);
            }
        }
    });
    connect(m_pThree.get(), &ThreeTaskThread::buf1BR, this, [this](QVector<uint16_t> buf){
        for (int i=0; i<16; ++i) {
            auto lable = this->findChild<QLabel*>(QString("label_o_a_v%1").arg(i+1));
            if(lable != nullptr){
                lable->setNum(buf[i]);
            }
            ui->label_o_a_v1_1->setNum(buf[0]);
            ui->label_o_a_v1_2->setNum(buf[1]);
            ui->label_o_a_v3_1->setNum(buf[2]);
            // Tip: label_o_a_v3_2 的数据在 0x0054寄存器中，为第三级别全开风量，和buf44R信号中读取
        }
    });
    connect(m_pThree.get(), &ThreeTaskThread::buf44R, this, [this](QVector<uint16_t> buf){
        for (int i=0; i<16; ++i) {
            auto label = this->findChild<QLabel*>(QString("label_a_rate%1").arg(i+1));
            label->setNum(buf[i]);
        }
        ui->label_o_a_v3_2->setNum(buf[buf.length()-1]);
    });

    this->initDeviationConnect();
    this->initSideWindowConnect();
    this->initFlipPlateConnect();
}

void ParSetThree::initDeviationConnect()
{
    for (int i=1; i<16; ++i) {
        auto deviation = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_deviation%1").arg(i+1));
        connect(deviation, &QDoubleSpinBox::editingFinished, this, [this, deviation, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            if(i > 1){
                auto pre = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_deviation%1").arg(i));
                if(deviation->value() < pre->value() + 0.1){
                    // 参数验证不合格
                    CustomInfoBox::customInfoBox(tr("温度偏差设置"), tr("温度偏差%1不能小于等于温度偏差%2").arg(i+1).arg(i));
                    return ;
                }
            }
            double vol = deviation->value() * 100;
            handle->writeSingleRegister(0x0057 + i*6, static_cast<uint16_t>(vol));
        });
    }
}

void ParSetThree::initSideWindowConnect()
{
    for (int i=2; i<10; ++i) {
        auto sw = this->findChild<QSpinBox*>(QString("spinBox_sw%1").arg(i+1));
        connect(sw, &QSpinBox::editingFinished, this, [this, sw, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            handle->writeSingleRegister(0x005B + i*6, static_cast<uint16_t>(sw->value()));
        });
    }
}

void ParSetThree::initFlipPlateConnect()
{
    for (int i=4; i<=15; ++i) {
        auto fp = this->findChild<QSpinBox*>(QString("spinBox_fp%1").arg(i+1));
        connect(fp, &QSpinBox::editingFinished, this, [this, fp, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            if(i > 4){
                auto pre = this->findChild<QSpinBox*>(QString("spinBox_fp%1").arg(i));
                if(fp->value() < pre->value() + 1){
                    // 参数验证不合格
                    CustomInfoBox::customInfoBox(tr("翻板设置"), tr("翻板%1不能小于等于翻板%2").arg(i+1).arg(i));
                    return ;
                }
            }
            handle->writeSingleRegister(0x005C + i*6, static_cast<uint16_t>(fp->value()));
        });
    }
}

void ParSetThree::updatePageData(bool flag)
{
    m_pThree->setPaused(!flag);
    this->setFocus(Qt::MouseFocusReason);
}


void ThreeTaskThread::run()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    while (!m_isExit) {
        if(m_isPaused){
            QThread::msleep(100);
            continue;
        }else{
            // 风级级别增大延时时间, 风级级别减小延时时间
            uint16_t buf3F[2]{0};
            if(handle->readHoldingRegisters(0x003F, 2, buf3F) > 0){
                emit buf3FR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf3F), std::end(buf3F))));
            }

            // 通风模式， 通风优先时组合一风机最短开启或关闭时间
            uint16_t buf130[2]{0};
            if(handle->readHoldingRegisters(0x0130, 2, buf130) > 0){
                emit buf130R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf130), std::end(buf130))));
            }

            // 通风级别温度偏差, 排风设备, 进风设备--热回收旁通阀%,
            // 进风设备--热水比例阀%, 进风设备--侧窗%, 进风设备--翻板%
            uint16_t buf57[96]{0};
            if(handle->readHoldingRegisters(0x0057, 96, buf57) > 0){
                emit buf57R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf57), std::end(buf57))));
            }

            // 通风级别1-16对应的排风量, 其中 风量级别3的第二个设置为 第三级别全开风量，寄存器 0x0054
            uint16_t buf1B[16]{0};
            if(handle->readInputRegisters(0x001B, 16, buf1B) > 0){
                emit buf1BR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf1B), std::end(buf1B))));
            }

            // 通风级别对应的风量占比, Tip: 第三级别全开风量
            uint16_t buf44[17]{0};
            if(handle->readInputRegisters(0x0044, 17, buf44) > 0){
                emit buf44R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf44), std::end(buf44))));
            }
            QThread::msleep(TIMER_UPDATE_INTER);
        }
    }
}

void ParSetThree::on_spinBox_add_delay_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x003F, static_cast<uint16_t>(ui->spinBox_add_delay->value()));
}

void ParSetThree::on_spinBox_sub_delay_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0040, static_cast<uint16_t>(ui->spinBox_sub_delay->value()));
}

void ParSetThree::on_comboBox_air_model_currentIndexChanged(int index)
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0130, static_cast<uint16_t>(index));
}

void ParSetThree::on_spinBox_open_close_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0131, static_cast<uint16_t>(ui->spinBox_open_close_time->value()));
}

