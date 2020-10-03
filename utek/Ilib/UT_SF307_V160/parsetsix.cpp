#include "parsetsix.h"
#include "customdialog.h"
#include "ui_parsetsix.h"

ParSetSix::ParSetSix(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParSetSix),
    m_pSix(new SixTaskThread)
{
    ui->setupUi(this);
    this->initConnection();
}

ParSetSix::~ParSetSix()
{
    delete ui;
    if(m_pSix != nullptr && m_pSix->isRunning()){
        m_pSix->exitThread();
        if(m_pSix->isRunning()){
            m_pSix->terminate();
        }
    }
}

void ParSetSix::initConnection()
{
    connect(m_pSix.get(), &SixTaskThread::buf139R, this, [this](QVector<uint16_t> buf){
        ui->checkBox_CC->setChecked(static_cast<bool>(buf.at(0)));
        ui->checkBox_Lx->setChecked(static_cast<bool>(buf.at(1)));
        ui->checkBox_model->setChecked(static_cast<bool>(buf.at(2)));
        ui->checkBox_model->setText(static_cast<bool>(buf.at(2)) ?  "变频": "开停");
    });
    connect(m_pSix.get(), &SixTaskThread::buf125R, this, [this](QVector<uint16_t> buf){
        ui->checkBox_FB1->setChecked(static_cast<bool>(buf.at(0)));
        ui->checkBox_FB2->setChecked(static_cast<bool>(buf.at(1)));
        ui->checkBox_select->setChecked(static_cast<bool>(buf.at(2)));
        ui->checkBox_select->setText(static_cast<bool>(buf.at(2)) ? "常闭" : "常开");
    });
    connect(m_pSix.get(), &SixTaskThread::buf12BR, this, [this](uint16_t buf){
        ui->checkBox_Alarm->setChecked(static_cast<bool>(buf));
    });
    connect(m_pSix.get(), &SixTaskThread::buf117R, this, [this](QVector<uint16_t> buf){
        ui->checkBox_CO2->setChecked(static_cast<bool>(buf.at(0)));
        ui->checkBox_Hot->setChecked(static_cast<bool>(buf.at(2)));
        g_CO2 = static_cast<bool>(buf.at(0));
        g_Hot = static_cast<bool>(buf.at(2));
    });
    connect(m_pSix.get(), &SixTaskThread::buf141R, this, [this](uint16_t buf){
        ui->checkBox_control->setChecked(static_cast<bool>(buf));
        ui->checkBox_control->setText(static_cast<bool>(buf) ? "同开同关" : "常开");
    });
    connect(m_pSix.get(), &SixTaskThread::buf128R, this, [this](QVector<uint16_t> buf){
        ui->checkBox_Pa->setChecked(static_cast<bool>(buf.at(0)));
        ui->checkBox_Wet->setChecked(static_cast<bool>(buf.at(1)));
        g_PA = static_cast<bool>(buf.at(0));
        g_Wet = static_cast<bool>(buf.at(1));
    });
    connect(m_pSix.get(), &SixTaskThread::buf173R, this, [this](QVector<uint16_t> buf){
        ui->spinBox_sw_time->setValue(buf[0]);
        ui->checkBox_in10->setChecked(static_cast<bool>(buf.at(1)));
        g_NH3 = !ui->checkBox_in10->isChecked();
        ui->checkBox_in10->setText(ui->checkBox_in10->isChecked() ? "T7" : "NH3");
    });
}

void ParSetSix::updatePageData(bool flag)
{
    m_pSix->setPaused(!flag);
    this->setFocus(Qt::MouseFocusReason);
}

