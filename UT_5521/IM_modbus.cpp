#include "IM_modbus.h"
#include <QByteArray>
#include <QDebug>
#include <QDateTime>

ModbusOptation::ModbusOptation()
{
//    m_SlaveHandle = nullptr;
}

ModbusOptation::~ModbusOptation()
{
//    CloseModbus();
}

//QString ModbusOptation::ErrorString()
//{
//    QString errorString = modbus_strerror(errno);
//    qDebug() << "errno: " << errno << " " << errorString;
//    return errorString;
//}

//bool ModbusOptation::CreateModbusHandle_rtu(QString device, int baud,
//                                            QString parity, int databits,
//                                            int stopbits, int rsModel)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle != nullptr){
//        modbus_close(m_SlaveHandle);
//        m_SlaveHandle = nullptr;
//    }
//    QByteArray comByte = QString("\\\\.\\" + device).toUtf8();
//    QByteArray parityByte = parity.toUtf8();
//    m_SlaveHandle = modbus_new_rtu(comByte, baud, parityByte[0], databits, stopbits);
//    if(m_SlaveHandle != nullptr ){
//        modbus_rtu_set_serial_mode(m_SlaveHandle, rsModel);
//        if(modbus_connect(m_SlaveHandle) == 0){
//            // 设置超时属性
//            timeval time;
//            time.tv_sec = MODBUS_TIMEOUT;
//            time.tv_usec = 0;
//            modbus_set_byte_timeout(m_SlaveHandle, &time);
//            modbus_set_response_timeout(m_SlaveHandle, &time);
//            return true;
//        }
//    }
//    this->ErrorString();
//    return false;
//}

//bool ModbusOptation::setSlaveId(int deviceId)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr){
//        return false;
//    }
//    if(modbus_set_slave(m_SlaveHandle, deviceId) == 0){
//        return true;
//    }
//    this->ErrorString();
//    return false;
//}

