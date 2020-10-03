#include <QDebug>
#include "customdialog.h"
#include "parsetone.h"
#include "ui_parsetone.h"

ParSetOne::ParSetOne(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParSetOne),
    m_pOne(new OneTaskThread)
{
    ui->setupUi(this);
    this->initConnection();
    for(int i=0; i<7; ++i) {
        auto day = this->findChild<QSpinBox*>(QString("spinBox_aim_day%1").arg(i+1));
        day->setRange(-3 + i*1, 999);
        auto t = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_aim_t%1").arg(i+1));
        t->setRange(15, 40);
    }
    for (int i=0; i<4; ++i) {
        auto levelDay = this->findChild<QSpinBox*>(QString("spinBox_level_d%1").arg(i+1));
        levelDay->setRange(-3 + i*1, 999);
        auto levelUp = this->findChild<QSpinBox*>(QString("spinBox_level_up%1").arg(i+1));
        auto levelDown = this->findChild<QSpinBox*>(QString("spinBox_level_down%1").arg(i+1));
        levelUp->setRange(3, 16);
        levelDown->setRange(1, 8);
    }
}

ParSetOne::~ParSetOne()
{
    delete ui;
    if(m_pOne != nullptr && m_pOne->isRunning()){
        m_pOne->exitThread();
        if(m_pOne->isRunning()){
            m_pOne->terminate();
        }
    }
}

void ParSetOne::initConnection()
{
    connect(m_pOne.get(), &OneTaskThread::buf005FR, this, [this](QVector<uint16_t> buf){
        for (int i=0, j=0; i<14; i+=2, j++) {
            uint airC = buf[i];
            airC = (airC << 8) | buf[i+1];
            uint airA = buf[i+14];
            airA = (airA << 8) | buf[i+15];
            auto pLabel_a = this->findChild<QLabel*>(QString("label_m_air_a%1").arg(j+1));
            auto pLabel_c = this->findChild<QLabel*>(QString("label_m_air_c%1").arg(j+1));
            qDebug() << "m_air_a:" << QString("label_m_air_a%1").arg(j+1) << "  m_air_c:" << QString("label_m_air_c%1").arg(j+1);
            pLabel_a->setText(QString::number(airA));
            pLabel_c->setText(QString::number(airC));
        }
    });
    connect(m_pOne.get(), &OneTaskThread::buf00R, this, [this](QVector<uint16_t> buf){
        for (int i=0, j=0; i<14; j++, i += 2) {
            auto pSpinBox = this->findChild<QSpinBox*>(QString("spinBox_aim_day%1").arg(j+1));
            auto pDoubleSpinBox = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_aim_t%1").arg(j+1));
            pSpinBox->setValue(static_cast<int16_t>(buf[i]));
            pDoubleSpinBox->setValue(static_cast<int16_t>(buf[i+1]) / 100.00);
        }
        uint matrixCount = buf[15];
        matrixCount  = (matrixCount << 16) | buf[16];
        ui->spinBox_matrix_count->setValue(static_cast<int>(matrixCount));
        ui->label_weight->setText(QString::number(buf[17] / 100.00));
    });
    connect(m_pOne.get(), &OneTaskThread::buf12FR, this, [this](uint16_t buf){
        ui->doubleSpinBox_discount_coefficient->setValue(buf / 10.00);
    });
    connect(m_pOne.get(), &OneTaskThread::currentDayR, this, [this](uint16_t buf){
        ui->spinBox_current_days->setValue(static_cast<int16_t>(buf));
    });
    connect(m_pOne.get(), &OneTaskThread::buf1ER, this, [this](QVector<uint16_t> buf){
        ui->comboBox_matrix_category->setCurrentIndex(buf[0]);
        for(int i = 0, j = 0; j < 7; j++, i += 2) {
            auto pSpinBox = this->findChild<QSpinBox*>(QString("spinBox_m_air_day%1").arg(j+1));
            pSpinBox->setValue(static_cast<int16_t>(buf[i+2]));
        }
    });
    connect(m_pOne.get(), &OneTaskThread::buf109R, this, [this](QVector<uint16_t> buf){
        for(int i = 0; i < 7; i++) {
            auto pDoubleSpinbox = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_m_air_matrix_w%1").arg(i+1));
            auto pDoubleSpinboxUnit = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_m_air_unit_v%1").arg(i+1));
            pDoubleSpinbox->setValue(buf[i] / 100.00);
            pDoubleSpinboxUnit->setValue(buf[i+7] / 100.00);
        }
    });
    connect(m_pOne.get(), &OneTaskThread::buf30R, this, [this](QVector<uint16_t> buf){
        for(int i = 0, j = 0; j < 4; i +=3, j++) {
            auto age = this->findChild<QSpinBox*>(QString("spinBox_level_d%1").arg(j+1));
            auto down = this->findChild<QSpinBox*>(QString("spinBox_level_down%1").arg(j+1));
            auto up = this->findChild<QSpinBox*>(QString("spinBox_level_up%1").arg(j+1));
            age->setValue(buf[i]);
            down->setValue(buf[i+1]);
            up->setValue(buf[i+2]);
        }
    });
    connect(m_pOne.get(), &OneTaskThread::buf166R, this, [this](QVector<uint16_t> buf){
        for (int i=0; i<8; ++i) {
            auto pTimeEdit = this->findChild<QTimeEdit*>(QString("timeEdit_dc_%1").arg(i+1));
            pTimeEdit->setTime(QTime(buf[i] / 100, buf[i] % 100));
        }
    });
    connect(m_pOne.get(), &OneTaskThread::calcWeightModelR, this, [this](uint16_t buf){
        ui->checkBox->setChecked(static_cast<bool>(buf));
    });


    // 设置参数的信号槽
    this->initAimDayTConnect();
    this->initUnitAirConnect();
    this->initAirLevelConnect();
    this->initDiscountConnect();
}

