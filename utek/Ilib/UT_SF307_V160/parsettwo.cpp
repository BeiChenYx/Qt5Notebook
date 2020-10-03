#include <QDebug>
#include "customdialog.h"
#include "parsettwo.h"
#include "ui_parsettwo.h"

ParSetTwo::ParSetTwo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParSetTwo),
    m_pTwo(new TwoTaskThread)
{
    ui->setupUi(this);
    this->initConnection();

    for (int i=0; i<7; ++i) {
        auto airVol = this->findChild<QSpinBox*>(QString("spinBox_air_vol%1").arg(i+1));
        airVol->setRange(0, 600);
    }
    ui->spinBox_circle_time->setRange(0, 150);
}

ParSetTwo::~ParSetTwo()
{
    delete ui;
    if(m_pTwo != nullptr && m_pTwo->isRunning()){
        m_pTwo->exitThread();
        if(m_pTwo->isRunning()){
            m_pTwo->terminate();
        }
    }
}

void ParSetTwo::initConnection()
{
    connect(m_pTwo.get(), &TwoTaskThread::buf42R, this, [this](QVector<uint16_t> buf){
        for(int i=0; i<7; ++i){
            auto spin = this->findChild<QSpinBox*>(QString("spinBox_air_vol%1").arg(i+1));
            spin->setValue(buf[i]);
        }
        ui->spinBox_circle_time->setValue(buf[7]);
        for (int row=0; row<13; ++row) {
            for (int col=0; col<7; ++col) {
                auto checkbox = this->findChild<QCheckBox*>(QString::asprintf("checkBox_%02d%d",row+1, col+1));
                checkbox->setChecked(static_cast<bool>(buf[row + 8] & (1 << col)));
                qDebug() << "checkBox: "<< checkbox->objectName() << " value:" << (buf[row + 8] & (1 << col));
            }
        }
        this->calcAirVolSum();
    });

    this->initAirVolConnect();
    this->initAirConbinationConnect();
}

void ParSetTwo::initAirVolConnect()
{
    for (int i=0; i<7; ++i) {
        auto airVol = this->findChild<QSpinBox*>(QString("spinBox_air_vol%1").arg(i+1));
        connect(airVol, &QSpinBox::editingFinished, this, [this, airVol, i](){
            auto handle = m_pHandbleModbus.lock();
            if(!handle){return;}
            if(!isHaveAuthority()){return;}
            if(handle->writeSingleRegister(0x0042 + i, static_cast<uint16_t>(airVol->value())) > 0){
                this->calcAirVolSum();
            }
        });
    }
}

void ParSetTwo::initAirConbinationConnect()
{
    for (int row=0; row<13; ++row) {
        for (int col=0; col<7; ++col) {
            auto checkbox = this->findChild<QCheckBox*>(QString::asprintf("checkBox_%02d%d",row+1, col+1));
            connect(checkbox, &QCheckBox::clicked, this, [this, row](){
                auto handle = m_pHandbleModbus.lock();
                if(!handle){return;}
                if(!isHaveAuthority()){return;}
                uint16_t dst{0};
                for (int i=0; i<7; ++i) {
                    auto checkbox = this->findChild<QCheckBox*>(QString::asprintf("checkBox_%02d%d",row+1, i+1));
                    uint16_t status = checkbox->isChecked() ? 1 : 0;
                    dst |= status << i;
                }
                if(this->calcAirVolSum()){
                    handle->writeSingleRegister(0x004A + row, dst);
                }
            });
        }
    }
}

void ParSetTwo::updatePageData(bool flag)
{
    m_pTwo->setPaused(!flag);
    this->setFocus(Qt::MouseFocusReason);
}

void TwoTaskThread::run()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    while (!m_isExit) {
        if(m_isPaused){
            QThread::msleep(100);
            continue;
        }else{
            // 纵向风机1-7组的风量,采用纵向风机去作为最小通风时的开停循环时间D12 风机组合
            uint16_t buf42[21]{0};
            if(handle->readHoldingRegisters(0x0042, 21, buf42) > 0){
                emit buf42R(QVector<uint16_t>::fromStdVector(
                                std::vector<uint16_t>(std::begin(buf42), std::end(buf42))));
            }
            QThread::msleep(TIMER_UPDATE_INTER);
        }
    }
}

bool ParSetTwo::calcAirVolSum()
{
    double preSum{0.0};
    bool flag = true;
    for (int row=0; row<13; ++row) {
        double sum = 0;
        for (int col=0; col<7; ++col) {
            auto spin= this->findChild<QSpinBox*>(QString("spinBox_air_vol%1").arg(col+1));
            auto checkbox = this->findChild<QCheckBox*>(QString::asprintf("checkBox_%02d%d",row+1, col+1));
            sum = sum + (checkbox->isChecked() ? 1 : 0) * spin->value();
        }
        auto lable = this->findChild<QLabel*>(QString::asprintf("label_%d0z", row+1));
        lable->setNum(sum);
        if(row > 0){
            if(sum < preSum){
//                ui->label_info->setText("组合风量必须逐级增加");
                flag = false;
            }
        }
        preSum = sum;
    }
    ui->label_info->setText(flag ? "" : "组合风量必须逐级增加");
    return true;
}

void ParSetTwo::on_spinBox_circle_time_editingFinished()
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    if(!isHaveAuthority()){return;}
    handle->writeSingleRegister(0x0049, static_cast<uint16_t>(ui->spinBox_circle_time->value()));
}
