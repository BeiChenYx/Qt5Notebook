#include "handlehid.h"
#include <QDateTime>
#include <QList>
#include <QDebug>


QString handleId(QString id)
{
    bool ok;
    QStringList rst;
    if(id.isEmpty()){return QString();}
    for(QString item: id.split(" ")){
        if(item.length()>2){ return QString(); }
        auto itemStr = item.rightJustified(2, '0');
        ok = true;
        itemStr.toUInt(&ok, 16);
        if(!ok){return QString();}
        rst.append(itemStr);
    }
    return rst.join(" ");
}

QString uintToQString(const uint id)
{
    QByteArray idByte ;
    uint32_t num = id;
    idByte.append(static_cast<char>((num >> 24) & 0xFF));
    idByte.append(static_cast<char>((num >> 16) & 0xFF));
    idByte.append(static_cast<char>((num >> 8) & 0xFF));
    idByte.append(static_cast<char>(num & 0xFF));
    return QString(idByte.toHex(' ').toUpper());
}

uint stringToUint(const QString id)
{
    bool ok;
    QString idStr = handleId(id);
    if(idStr.isEmpty()){return 0;}
    return idStr.replace(" ", "").toUInt(&ok, 16);
}

HandleHid::HandleHid()
{
}

HandleHid::~HandleHid()
{
}

bool HandleHid::openHid(QString vid, QString pid, QString serialNumber)
{
    bool ok;
    QByteArray serial = serialNumber.toLatin1();
    char *pSerial = serial.data();
    size_t len = static_cast<size_t>(serialNumber.length());
    auto re = hidOpenSerial(vid.toUShort(&ok, 16), pid.toUShort(&ok, 16), pSerial, len);
    return re;
}

void HandleHid::closeHid()
{
    hidClose();
}

bool HandleHid::isAlive()
{
    return isAliveHid();
}

bool HandleHid::openCan()
{
    return canOpen();
}

bool HandleHid::closeCan()
{
    return canClose();
}

bool HandleHid::initCan(QString &version)
{
    QByteArray buf(32, '\0');
    bool re = canInit(buf.data(), static_cast<size_t>(buf.length()));
    version = QString(buf);
    return re;
}

bool HandleHid::enableRcvCan(bool enable)
{
    return canRcvEnable(enable);
}

bool HandleHid::setCanModel(QString model)
{
    if(!m_canModel.contains(model)){ return false; }
    return canSetModel(static_cast<uchar>(m_canModel.indexOf(model)));
}

bool HandleHid::resetCan()
{
    return canReset();
}

bool HandleHid::resetCanError()
{
    return canResetError();
}

bool HandleHid::setCanBaudrate(QString baudrate)
{
    if(!m_canBaudrates.contains(baudrate)){ return false; }
    return canSetBaudRate(static_cast<uchar>(m_canBaudrates.indexOf(baudrate)));
}

int HandleHid::writeToCan(QString id, QString frameType, QString frameFormat, QString data)
{
    if(!m_frameType.contains(frameType)){ return false; }
    if(!m_frameFormat.contains(frameFormat)){ return false; }
    uint32_t idNum = stringToUint(id);
    uchar frameTyepNum = static_cast<uchar>(m_frameType.indexOf(frameType));
    uchar frameFormatNum = static_cast<uchar>(m_frameFormat.indexOf(frameFormat));
    QByteArray buf = QByteArray::fromHex(data.trimmed().replace(" ", "").toLocal8Bit());
    return canWrite(idNum, frameTyepNum, frameFormatNum,
                    reinterpret_cast<uchar*>(buf.data()), static_cast<uchar>(buf.length()));
}

void HandleHid::readFromCan()
{
    canRead();
}

bool HandleHid::setCanFilterId(QList<QStringList> &filter)
{
    uchar filterCount = static_cast<uchar>(filter.length());
    for(QStringList item: filter){
        try {
            uchar index = static_cast<uchar>(filter.indexOf(item) + 1);
            uchar filterType = static_cast<uchar>(m_idFilterTypeList.indexOf(item.at(0)));
            QString id1 = item.at(1), id2 = item.at(2);
            uint32_t start = stringToUint(id1);
            uint32_t end = stringToUint(id2);
            if(!setFilterId(filterCount, index, filterType, start, end)){
                return false;
            }
        } catch (...) {
            return false;
        }
    }
    return true;
}

bool HandleHid::clearCanFilterId()
{
    return clearFilterId();
}