void ParSetOne::initAimDayTConnect()
{
    /* 目标温度和日龄
     * A11限制为为（-3,999）默认值1， A12限制为为（A11+1,999）默认值7， A13限制为为（A12+1,999）
     * 默认值14， A14限制为为（A13+1,999）默认值21， A15限制为为（A14+1,999）默认值28， A16限制
     * 为为（A15+1,999）默认值35， A17限制为为（A16+1,999）默认值42。
     * T11限制为为（15,40）默认值32， T12限制为为（15， T11）默认值29， T13限制为为（15， T12）默
     * 认值27， T14限制为为（15， T13）默认值24， T15限制为为（15， T14）默认值22， T16限制为为（15，
     * T15）默认值21， T17限制为为（15， T16）默认值20。
     */
    for (int i=1; i<=7; ++i) {
        auto day = this->findChild<QSpinBox*>(QString("spinBox_aim_day%1").arg(i));
        auto t = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_aim_t%1").arg(i));
        connect(day, &QSpinBox::editingFinished, this, [this, day, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            auto nextDay = this->findChild<QSpinBox*>(QString("spinBox_aim_day%1").arg(i+1));
            if(day->value() > nextDay->value() - 1){
                // 参数验证不合格
                CustomInfoBox::customInfoBox(tr("目标温度设置"), tr("日龄%1不能小于等于日龄%2").arg(i+1).arg(i));
                return ;
            }
            handle->writeSingleRegister(0x00 + (i-1)*2, day->value());
        });
        connect(t, &QSpinBox::editingFinished, this, [this, t, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            auto preT = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_aim_t%1").arg(i-1));
            if(preT != nullptr){
                if(t->value() >= preT->value()){
                    CustomInfoBox::customInfoBox(tr("目标温度设置"), tr("目标温度%1不能大于等于目标温度%2").arg(i).arg(i-1));
                    return ;
                }
            }
            double vol = t->value() * 100;
            handle->writeSingleRegister(0x01 + (i-1)*2, static_cast<uint16_t>(vol));
        });
    }
}