void SixTaskThread::run()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    while (!m_isExit) {
        if(m_isPaused){
            QThread::msleep(100);
            continue;
        }else{
            // 侧窗开关，料线计时开关，风机组合一运行模式
            uint16_t buf139[3]{0};
            if(handle->readHoldingRegisters(0x0139, 3, buf139) > 0){
                emit buf139R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf139), std::end(buf139))));
            }

            // 翻板1,2开关， 报警常开常闭报警选择，压差传感器开关，湿度传感器开关
            uint16_t buf125[5]{0};
            if(handle->readHoldingRegisters(0x0125, 5, buf125) > 0){
                emit buf125R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf125), std::end(buf125))));
            }

            // 热回收机组旁通报警开关
            uint16_t buf12B{0};
            if(handle->readHoldingRegisters(0x012B, 1, &buf12B) > 0){
                emit buf12BR(buf12B);
            }

            // 二氧化碳传感器开关, 占位， 热回收比例阀开关
            uint16_t buf117[3]{0};
            if(handle->readHoldingRegisters(0x0117, 3, buf117) > 0){
                emit buf117R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf117), std::end(buf117))));
            }

            // 第三级别时侧窗的控制方式
            uint16_t buf141{0};
            if(handle->readHoldingRegisters(0x0141, 1, &buf141) > 0){
                emit buf141R(buf141);
            }

            // 湿度，二氧化碳传感器
            uint16_t buf128[2]{0};
            if(handle->readHoldingRegisters(0x0128, 2, buf128) > 0){
                emit buf128R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf128), std::end(buf128))));
            }

            // 侧窗随动时间，INIO功能选择
            uint16_t buf173[2]{0};
            if(handle->readHoldingRegisters(0x0173, 2, buf173) > 0){
                emit buf173R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf173), std::end(buf173))));
            }

            QThread::msleep(TIMER_UPDATE_INTER);
        }
    }
}

void ParSetSix::on_checkBox_CC_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0139, ui->checkBox_CC->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_CC->setChecked(!ui->checkBox_CC->isChecked());
    }
}

void ParSetSix::on_checkBox_Lx_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x013A, ui->checkBox_Lx->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_Lx->setChecked(!ui->checkBox_Lx->isChecked());
    }
}

void ParSetSix::on_checkBox_model_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x013B, ui->checkBox_model->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_model->setChecked(!ui->checkBox_model->isChecked());
    }
    ui->checkBox_model->setText(ui->checkBox_model->isChecked() ?  "变频": "开停");
}

void ParSetSix::on_checkBox_FB1_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0125, ui->checkBox_FB1->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_FB1->setChecked(!ui->checkBox_FB1->isChecked());
    }
}

void ParSetSix::on_checkBox_FB2_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0126, ui->checkBox_FB2->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_FB2->setChecked(!ui->checkBox_FB2->isChecked());
    }
}

void ParSetSix::on_checkBox_select_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0127, ui->checkBox_select->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_select->setChecked(!ui->checkBox_select->isChecked());
    }
    ui->checkBox_select->setText(ui->checkBox_select->isChecked() ? "常闭" : "常开");
}

void ParSetSix::on_checkBox_Pa_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0128, ui->checkBox_Pa->isChecked() ? 1 : 0);
        g_PA = ui->checkBox_Pa->isChecked();
    }else{
        ui->checkBox_Pa->setChecked(!ui->checkBox_Pa->isChecked());
    }
}

void ParSetSix::on_checkBox_Wet_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0129, ui->checkBox_Wet->isChecked() ? 1 : 0);
        g_Wet = ui->checkBox_Wet->isChecked();
    }else{
        ui->checkBox_Wet->setChecked(!ui->checkBox_Wet->isChecked());
    }
}

void ParSetSix::on_checkBox_Alarm_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x012B, ui->checkBox_Alarm->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_Alarm->setChecked(!ui->checkBox_Alarm->isChecked());
    }
}

void ParSetSix::on_checkBox_CO2_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0117, ui->checkBox_CO2->isChecked() ? 1 : 0);
        g_CO2 = ui->checkBox_CO2->isChecked();
    }else{
        ui->checkBox_CO2->setChecked(!ui->checkBox_CO2->isChecked());
    }
}

void ParSetSix::on_checkBox_Hot_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0119, ui->checkBox_Hot->isChecked() ? 1 : 0);
        g_Hot = ui->checkBox_Hot->isChecked();
    }else{
        ui->checkBox_Hot->setChecked(!ui->checkBox_Hot->isChecked());
    }
}

void ParSetSix::on_checkBox_control_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0141, ui->checkBox_Hot->isChecked() ? 1 : 0);
    }else{
        ui->checkBox_control->setChecked(!ui->checkBox_control->isChecked());
    }
    ui->checkBox_control->setText(ui->checkBox_Hot->isChecked() ? "同开同关" : "常开");
}

void ParSetSix::on_spinBox_sw_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0173, ui->spinBox_sw_time->value());
    }
}

void ParSetSix::on_checkBox_in10_clicked()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(isHaveAuthority()){
        handle->writeSingleRegister(0x0174, ui->checkBox_in10->isChecked() ? 1 : 0);
        g_NH3 = !ui->checkBox_in10->isChecked();
    }else{
        ui->checkBox_in10->setChecked(!ui->checkBox_in10->isChecked());
    }
    ui->checkBox_in10->setText(ui->checkBox_in10->isChecked() ? "T7" : "NH3");
}