bool HandleHid::readCanPar(QString &baudrate, QList<QStringList> &filter)
{
    uchar baudrateNum;
    QByteArray buf(512, '\0');
    auto re = canParameter(&baudrateNum, buf.data(), static_cast<size_t>(buf.length()));
    if(baudrateNum > m_canBaudrates.length()){return false;}
    baudrate = m_canBaudrates.at(baudrateNum);
    for(auto item: QString(buf).trimmed().split(";")) {
        if(item.trimmed().isEmpty()){continue;}
        auto row = item.split(",");
        if(row.length() != 3){return false;}
        filter.append(row);
    }
    return re;
}

bool HandleHid::setSerialPortTransitionPar(const TransiteParameterUi &par)
{
    TransiteParameter parLib;
    parLib.model = static_cast<uchar>(m_transitionModel.indexOf(par.model));
    parLib.directe = static_cast<uchar>(m_transitionDirecte.indexOf(par.directe));
    parLib.enableSerial = par.enableSerial == tr("不允许") ? 0x00 : 0x01;
    parLib.enableId= par.enableId== tr("不允许") ? 0x00 : 0x01;
    parLib.offset = static_cast<uchar>(par.offset.toUInt());
    parLib.frameLen = static_cast<uchar>(par.frameLen.toUInt());
    parLib.serialTimes = static_cast<uchar>(par.serialTimes.toUInt());
    parLib.transiteTimes = static_cast<uchar>(par.transiteTimes.toUInt());
    return canSetSerialPortTransitionPar(parLib);
}

bool HandleHid::setSerialPortPar(const SerialPortParameterUi &par)
{
    SerialPortParameter parLib;
    parLib.baudrate = static_cast<uchar>(m_serailBaudrates.indexOf(par.baudrate));
    parLib.databit = static_cast<uchar>(m_databits.indexOf(par.databit));
    parLib.paritybit = static_cast<uchar>(m_paritybits.indexOf(par.paritybit));
    parLib.stopbit = static_cast<uchar>(m_stopbits.indexOf(par.stopbit));
    parLib.flow = par.flow == tr("硬件流控制") ? 0x00 : 0x01;
    parLib.model = static_cast<uchar>(m_serailmodel.indexOf(par.model));
    return canSetSerialPortPar(parLib);
}

bool HandleHid::setSerialPortCanPar(QString baudrate, QString id, QString frameFormat)
{
    uchar baudrateNum = static_cast<uchar>(m_canBaudrates.indexOf(baudrate));
    uint32_t idNum = stringToUint(id);
    uchar frameFormatNum = frameFormat == tr("标准帧") ? 0x00 : 0x01;
    return canSetSerialPortCanPar(baudrateNum, idNum, frameFormatNum);
}

bool HandleHid::readSerialPortPar(TransiteParameterUi &parTransite, SerialPortParameterUi &parSerial)
{
    TransiteParameter parLib;
    SerialPortParameter parSerialLib;
    if(!canSerialPortPar(&parLib, &parSerialLib)){
        return false;
    }
    parSerial.baudrate = m_serailBaudrates.at(parSerialLib.baudrate);
    parSerial.databit = m_databits.at(parSerialLib.databit);
    parSerial.paritybit = m_paritybits.at(parSerialLib.paritybit);
    parSerial.stopbit = m_stopbits.at(parSerialLib.stopbit);
    parSerial.flow = parSerialLib.flow == 0x00 ? tr("硬件流控制") : tr("无流控制");
    parSerial.model = m_serailmodel.at(parSerialLib.model);

    parTransite.model = m_transitionModel.at(parLib.model);
    parTransite.directe = m_transitionDirecte.at(parLib.directe);
    parTransite.enableSerial = parLib.enableSerial ? tr("允许") : tr("不允许");
    parTransite.enableId = parLib.enableId ? tr("允许") : tr("不允许");
    parTransite.offset = QString::number(parLib.offset);
    parTransite.frameLen = QString::number(parLib.frameLen);
    parTransite.serialTimes = QString::number(parLib.serialTimes);
    parTransite.transiteTimes = QString::number(parLib.transiteTimes);
    return true;
}

bool HandleHid::readSerialPortCanPar(QString &baudrate, QString &id, QString &frameFormat, QList<QStringList> &filter)
{
    uchar canBaudrate;
    uint32_t canId;
    uchar canFrameFormat;
    QByteArray canFilter(512, '\0');
    if(!canSerialPortCanPar(&canBaudrate, &canId, &canFrameFormat, canFilter.data(), 512)){
        return false;
    }
    baudrate = m_canBaudrates.at(canBaudrate);
    id = uintToQString(canId);
    frameFormat = canFrameFormat ? tr("扩展帧") : tr("标准帧");
    for(auto item : QString(canFilter).split(";")) {
        if(item.trimmed().isEmpty()){continue;}
        filter.append(item.split(","));
    }
    return true;
}

