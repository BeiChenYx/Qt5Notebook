#include "setparameter.h"
#include "ui_setparameter.h"
#include "FrameLessWidget/framelesswidget.h"

SetParameter::SetParameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetParameter)
{
    ui->setupUi(this);
    on_comboBox_ts_model_currentIndexChanged(0);
}

SetParameter::~SetParameter()
{
    delete ui;
}

void SetParameter::setBaudrates(QStringList &canBaudrates, QStringList &serialPortBaudrates)
{
    ui->comboBox_baude->addItems(canBaudrates);
    ui->comboBox_baude_can->addItems(canBaudrates);
    ui->comboBox_baude_par->addItems(serialPortBaudrates);
}

void SetParameter::on_pushButton_set_clicked()
{
    if(!m_isHid){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID连接."));
        return;
    }
    QString baudrate = ui->comboBox_baude->currentText();
    HidTaskPar par;
    par.m_baudrate = baudrate;
    m_pHidWorkThread->pushToTask(HidTask::SET_PAR_CAN, par);
}

void SetParameter::on_pushButton_model_clicked()
{
    if(!m_isHid){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID连接."));
        return;
    }
    HidTaskPar par;
    par.m_canModel = ui->comboBox_model->currentText();
    m_pHidWorkThread->pushToTask(HidTask::SET_MODE_CAN, par);
}

void SetParameter::on_pushButton_reset_clicked()
{
    if(!m_isHid){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID连接."));
        return;
    }
    m_pHidWorkThread->pushToTask(HidTask::RESET_CAN);
}

void SetParameter::on_pushButton_reset_error_clicked()
{
    if(!m_isHid){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID连接."));
        return;
    }
    m_pHidWorkThread->pushToTask(HidTask::RESET_ERROR_CAN);
}

void SetParameter::on_pushButton_read_can_clicked()
{
    if(!m_isHid){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID连接."));
        return;
    }
    m_pHidWorkThread->pushToTask(HidTask::CAN_PAR);
}

void SetParameter::on_pushButton_com_par_clicked()
{
    if(!m_isHid && !m_isSerial){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID或串口的连接."));
        return;
    }
    if(m_isHid){
        HidTaskPar par;
        par.bauderateSerial = ui->comboBox_baude_par->currentText();
        par.dataBit = ui->comboBox_databits_par->currentText();
        par.parityBit = ui->comboBox_parity_par->currentText();
        par.stopBit = ui->comboBox_stopbits_par->currentText();
        par.flowControl = ui->comboBox_flowControl_par->currentText();
        par.serailModel = ui->comboBox_model_par->currentText();
        m_pHidWorkThread->pushToTask(HidTask::SET_PAR_SERIAL, par);
    }
    if(m_isSerial){
        SerialTaskPar par;
        par.bauderateSerial = ui->comboBox_baude_par->currentText();
        par.dataBit = ui->comboBox_databits_par->currentText();
        par.parityBit = ui->comboBox_parity_par->currentText();
        par.stopBit = ui->comboBox_stopbits_par->currentText();
        par.flowControl = ui->comboBox_flowControl_par->currentText();
        par.serailModel = ui->comboBox_model_par->currentText();
        m_pHandleSerial->setSerialPar(par);
    }
}

void SetParameter::on_pushButton_com_read_par_clicked()
{
    if(!m_isHid && !m_isSerial){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID或串口的连接."));
        return;
    }
    if(m_isHid){
        m_pHidWorkThread->pushToTask(HidTask::MCU_PAR);
    }
    if(m_isSerial){
        m_pHandleSerial->readMcuConfig();
    }
}

void SetParameter::on_pushButton_ts_clicked()
{
    if(!m_isHid && !m_isSerial){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID或串口的连接."));
        return;
    }
    if(m_isSerial){
        // 设置转换参数
        SerialTaskPar par;
        par.transitionModel = ui->comboBox_ts_model->currentText();
        par.transitionDirecte = ui->comboBox_ts_direct->currentText();
        par.enableSerial = ui->comboBox_ts_msg->currentText();
        par.enableID = ui->comboBox_ts_id->currentText();
        par.serialOffset = ui->comboBox_frame_offset->currentText().toInt();
        par.serialFramLen = ui->comboBox_frame_len->currentText().toInt();
        par.serialTimes = ui->spinBox_frame_times->text().toInt();
        par.transitionTimes = ui->spinBox_ts_times->text().toInt();
        m_pHandleSerial->setTransitionPar(par);
    }
    if(m_isHid){
        HidTaskPar par;
        par.transitionModel = ui->comboBox_ts_model->currentText();
        par.transitionDirecte = ui->comboBox_ts_direct->currentText();
        par.enableSerial = ui->comboBox_ts_msg->currentText();
        par.enableID = ui->comboBox_ts_id->currentText();
        par.serialOffset = ui->comboBox_frame_offset->currentText();
        par.serialFramLen = ui->comboBox_frame_len->currentText();
        par.serialTimes = ui->spinBox_frame_times->text();
        par.transitionTimes = ui->spinBox_ts_times->text();
        m_pHidWorkThread->pushToTask(HidTask::SET_TRANSITION_PAR_SERIAL, par);
    }
}