//void ModbusOptation::CloseModbus()
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle != nullptr){
//        modbus_close(m_SlaveHandle);
//        m_SlaveHandle = nullptr;
//    }
//}

///*
// * DESCRIPTION
//-----------
//The *modbus_read_bits()* function shall read the status of the _nb_ bits (coils)
//to the address _addr_ of the remote device. The result of reading is stored in
//_dest_ array as unsigned bytes (8 bits) set to `TRUE` or `FALSE`.

//You must take care to allocate enough memory to store the results in _dest_
//(at least _nb_ * sizeof(uint8_t)).

//The function uses the Modbus function code 0x01 (read coil status).


//RETURN VALUE
//------------
//The function shall return the number of read bits if successful. Otherwise it
//shall return -1 and set errno.
// */
//int ModbusOptation::ReadCoilStatus(int addr, int nbit, uint8_t *dest)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_read_bits(m_SlaveHandle, addr, nbit, dest);
//    if (re != -1)
//    {
//        return re;
//    }
//    return -1;
//}


///*
// * DESCRIPTION
//-----------
//The *modbus_read_input_bits()* function shall read the content of the _nb_ input
//bits to the address _addr_ of the remote device.  The result of reading is stored
//in _dest_ array as unsigned bytes (8 bits) set to _TRUE_ or _FALSE_.

//You must take care to allocate enough memory to store the results in _dest_
//(at least _nb_ * sizeof(uint8_t)).

//The function uses the Modbus function code 0x02 (read input status).


//RETURN VALUE
//------------
//The function shall return the number of read input status if
//successful. Otherwise it shall return -1 and set errno.
//*/
//int ModbusOptation::ReadInputStatus(int addr, int nbit, uint8_t *dest)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_read_input_bits(m_SlaveHandle, addr, nbit, dest);
//    if (re != -1)
//    {
//        return re;
//    }
//    this->ErrorString();
//    return -1;
//}

///*
// * DESCRIPTION
//-----------
//The *modbus_read_input_registers()* function shall read the content of the _nb_
//input registers to address _addr_ of the remote device. The result of the
//reading is stored in _dest_ array as word values (16 bits).

//You must take care to allocate enough memory to store the results in _dest_ (at
//least _nb_ * sizeof(uint16_t)).

//The function uses the Modbus function code 0x04 (read input registers). The
//holding registers and input registers have different historical meaning, but
//nowadays it's more common to use holding registers only.


//RETURN VALUE
//------------
//The function shall return the number of read input registers if
//successful. Otherwise it shall return -1 and set errno.
//*/
//int  ModbusOptation::ReadInputRegisters(int addr, int nb, uint16_t *dest)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_read_input_registers(m_SlaveHandle, addr, nb, dest);
//    if (re != -1)
//    {
//        return re;
//    }
//    this->ErrorString();
//    return -1;
//}

///*
// * DESCRIPTION
//-----------
//The *modbus_read_registers()* function shall read the content of the _nb_
//holding registers to the address _addr_ of the remote device. The result of
//reading is stored in _dest_ array as word values (16 bits).

//You must take care to allocate enough memory to store the results in _dest_
//(at least _nb_ * sizeof(uint16_t)).

//The function uses the Modbus function code 0x03 (read holding registers).


//RETURN VALUE
//------------
//The function shall return the number of read registers
//if successful. Otherwise it shall return -1 and set errno.
//*/
//int  ModbusOptation::ReadHoldingRegisters(int addr, int nb, uint16_t *dest)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_read_registers(m_SlaveHandle, addr, nb, dest);
//    if (re != -1)
//    {
//        return re;
//    }
//    this->ErrorString();
//    return -1;
//}

///* DESCRIPTION
//-----------
//The *modbus_write_bit()* function shall write the status of _status_ at the
//address _addr_ of the remote device. The value must be set to `TRUE` or `FALSE`.

//The function uses the Modbus function code 0x05 (force single coil).

//RETURN VALUE
//------------
//The function shall return 1 if successful. Otherwise it shall return -1 and set
//errno.
//*/
//int ModbusOptation::WriteSingleCoil(int addr, int value)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_write_bit(m_SlaveHandle, addr, value);
//    if (re != -1)
//    {
//        return re;
//    }
//    this->ErrorString();
//    return -1;
//}

///*
// * DESCRIPTION
//-----------
//The *modbus_write_bits()* function shall write the status of the _nb_ bits
//(coils) from _src_ at the address _addr_ of the remote device. The
//_src_ array must contains bytes set to `TRUE` or `FALSE`.

//The function uses the Modbus function code 0x0F (force multiple coils).


//RETURN VALUE
//------------
//The function shall return the number of written bits if successful. Otherwise it
//shall return -1 and set errno.
//*/
//int  ModbusOptation::WriteMultipleCoil(int addr, int nb, const uint8_t *src)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_write_bits(m_SlaveHandle, addr, nb, src);
//    if (re != -1)
//    {
//        return re;
//    }
//    this->ErrorString();
//    return -1;
//}

///*
// * DESCRIPTION
//-----------
//The *modbus_write_register()* function shall write the value of _value_
//holding registers at the address _addr_ of the remote device.

//The function uses the Modbus function code 0x06 (preset single register).


//RETURN VALUE
//------------
//The function shall return 1 if successful. Otherwise it shall return -1 and set
//errno.
//*/
//int  ModbusOptation::WriteSingleRegister(int addr, int value)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_write_register(m_SlaveHandle, addr, value);
//    if (re != -1)
//    {
//        return re;
//    }
//    this->ErrorString();
//    return -1;
//}

///*
// * DESCRIPTION
//-----------
//The *modbus_write_registers()* function shall write the content of the _nb_
//holding registers from the array _src_ at address _addr_ of the remote device.

//The function uses the Modbus function code 0x10 (preset multiple registers).


//RETURN VALUE
//------------
//The function shall return the number of written registers if
//successful. Otherwise it shall return -1 and set errno.
//*/
//int  ModbusOptation::WriteMultipleRegisters(int addr, int nb, const uint16_t *src)
//{
//    std::unique_lock<std::mutex> lk(mut);
//    if(m_SlaveHandle == nullptr)
//    {
//        return -1;
//    }
//    int re = modbus_write_registers(m_SlaveHandle, addr,nb, src);
//    if (re != -1)
//    {
//        return re;
//    }
//    this->ErrorString();
//    return -1;
//}

//int ModbusOptation::WriteToModbus(Task &info)
//{
//    int re = -1;
//    switch (info.WriteRegisterType) {
//    case Task::RegisterType::coils:{
//        uint8_t *pBuff = new uint8_t[info.GetWritLen()]();
//        for(int i = 0; i < info.GetWritLen(); i++)
//        {
//            pBuff[i] = info.WriteCoils.at(i);
//        }
//        re = WriteMultipleCoil(info.WriteStartAddress, info.GetWritLen(), pBuff);
//        delete[] pBuff;
//        pBuff = nullptr;
//    }
//        break;
//    case Task::RegisterType::HoldingRegisters:{
//        uint16_t *pBuff = new uint16_t[info.GetWritLen()]();
//        for(int i = 0; i < info.GetWritLen(); i++)
//        {
//            pBuff[i] = info.WriteHolding.at(i);
//        }
//        re = WriteMultipleRegisters(info.WriteStartAddress, info.GetWritLen(), pBuff);
//        delete[] pBuff;
//        pBuff = nullptr;
//    }
//        break;
//    case Task::RegisterType::HoldingSingleRegister:{
//        uint16_t buff = 0;
//        buff = info.WriteHolding.at(0);
//        re = WriteSingleRegister(info.WriteStartAddress, buff);
//    }
//        break;
//    default:
//        break;
//    }
//    return re;
//}

//int ModbusOptation::ReadFromModbus(Task &info)
//{
//    int re = -1;
//    switch (info.ReadRegisterType) {
//    case Task::RegisterType::coils:{
//        uint8_t *pBuff = new uint8_t[info.ReadLen]();
//        re = ReadCoilStatus(info.ReadStartAddress, info.ReadLen, pBuff);
//        if(re > 0)
//        {
//            info.ReadCoils.resize(re);
//            for(int i = 0; i < re; i++)
//            {
//                info.ReadCoils.setBit(i, pBuff[i]);
//            }
//        }
//        delete[] pBuff;
//        pBuff = nullptr;
//    }
//        break;
//    case Task::RegisterType::DiscreteInputs:{
//        uint8_t *pBuff = new uint8_t[info.ReadLen]();
//        re = ReadInputStatus(info.ReadStartAddress, info.ReadLen, pBuff);
//        if(re > 0)
//        {
//            info.ReadCoils.resize(re);
//            for(int i = 0; i < re; i++)
//            {
//                info.ReadCoils.setBit(i, pBuff[i]);
//            }
//        }
//        delete[] pBuff;
//        pBuff = nullptr;
//    }
//        break;
//    case Task::RegisterType::InputRegisters:{
//        uint16_t *pBuff = new uint16_t[info.ReadLen]();
//        re = ReadInputRegisters(info.ReadStartAddress, info.ReadLen, pBuff);
//        if(re > 0)
//        {
//            info.ReadHolding.clear();
//            for(int i = 0; i < re; i++)
//            {
//                info.ReadHolding.append(pBuff[i]);
//            }
//        }
//        delete[] pBuff;
//        pBuff = nullptr;
//    }
//        break;
//    case Task::RegisterType::HoldingRegisters:{
//        uint16_t *pBuff = new uint16_t[info.ReadLen]();
//        re = ReadHoldingRegisters(info.ReadStartAddress, info.ReadLen, pBuff);
//        if(re > 0)
//        {
//            info.ReadHolding.clear();
//            for(int i = 0; i < re; i++)
//            {
//                info.ReadHolding.append(pBuff[i]);
//            }
//        }
//        delete[] pBuff;
//        pBuff = nullptr;
//    }
//        break;
//    default:
//        break;
//    }
//    return re;
//}


WorkThread::WorkThread()
{
    this->m_onlineDevice.resize(DEVICE_MAX_COUNT);
    this->m_onlineDevice.fill(true);

    m_prealHumiture = new RealHumiture;

    connect(this, SIGNAL(homeResult(QVariant)),
            m_prealHumiture, SLOT(onHomeResult(QVariant)));
}

bool WorkThread::handleOpenDevice(ModbusOptation *pdevice, Task & task)
{
    return true;
//    bool re = false;
//    if(task.m_isRtu){
//        re = pdevice->CreateModbusHandle_rtu(
//                    task.rtuModbusInfo.m_comDevice,
//                    task.rtuModbusInfo.m_baud,
//                    task.rtuModbusInfo.m_parity,
//                    task.rtuModbusInfo.m_databits,
//                    task.rtuModbusInfo.m_stopbits,
//                    task.rtuModbusInfo.m_rsModel);
//    }else{// TODO: tcp 先不处理
//        ;
//    }
//    return re;
}
int WorkThread::handleReadRquest(ModbusOptation *pdevice, Task & task)
{
//    if(!this->m_onlineDevice[task.m_deviceAddr - 1]){
//        // 设备不在线就不去读取
//        return -1;
//    }
//    int re = pdevice->ReadFromModbus(task);
//    if(re > 0){
        // 任务分发
        // TODO: 处理 UT-5521 的业务，其他到时候再更改
        qsrand(static_cast<uint>(QTime(0, 0, 0).secsTo(QTime::currentTime())));
        uint16_t temperature = qrand() % 4000;
        uint16_t humidity = qrand() % 10000;
        if (task.ReadRegisterType == Task::RegisterType::HoldingRegisters) {
            // 0x03 功能码
            QVariant msg;
            task.ReadHolding.append(temperature);
            msg.setValue(task);
            if(task.m_page == 3){
                emit readCmdResult(msg);
            }
        }else if(task.ReadRegisterType == Task::RegisterType::InputRegisters){
            // 0x04 功能码, 有两个页面会产生这个数据
            QVariant msg;
            task.ReadHolding.append(temperature);
            task.ReadHolding.append(humidity);
            msg.setValue(task);
            if(task.m_page == 0){
                emit homeResult(msg);
            }else if(task.m_page == 2){
                emit modbusTestResult(msg);
            }
        }
        return true;
//    }else{
//        // 判断设备不在线
//        this->m_onlineDevice[task.m_deviceAddr - 1] = false;
//    }
//    return re;
}

void WorkThread::run()
{
    // TODO: 先处理 UT-5521 特殊任务，不去管 modbus 的封装
    bool ExitFlag = false;
    ModbusOptation *pDevice = new ModbusOptation;
    qint64 checkTime = QDateTime::currentSecsSinceEpoch();
    // 循环在收到关闭设备的任务时退出循;
    while (!ExitFlag) {
        // DEVICE_CHECK_ONLINE_TIME 之后从新检查
        qint64 nowTime = QDateTime::currentSecsSinceEpoch();
        if(nowTime - checkTime >= DEVICE_CHECK_ONLINE_TIME * 60){
            this->m_onlineDevice.fill(true);
            checkTime = nowTime;
        }
        Task task = this->popFromTask();
        if(task.task_type != Task::TaskType::Invalids)
        {
            // 根据不同的 task_type 类型处理不同的任务
            switch (task.task_type) {
            case Task::TaskType::Open_Device:{
                QVariant msg;
                emit openResult(true, msg);
                // TODO: 设置从机地址没有处理
//                if(this->handleOpenDevice(pDevice, task)){
//                    msg.setValue(task);
//                    emit openResult(true, msg);
//                }else {
//                    msg.setValue(task);
//                    emit openResult(false, msg);
//                }
            }
                break;
            case Task::TaskType::Close_Device:{
//                pDevice->CloseModbus();
                emit closeResult();
                emit exitModbus();
            }
                break;
            case Task::TaskType::Write_Data:{
//                pDevice->setSlaveId(task.m_deviceAddr);
                QVariant msg;
                msg.setValue(task);
                emit modifyCmdResult(msg);
//                int re = pDevice->WriteToModbus(task);
//                if(re == -1){
//                    // TODO: 先不处理写入错误
//                    ;
//                }else {
//                    QVariant msg;
//                    msg.setValue(task);
//                    emit modifyCmdResult(msg);
//                }
            }
                break;
            case Task::TaskType::Read_Request:{
//                pDevice->setSlaveId(task.m_deviceAddr);
                this->handleReadRquest(pDevice, task);
//                if(task.m_page == 0){
//                    handleInsertRecord(m_phandleRecord, task);
//                }
            }
                break;
            case Task::TaskType::Query_Record:{
//                QList<HumitureRecord> records = m_phandleRecord->queryDatabase(
//                            task.m_deviceAddr, task.m_queryDate);
//                foreach(HumitureRecord record, records){
//                    QVariant msg;
//                    msg.setValue(record);
//                    emit recordResult(msg);
//                }
            }
                break;
            case Task::TaskType::Exit_Thread:{
                ExitFlag = true;
            }
                break;
            default:
                break;
            }
        }else {
            QThread::msleep(100);
        }
    }
//    m_phandleRecord->closeConnection();
    delete m_prealHumiture;
}

void WorkThread::pushToTask(Task &task)
{
    QMutexLocker locker(&m_mutex);
    this->m_Task.enqueue(task);
}

Task WorkThread::popFromTask()
{
    QMutexLocker locker(&m_mutex);
    if(this->m_Task.isEmpty()){
        // 返回无效的 Task
        return Task();
    }else{
        return this->m_Task.dequeue();
    }
}