bool HandleHid::upgradeDevReq(const size_t &fileSize)
{
    int re = upgradeReq(fileSize);
    if(re){
        return true;
    }
    QString error = m_upgradeStatus.value(re, "");
    emit updateStatus(error);
    return false;
}

bool HandleHid::upgradeDevData(const unsigned int index, const QByteArray &data)
{
    char *pData = const_cast<char*>(data.data());
    int re = upgradeDataReq(index, reinterpret_cast<uchar*>(pData), static_cast<size_t>(data.length()));
    if(re){
        return true;
    }
    QString error = m_upgradeStatus.value(re, "");
    emit updateStatus(error);
    return false;
}

bool HandleHid::upgradeDevStatus()
{
    int re = upgradeStatus();
    if(re){
        return true;
    }
    QString error = m_upgradeStatus.value(re, "");
    emit updateStatus(error);
    return false;
}


void HandleHid::upgradeDev(QString &filePath)
{
    int index = 0;
    QByteArray fileBuf;
    const int pkgLen = 16;
    auto file = new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly)){
        // 发送错误信息
        emit updateStatus(tr("升级文件无法打开."));
        emit updateResult(false);
        return;
    }
    fileBuf = file->readAll();
    file->close();
    emit processRange(0, fileBuf.length());
    if(!upgradeReq(static_cast<size_t>(fileBuf.length()))){
        emit updateStatus(tr("升级请应答无响应."));
        emit updateResult(false);
        return;
    }
    // 数据包流水号，从 1 开始累加
    uint32_t pkgIndex = 1;
    while (true) {
        if(index < fileBuf.length()){
            for (int retry=0; retry<3; ++retry) {
                auto buf = fileBuf.mid(index, pkgLen);
                if(upgradeDataReq(pkgIndex,
                                  reinterpret_cast<uchar*>(buf.data()),
                                  static_cast<size_t>(buf.length()))){
                    index += buf.length();
                    pkgIndex += 1;
                    break;
                }else{
                    if(retry >= 2){
                        emit updateStatus(tr("升级包发送失败，停止升级."));
                        emit updateResult(false);
                        return;
                    }
                }
            }
            qDebug() << "upgrade: " << index;
            emit processBarV(index);
        }else{
            // 处理升级结果, 兼容两个不同版本的协议，最后文件校验不去处理结果
            upgradeStatus();
            qDebug() << "upgrade end.";
            break;
        }
    }
    emit updateResult(true);
    qDebug() << "upgrade index: " << index << " sum len: " << fileBuf.length();
}


void HandleHid::pushToTask(HidTask type)
{
    qDebug() << QThread::currentThreadId();
    QMutexLocker locker(&m_mutex);
    m_queue.enqueue(QPair<HidTask, HidTaskPar>(type, HidTaskPar()));
}

void HandleHid::pushToTask(HidTask type, HidTaskPar par)
{
    QMutexLocker locker(&m_mutex);
    m_queue.enqueue(QPair<HidTask, HidTaskPar>(type, par));
}

void HandleHid::exitWorkThread()
{
    m_isExit = true;
}