void SetParameter::on_pushButton_set_can_clicked()
{
    if(!m_isHid && !m_isSerial){
        CustomWarningBox::customWarningBox(tr("操作错误"), tr("请先打开设备的USB HID或串口的连接."));
        return;
    }
    QString id = handleId(ui->lineEdit_can_id->text().trimmed());
    if(id.isEmpty()){
        CustomInfoBox::customInfoBox(tr("帧ID不合法"), tr("帧ID错误, 正确格式: 0A FA 0B 12"));
        return;
    }
    QString max = ui->comboBox_frame_format->currentIndex() ? "1F FF FF FF" : "00 00 07 FF";
    QString errMsg = ui->comboBox_frame_format->currentIndex() ? tr("扩展帧ID最大0x1FFFFFFF"): tr("标准帧ID最大0x07FF");
    if(id > max){
        CustomInfoBox::customInfoBox(tr("帧ID不合法"), errMsg);
        return;
    }
    if(m_isSerial){
        // 设置CAN参数
        SerialTaskPar par;
        par.bauterateCan = ui->comboBox_baude_can->currentText();
        par.canID = id;
        par.frameFormat = ui->comboBox_frame_format->currentText();
        m_pHandleSerial->setCanPar(par);
    }
    if(m_isHid){
        HidTaskPar par;
        par.bauterateCan = ui->comboBox_baude_can->currentText();
        par.canID = id;
        par.frameFormat = ui->comboBox_frame_format->currentText();
        m_pHidWorkThread->pushToTask(HidTask::SET_CAN_SERIAL, par);
    }
}

void SetParameter::onIsRMcuPar(TransiteParameterUi &canPar, SerialPortParameterUi &serialPar)
{
    ui->comboBox_ts_model->setCurrentText(canPar.model);
    ui->comboBox_ts_direct->setCurrentText(canPar.directe);
    ui->comboBox_ts_msg->setCurrentText(canPar.enableSerial);
    ui->comboBox_ts_id->setCurrentText(canPar.enableId);
    ui->comboBox_frame_offset->setCurrentText(canPar.offset);
    ui->comboBox_frame_len->setCurrentText(canPar.frameLen);
    ui->spinBox_frame_times->setValue(canPar.serialTimes.toInt());
    ui->spinBox_ts_times->setValue(canPar.transiteTimes.toInt());

    ui->comboBox_baude_par->setCurrentText(serialPar.baudrate);
    ui->comboBox_databits_par->setCurrentText(serialPar.databit);
    ui->comboBox_parity_par->setCurrentText(serialPar.paritybit);
    ui->comboBox_stopbits_par->setCurrentText(serialPar.stopbit);
    ui->comboBox_flowControl_par->setCurrentText(serialPar.flow);
    ui->comboBox_model_par->setCurrentText(serialPar.model);
}

void SetParameter::onIsRSerialPar(QString &baudrate, QString &id, QString &frameFormat)
{
    ui->comboBox_baude_can->setCurrentText(baudrate);
    if(!id.trimmed().contains(" ")){
        ui->lineEdit_can_id->setText(handleId(id));
    }else{
        ui->lineEdit_can_id->setText(id);
    }
    ui->comboBox_frame_format->setCurrentText(frameFormat);
}

void SetParameter::onCanBaudrate(QString &baudrate)
{
    ui->comboBox_baude->setCurrentText(baudrate);
}

void SetParameter::onSerialStatus(bool status)
{
    ui->pushButton_set->setEnabled(!status);
    ui->pushButton_model->setEnabled(!status);
    ui->pushButton_reset->setEnabled(!status);
    ui->pushButton_reset_error->setEnabled(!status);
    ui->pushButton_read_can->setEnabled(!status);
    m_isSerial = status;
}

void SetParameter::onHidStatus(bool status)
{
    m_isHid = status;
}

void SetParameter::on_comboBox_ts_model_currentIndexChanged(int index)
{
    if(index == 0){
        ui->comboBox_frame_offset->setEnabled(false);
        ui->comboBox_frame_len->setEnabled(false);
        ui->spinBox_frame_times->setEnabled(true);
        ui->spinBox_ts_times->setEnabled(true);
        ui->comboBox_ts_id->setEnabled(true);
        ui->comboBox_ts_msg->setEnabled(true);
    }else if(index == 1){
        ui->comboBox_ts_id->setEnabled(false);
        ui->comboBox_ts_msg->setEnabled(false);
        ui->comboBox_frame_offset->setEnabled(true);
        ui->comboBox_frame_len->setEnabled(true);
        ui->spinBox_frame_times->setEnabled(true);
        ui->spinBox_ts_times->setEnabled(true);
    }else if(index == 2){
        ui->comboBox_ts_id->setEnabled(false);
        ui->comboBox_ts_msg->setEnabled(false);
        ui->comboBox_frame_offset->setEnabled(false);
        ui->comboBox_frame_len->setEnabled(false);
    }else if(index == 3){
        ui->spinBox_frame_times->setEnabled(false);
        ui->spinBox_ts_times->setEnabled(false);
        ui->comboBox_ts_id->setEnabled(false);
        ui->comboBox_ts_msg->setEnabled(false);
        ui->comboBox_frame_offset->setEnabled(false);
        ui->comboBox_frame_len->setEnabled(false);
    }else{
        ui->comboBox_frame_offset->setEnabled(true);
        ui->comboBox_frame_len->setEnabled(true);
        ui->spinBox_frame_times->setEnabled(true);
        ui->spinBox_ts_times->setEnabled(true);
        ui->comboBox_ts_id->setEnabled(true);
        ui->comboBox_ts_msg->setEnabled(true);
    }
}