void ParSetOne::initUnitAirConnect()
{
    for (int i=0; i<7; ++i) {
        auto day = this->findChild<QSpinBox*>(QString("spinBox_m_air_day%1").arg(i+1));
        auto w = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_m_air_matrix_w%1").arg(i+1));
        auto v = this->findChild<QDoubleSpinBox*>(QString("doubleSpinBox_m_air_unit_v%1").arg(i+1));
        connect(day, &QSpinBox::editingFinished, this, [this, day, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            handle->writeSingleRegister(0x0020 + i * 2, day->value());
        });
        connect(w, &QDoubleSpinBox::editingFinished, this, [this, w, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            double vol = w->value() * 100;
            handle->writeSingleRegister(0x0109 + i, static_cast<uint16_t>(vol));
        });
        connect(v, &QDoubleSpinBox::editingFinished, this, [this, v, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            double vol = v->value() * 100;
            handle->writeSingleRegister(0x0110 + i, static_cast<uint16_t>(vol));
        });
    }
}

void ParSetOne::initAirLevelConnect()
{
    for (int i=0; i<4; ++i) {
        auto day = this->findChild<QSpinBox*>(QString("spinBox_level_d%1").arg(i+1));
        auto up = this->findChild<QSpinBox*>(QString("spinBox_level_up%1").arg(i+1));
        auto down = this->findChild<QSpinBox*>(QString("spinBox_level_down%1").arg(i+1));
        connect(day, &QSpinBox::editingFinished, this, [this, day, i]{
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            if(i > 0){
                auto preDay = this->findChild<QSpinBox*>(QString("spinBox_aim_day%1").arg(i));
                if(day->value() < preDay->value() + 1){
                    // 参数验证不合格
                    CustomInfoBox::customInfoBox(tr("通风级别设置"), tr("日龄%1不能小于等于日龄%2").arg(i+1).arg(i));
                    return ;
                }
            }
            handle->writeSingleRegister(0x0030 + i*3, day->value());
        });
        connect(up, &QSpinBox::editingFinished, this, [this, up, i]{
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            if(i > 0){
                auto preUp = this->findChild<QSpinBox*>(QString("spinBox_level_up%1").arg(i));
                if(preUp->value() > up->value()){
                    // 参数验证不合格
                    CustomInfoBox::customInfoBox(tr("通风级别设置"), tr("上限%1不能小于等于上限%2").arg(i+1).arg(i));
                    return ;
                }
            }
            handle->writeSingleRegister(0x0032 + i*3, up->value());
        });
        connect(down, &QSpinBox::editingFinished, this, [this, down, i]{
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            handle->writeSingleRegister(0x0031 + i*3, down->value());
        });
    }
}

void ParSetOne::initDiscountConnect()
{
    for (int i=0; i<8; ++i) {
        auto timeEidt = this->findChild<QTimeEdit*>(QString("timeEdit_dc_%1").arg(i+1));
        connect(timeEidt, &QTimeEdit::editingFinished, this, [this, timeEidt, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            if(i > 0){
                auto preTime = this->findChild<QTimeEdit*>(QString("timeEdit_dc_%1").arg(i));
                if(timeEidt->time() <= preTime->time()){
                    // 参数验证不合格
                    CustomInfoBox::customInfoBox(tr("折扣时间设置"), tr("时间%1不能小于等于时间%2").arg(i+1).arg(i));
                    return ;
                }
            }
            double vol = timeEidt->text().replace(2, 1, '.').toDouble() * 100;
            handle->writeSingleRegister(0x0166 + i, static_cast<uint16_t>(vol));
        });
    }
}

void ParSetOne::updatePageData(bool flag)
{
    m_pOne->setPaused(!flag);
//    this->setFocus(Qt::MouseFocusReason);
}