void HandleHid::run()
{
    if(!initLib()){
        // 初始化失败
        emit isInitError(tr("动态库初始化错误, 请检查程序的完整性!"));
        return;
    }
    while (true) {
        try {
            if(m_isExit){ break; }

            this->readFromCan();
            QPair<HidTask, HidTaskPar> task;
            if(m_queue.isEmpty()){
                QThread::usleep(1);
                continue;
            }else{
                QMutexLocker locker(&m_mutex);
                task = m_queue.dequeue();
            }
            if (task.first == HidTask::OPEN_HID) {
                // 打开HID
                QString version;
                auto par = task.second;
                bool re = this->openHid(par.m_vid, par.m_pid, par.m_serialNumber);
                if(re && openCan() && initCan(version) && enableRcvCan(true)){
                    emit isOpenHid(true, version);
                }else{
                    emit isOpenHid(false, "V***");
                }
            } else if (task.first == HidTask::CLOSE_HID) {
                // 关闭HID
                this->closeHid();
                emit isCloseHid();
            } else if (task.first == HidTask::IS_ALIVE_HID) {
                // HID 是否在线
                if(m_queue.isEmpty()){
                    // 如果队列还有任务则不去查询，只有空闲才查询在线情况
                    emit isAliveStatus(isAlive());
                }
            } else if (task.first == HidTask::SET_MODE_CAN) {
                // 设置CAN模式
                emit isCanSetMode(this->setCanModel(task.second.m_canModel));
            } else if (task.first == HidTask::RESET_CAN) {
                // 重置CAN
                emit isCanReset(this->resetCan());
            } else if (task.first == HidTask::RESET_ERROR_CAN) {
                // 重置CAN设备错误状态
                emit isCanResetError(this->resetCanError());
            } else if (task.first == HidTask::SET_PAR_CAN) {
                // 设置CAN设备参数
                emit isCanSetPar(this->setCanBaudrate(task.second.m_baudrate));
            } else if (task.first == HidTask::WRITE_CAN) {
                // 向CAN设备写数据
                emit numCanWrite(this->writeToCan(task.second.m_id,
                                                task.second.m_frameType,
                                                task.second.m_frameFormat,
                                                task.second.m_data),
                                 task.second.m_id,
                                 task.second.m_frameType,
                                 task.second.m_frameFormat,
                                 task.second.m_data,
                                 QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
            } else if (task.first == HidTask::UPDATE_DEVICE) {
                // 升级设备
                upgradeDev(task.second.m_updateFilePath);
            } else if (task.first == HidTask::FILTER_ID) {
                // 设置过滤规则
                emit isFilterId(this->setCanFilterId(task.second.filterData));
            } else if (task.first == HidTask::CAN_PAR){
                // 读取CAN设备参数
                QString baudrate;
                QList<QStringList> filter;
                emit isCanParStatus(this->readCanPar(baudrate,filter));
                emit isCanPar(baudrate, filter);
            } else if (task.first == HidTask::CLEAR_FILTER_ID) {
                // 清空过滤规则
                emit isClearFilterId(this->clearCanFilterId());
            } else if(task.first == HidTask::SET_TRANSITION_PAR_SERIAL){
                // 设置串口端转换参数
                TransiteParameterUi par{
                    task.second.transitionModel,
                    task.second.transitionDirecte,
                    task.second.enableSerial,
                    task.second.enableID,
                    task.second.serialOffset,
                    task.second.serialFramLen,
                    task.second.serialTimes,
                    task.second.transitionTimes,
                };
                emit isTransitionParCan(this->setSerialPortTransitionPar(par));
            } else if(task.first == HidTask::SET_PAR_SERIAL){
                // 设置串口参数
                SerialPortParameterUi par{
                    task.second.bauderateSerial,
                    task.second.dataBit,
                    task.second.parityBit,
                    task.second.stopBit,
                    task.second.flowControl,
                    task.second.serailModel
                };
                emit isSerialPar(this->setSerialPortPar(par));
            } else if(task.first == HidTask::MCU_PAR){
                // 读取mcu的参数
                // 读取串口参数
                TransiteParameterUi parTransite;
                SerialPortParameterUi parSerial;
                bool re = this->readSerialPortPar(parTransite, parSerial);
                emit isRMcuPar(parTransite, parSerial);
                // 读取串口端CAN参数
                QString baudrate; QString id; QString frameFormat; QList<QStringList> filter;
                re &= this->readSerialPortCanPar(baudrate, id, frameFormat, filter);
                emit isRSerialPar(re, baudrate, id, frameFormat, filter);
            } else if(task.first == HidTask::SET_CAN_SERIAL){
                // 设置串口端CAN参数
                emit isParCan(this->setSerialPortCanPar(
                                  task.second.bauterateCan,
                                  task.second.canID,
                                  task.second.frameFormat));
            } else if(task.first == HidTask::SCAN_HID){
                qDebug() << QThread::currentThreadId();
                scanHidDev();
            } else {
                ;
            }

        } catch (...) {
            // 出现异常就丢弃结果，继续运行
            continue;
        }
    }
    // 关闭CAN，关闭hid
    this->closeCan();
    this->closeHid();
    freeLib();
}

void HandleHid::scanHidDev()
{
    qDebug() << QThread::currentThreadId();
    QList<QStringList> re;
    hid_device_info *devs = enumHidDevs();
    hid_device_info *tmpDevs = devs;
    while (devs) {
        QStringList info;
        QString manu(QString::fromWCharArray(devs->manufacturer_string));
        QString serial(QString::fromWCharArray(devs->serial_number));
        info.append(QString::asprintf("0x%04X", devs->vendor_id));
        info.append(QString::asprintf("0x%04X", devs->product_id));
        info.append(serial);
        info.append(manu.toUpper());
        info.append(QString::fromWCharArray(devs->product_string));
        re.append(info);
        devs = devs->next;
    }
    freeEnumHidDevs(tmpDevs);
    emit isScanHidDev(re);
}