void OneTaskThread::run()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    while (!m_isExit) {
        if(m_isPaused){
            QThread::msleep(100);
            continue;
        }else{
            // 实际最小风量 V159之前的是用一个寄存器，V160的更改为两个寄存器保存一个值
            // 计算最小风量, 实际最小风量
            uint16_t buf005F[28]{0};
            if(handle->readInputRegisters(0x005F, 28, buf005F) > 0){
                emit buf005FR(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf005F), std::end(buf005F))));
            }

            // 目标日龄，目标温度, 最小通风比例，鸡数量，鸡体重
            uint16_t buf00[18]{0};
            if(handle->readHoldingRegisters(0x00, 18, buf00) > 0){
                emit buf00R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf00), std::end(buf00))));
            }
            // 夜间单位体重通风量折扣系数
            uint16_t buf12F;
            if(handle->readHoldingRegisters(0x012F, 1, &buf12F) > 0){
                emit buf12FR(buf12F);
            }

            // 鸡的当前日龄
            uint16_t currentDay;
            if(handle->readHoldingRegisters(0x003C, 1, &currentDay) > 0){
                emit currentDayR(currentDay);
            }

            // 最小通风比例： 养殖鸡群类别,最小通风量计算方式（废弃），7个日龄，7个最小风量
            uint16_t buf1E[16]{0};
            if(handle->readHoldingRegisters(0x001E, 16, buf1E) > 0){
                emit buf1ER(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf1E), std::end(buf1E))));
            }

            // 单只体重和单位体积通风量
            uint16_t buf109[14]{0};
            if(handle->readHoldingRegisters(0x0109, 14, buf109) > 0){
                emit buf109R(QVector<uint16_t>::fromStdVector(
                                 std::vector<uint16_t>(std::begin(buf109), std::end(buf109))));
            }

            // 通风级别限制
            uint16_t buf30[12]{0};
            if(handle->readHoldingRegisters(0x0030, 12, buf30) > 0){
                emit buf30R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf30), std::end(buf30))));
            }

            // 最小通风夜间设置
            uint16_t buf166[8]{0};
            if(handle->readHoldingRegisters(0x0166, 12, buf166) > 0){
                emit buf166R(QVector<uint16_t>::fromStdVector(
                                 std::vector<uint16_t>(std::begin(buf166), std::end(buf166))));
            }

            // 体重计算模式
            uint16_t calcWeightModel;
            if(handle->readHoldingRegisters(0x0118, 1, &calcWeightModel) > 0){
                emit calcWeightModelR(calcWeightModel);
            }

            QThread::msleep(TIMER_UPDATE_INTER);
        }
    }
}

void ParSetOne::on_comboBox_matrix_category_currentIndexChanged(int index)
{
    if(!isHaveAuthority()){return;}
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    uint16_t buf = static_cast<uint16_t>(index);
    handle->writeMultipleRegisters(0x001E, 1, &buf);
}

void ParSetOne::on_doubleSpinBox_discount_coefficient_editingFinished()
{
    if(!isHaveAuthority()){return;}
    // 夜间单位体重通风量折扣系数
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    double nightWind = ui->doubleSpinBox_discount_coefficient->value() * 10;
    handle->writeSingleRegister(0x012F, static_cast<uint16_t>(nightWind));
}

void ParSetOne::on_spinBox_matrix_count_editingFinished()
{
    // 鸡数量
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    auto matrixCount = ui->spinBox_matrix_count->value();
    uint16_t buf[2]{0};
    buf[0] = (matrixCount >> 16) & 0x00FF; buf[1] = matrixCount & 0xFFFF;
    handle->writeMultipleRegisters(0x000F, 2, buf);
}

void ParSetOne::on_spinBox_current_days_editingFinished()
{
    // 鸡的当前日龄
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    int buf = static_cast<uint16_t>(ui->spinBox_current_days->value());
    handle->writeSingleRegister(0x003C, static_cast<uint16_t>(buf));
}

void ParSetOne::on_checkBox_clicked()
{
    // 单只体重计算模式
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    uint16_t buf = ui->checkBox->isChecked() ? 1 : 0;
    handle->writeSingleRegister(0x0118, buf);
}

