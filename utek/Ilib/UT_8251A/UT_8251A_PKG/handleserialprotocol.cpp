#include <QThread>
#include <QFile>
#include <QDebug>
#include "handleserialprotocol.h"
#include "handlehid.h"

const int SLEEP_TIME = 300;

HandleSerialProtocol::HandleSerialProtocol(QObject *parent)
    : QObject(parent),
      m_pHandleSerial(new HandleSerialPort)
{
    connect(m_pHandleSerial.get(), &HandleSerialPort::readyReadByte, this, &HandleSerialProtocol::onReadyBytes);
    connect(&m_reqTimer, &QTimer::timeout, this, [this](){
        if(m_isReq){
            if(QDateTime::currentMSecsSinceEpoch() - m_reqTime > 1000){
                // 超时信号
                this->setTimeOutEnable();
                if(m_isAlive){
                    emit isAliveCan(false);
                    return;
                }
                if(m_isUpgrade){
                    emit updateStatus(tr("升级等待超时."));
                }else{
                    emit readTimeout();
                }
            }
        }
    });
}
HandleSerialProtocol::~HandleSerialProtocol()
{
    if(m_pHandleSerial->isOpen()){
        m_pHandleSerial->serialPortClose();
    }
}

void HandleSerialProtocol::onReadyBytes(QByteArray bytes)
{
    if(static_cast<uint>(bytes.length()) < sizeof(GeneralPkgCom)){return;}
    GeneralPkgCom *pAck = reinterpret_cast<GeneralPkgCom *>(bytes.data());
    switch (pAck->cmd) {
    case 0x01:
        this->onSetTransitionPar(bytes);
        setTimeOutEnable();
        break;
    case 0x02:
        this->onSetCanPar(bytes);
        setTimeOutEnable();
        break;
    case 0x03: // 读取过滤和清空过滤
        this->onSetFilter(bytes);
        setTimeOutEnable();
        break;
    case 0x04:
        this->onSetSerialPar(bytes);
        setTimeOutEnable();
        break;
    case 0x05:
        this->onOpenCan(bytes);
        setTimeOutEnable();
        break;
    case 0x06:
        this->onReadMcuConfig(bytes);
        setTimeOutEnable();
        break;
    case 0x07:
        this->onInitCan(bytes);
        setTimeOutEnable();
        break;
    case 0x08:
        this->onIsAlive(bytes);
        setTimeOutEnable();
        break;
    case 0x09:
        this->onUpgradeDevReq(bytes);
        break;
    case 0x0A:
        this->onUpgradeDevData(bytes);
        break;
    case 0x0B:
        this->onUpgradeDevStatus(bytes);
        break;
    default:
        return;
    }
}

bool HandleSerialProtocol::open(SerialTaskPar &par)
{
    bool re = m_pHandleSerial->serialPortOpen(par.com);
    if(re){
        re &= m_pHandleSerial->setBaudRate(par.bauderateSerial);
        re &= m_pHandleSerial->setDataBits(par.dataBit);
        re &= m_pHandleSerial->setStopBits(par.stopBit);
        re &= m_pHandleSerial->setParity(par.parityBit);
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgCom));
        m_reqTimer.start(1000);
        return re;
    }
    return false;
}

void HandleSerialProtocol::close()
{
    m_pHandleSerial->serialPortClose();
    m_reqTimer.stop();
    emit isAliveCan(false);
}

unsigned char HandleSerialProtocol::checkSum(unsigned char *buff, int len)
{
    unsigned int sum = 0;
    for(int i = 0; i < len; ++i){
        sum += buff[i];
    }
    sum = ~sum + 1;
    return static_cast<unsigned char>(sum);
}


bool HandleSerialProtocol::setTransitionPar(SerialTaskPar &par)
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        TransitionParamREQCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(TransitionParamREQCom) - 2;
        req.cmd = 0xFE;
        req.transitionModel = static_cast<uchar>(transitionModelList.indexOf(par.transitionModel));
        req.transitionDirecte = static_cast<uchar>(transitionDirecteList.indexOf(par.transitionDirecte));
        req.enableSerial = par.enableSerial == tr("不允许") ? 0x00 : 0x01;
        req.enableID = par.enableID == tr("不允许") ? 0x00 : 0x01;
        req.serialOffset = static_cast<uchar>(par.serialOffset);
        req.serialFramLen = static_cast<uchar>(par.serialFramLen);
        req.serialTimes = static_cast<uchar>(par.serialTimes);
        req.transitionTimes = static_cast<uchar>(par.transitionTimes);
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(TransitionParamREQCom)-1] = this->checkSum(pReq+2, sizeof(TransitionParamREQCom)-3);
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgReCom));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof(TransitionParamREQCom));
        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        return re;
    } catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::setCanPar(SerialTaskPar &par)
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        CANParamREQCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(CANParamREQCom) - 2;
        req.cmd = 0xFD;
        req.bauterate = static_cast<uchar>(m_baudrates.indexOf(par.bauterateCan));
        unsigned int idUInt = stringToUint(par.canID);
        req.canID[0] = (idUInt & 0xFF000000) >> 24;
        req.canID[1] = (idUInt & 0x00FF0000) >> 16;
        req.canID[2] = (idUInt & 0x0000FF00) >> 8;
        req.canID[3] = (idUInt & 0x000000FF);
        req.frameFormat = par.frameFormat == tr("标准帧") ? 0x00 : 0x01;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(CANParamREQCom)-1] = this->checkSum(pReq+2, sizeof(CANParamREQCom)-3);
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgReCom));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof (CANParamREQCom));
        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        return re;
    } catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::setFilter(SerialTaskPar &par)
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        auto filteridCount = static_cast<decltype(sizeof(CANParamREQCom))>(par.filterData.size());
        CANFilterREQCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = static_cast<uchar>(filteridCount * sizeof(FilterData) + 3);
        req.cmd = 0xFC;
        QByteArray buf;
        buf.append(static_cast<char>(req.header[0]));
        buf.append(static_cast<char>(req.header[1]));
        buf.append(static_cast<char>(req.len));
        buf.append(static_cast<char>(req.cmd));
        for (auto info: par.filterData) {
            uint startId = stringToUint(info.startID);
            uint endId = stringToUint(info.endID);
            buf.append(static_cast<char>(idFilterTypeList.indexOf(info.idFilterType)));
            buf.append(static_cast<char>((startId & 0xFF000000)>>24));
            buf.append(static_cast<char>((startId & 0x00FF0000)>>16));
            buf.append(static_cast<char>((startId & 0x0000FF00)>>8));
            buf.append(static_cast<char>(startId & 0x000000FF));
            buf.append(static_cast<char>((endId& 0xFF000000)>>24));
            buf.append(static_cast<char>((endId& 0x00FF0000)>>16));
            buf.append(static_cast<char>((endId& 0x0000FF00)>>8));
            buf.append(static_cast<char>(endId& 0x000000FF));
        }
        char *pbuf = buf.data();
        uchar check = this->checkSum(reinterpret_cast<uchar *>(pbuf+2), buf.size()-2);
        buf.append(static_cast<char>(check));
        m_isSetFilter = true;
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgReCom));
        auto re = m_pHandleSerial->writeByte(reinterpret_cast<uchar*>(pbuf), buf.size());
        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        return re;
    } catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::clearfilter()
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        GeneralPkgCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(GeneralPkgCom) - 2;
        req.cmd = 0xFC;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        req.checkSum = this->checkSum(pReq+2, sizeof(GeneralPkgCom)-3);
        m_isSetFilter = false;
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgReCom));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof(GeneralPkgCom));
        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        return re;
    } catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::setSerialPar(SerialTaskPar &par)
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        USARTParamREQCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(USARTParamREQCom) - 2;
        req.cmd = 0xFB;
        req.bauderate = static_cast<uchar>(m_serailBaudrates.indexOf(par.bauderateSerial));
        req.dataBit = static_cast<uchar>(databitslist.indexOf(par.dataBit));
        req.parityBit = static_cast<uchar>(paritybitslist.indexOf(par.parityBit));
        req.stopBit = static_cast<uchar>(stopbitslist.indexOf(par.stopBit));
        req.flowControl = par.flowControl == tr("硬件流控制") ? 0x00 : 0x01;
        req.serailModel = static_cast<uchar>(serailmodellist.indexOf(par.serailModel));

        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(USARTParamREQCom)-1] = this->checkSum(pReq+2, sizeof(USARTParamREQCom)-3);
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgReCom));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof (USARTParamREQCom));
        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        return re;
    } catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::readMcuConfig()
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        GeneralPkgCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(GeneralPkgCom) - 2;
        req.cmd = 0xF9;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(GeneralPkgCom)-1] = this->checkSum(pReq+2, sizeof(GeneralPkgCom)-3);
        m_pHandleSerial->setNeedLen(sizeof(USARTComfigACKCom));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof(GeneralPkgCom));
        QThread::msleep(1000);
        setTimeOutEnable(true);
        return re;
    }catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::isAlive()
{
    if(m_isAlive){return false;}
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        m_isAlive = true;
        GeneralPkgCom req;
        req.cmd = 0xF7;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(GeneralPkgCom)-1] = this->checkSum(pReq+2, sizeof(GeneralPkgCom)-3);
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgCom ));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof(GeneralPkgCom));
//        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        return re;
    } catch (...) {
        return false;
    }

}

bool HandleSerialProtocol::openCan()
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        GeneralPkgCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(GeneralPkgCom) - 2;
        req.cmd = 0xFA;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(GeneralPkgCom)-1] = this->checkSum(pReq+2, sizeof(GeneralPkgCom)-3);
        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgCom));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof(GeneralPkgCom));
        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        return re;
    } catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::initCan()
{
    if(!m_pHandleSerial->isOpen()) { return false; }
    try {
        GeneralPkgCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(GeneralPkgCom) - 2;
        req.cmd = 0xF8;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(GeneralPkgCom)-1] = this->checkSum(pReq+2, sizeof(GeneralPkgCom)-3);
        m_pHandleSerial->setNeedLen(sizeof(USARTInitInfoREQCom));
        auto re = m_pHandleSerial->writeByte(pReq, sizeof(GeneralPkgCom));
        QThread::msleep(SLEEP_TIME);
        setTimeOutEnable(true);
        emit isAliveCan(re);
        return re;
    } catch (...) {
        return false;
    }
}

bool HandleSerialProtocol::onSetTransitionPar(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgReCom *pRead = reinterpret_cast<GeneralPkgReCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x01 || pRead->re == 0x00){
        emit isTransitionParCan(false);
        return false;
    }
    emit isTransitionParCan(true);
    return true;
}

bool HandleSerialProtocol::onSetCanPar(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgReCom *pRead = reinterpret_cast<GeneralPkgReCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x02 || pRead->re == 0x00){
        emit isParCan(false);
        return false;
    }
    emit isParCan(true);
    return true;
}

bool HandleSerialProtocol::onSetFilter(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgReCom *pRead = reinterpret_cast<GeneralPkgReCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x03 || pRead->re == 0x00){
        if(m_isSetFilter){
            emit isFilterCan(false);
        }else{
            emit isClearFilter(false);
        }
        return false;
    }
    if(m_isSetFilter){
        emit isFilterCan(true);
    }else{
        emit isClearFilter(true);
    }
    return true;
}

bool HandleSerialProtocol::onSetSerialPar(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgReCom *pRead = reinterpret_cast<GeneralPkgReCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x04 || pRead->re == 0x00){
        emit isSerialPar(false);
        return false;
    }
    emit isSerialPar(true);
    return true;
}

bool HandleSerialProtocol::onReadMcuConfig(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pAck[bytes.length()-1] || pAck[3] != 0x06){
        emit isMcuConfig(SerialTaskPar(), false);
        return false;
    }
    // 拆包
    uchar config[sizeof(USARTComfigACKCom)]{0};// 不包含过滤ID的数据
    memcpy(config, pAck, sizeof(USARTComfigACKCom));
    USARTComfigACKCom *pConfig = reinterpret_cast<USARTComfigACKCom *>(config);
    uint pkgCount = pConfig->filterCount;
    uchar filterIdBuf[90]{0};
    memcpy(filterIdBuf, pAck+sizeof(USARTComfigACKCom), pkgCount*sizeof(FilterData));

    SerialTaskPar rst;
    for (uint i=0; i<pkgCount; ++i) {
        FilterData tmp;
        memcpy(&tmp, filterIdBuf+i*sizeof(FilterData), sizeof(FilterData));
        FilterConfig idInfo;
        idInfo.idFilterType = idFilterTypeList[tmp.idFilterType];
        idInfo.startID = QString::asprintf("%02X %02X %02X %02X", tmp.startID[0],
                tmp.startID[1], tmp.startID[2], tmp.startID[3]);
        idInfo.endID = QString::asprintf("%02X %02X %02X %02X", tmp.endID[0],
                tmp.endID[1], tmp.endID[2], tmp.endID[3]);
        rst.filterData.append(idInfo);
    }
    rst.transitionModel = transitionModelList[pConfig->transitionModel];
    rst.transitionDirecte = transitionDirecteList[pConfig->transitionDirecte];
    rst.enableSerial = pConfig->enableSerial ? tr("允许") : tr("不允许");
    rst.enableID = pConfig->enableID ? tr("允许") : tr("不允许");
    rst.serialOffset = pConfig->serialOffset;
    rst.serialFramLen = pConfig->serialFramLen;
    rst.serialTimes = pConfig->serialTimes;
    rst.transitionTimes = pConfig->transitionTimes;
    rst.bauderateSerial = m_serailBaudrates[pConfig->bauderate];
    rst.dataBit = databitslist[pConfig->dataBit];
    rst.parityBit = paritybitslist[pConfig->parityBit];
    rst.stopBit = stopbitslist[pConfig->stopBit];
    rst.flowControl = pConfig->flowControl ? tr("无流控制") : tr("硬件流控制");
    rst.serailModel = serailmodellist[pConfig->serailModel];
    rst.bauterateCan = m_baudrates[pConfig->canBauderate];
    rst.canID = QString::asprintf("%02X %02X %02X %02X", pConfig->canID[0],
            pConfig->canID[1], pConfig->canID[2], pConfig->canID[3]);
    rst.frameFormat = pConfig->frameFormat ? tr("扩展帧") : tr("标准帧");
    emit isMcuConfig(rst, true);
    return true;
}

bool HandleSerialProtocol::onIsAlive(QByteArray &bytes)
{
    setTimeOutEnable();
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgCom *pRead = reinterpret_cast<GeneralPkgCom*>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x08){
        emit isAliveCan(false);
        return false;
    }
    // 发送信号
    m_isAlive = false;
    emit isAliveCan(true);
    return true;
}

bool HandleSerialProtocol::onOpenCan(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgCom *pRead = reinterpret_cast<GeneralPkgCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x05){
        emit isOpenCan(false);
        return false;
    }
    emit isOpenCan(true);
    return true;
}

bool HandleSerialProtocol::onInitCan(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    USARTInitInfoREQCom *pRead = reinterpret_cast<USARTInitInfoREQCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x07){
        emit isInitCan("V****", false);
        return false;
    }
    QString version = QString::asprintf("V%c.%c", pRead->mVer, pRead->pVer);
    emit isInitCan(version, true);
    return true;
}

bool HandleSerialProtocol::upgradeDev(QString filePath)
{
    m_fileBuf.clear();
    m_pkgIndex = 1;
    m_index = 0;
    auto file = new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly)){
        // 发送错误信息
        emit updateStatus(tr("升级文件无法打开."));
        emit updateResult(false);
        return false;
    }
    m_fileBuf = file->readAll();
    file->close();
    emit processRange(0, m_fileBuf.length());
    m_isUpgrade = true;
    return this->upgradeDevReq();
}

bool HandleSerialProtocol::upgradeDevReq()
{
    unsigned int fileLen = static_cast<unsigned int>(m_fileBuf.length());
    UpgradeREQCom req;
    req.header[0] = 0x2E; req.header[1] = 0x30;
    req.len = sizeof(UpgradeREQCom) - 2;
    req.cmd = 0xF6;
    req.fileSize[0] = (fileLen & 0xFF000000) >> 24;
    req.fileSize[1] = (fileLen & 0x00FF0000) >> 16;
    req.fileSize[2] = (fileLen & 0x0000FF00) >> 8;
    req.fileSize[3] = (fileLen & 0x000000FF);
    uchar *pReq = reinterpret_cast<uchar *>(&req);
    pReq[sizeof(UpgradeREQCom)-1] = this->checkSum(pReq+2, sizeof(UpgradeREQCom)-3);
    m_pHandleSerial->setNeedLen(sizeof(GeneralPkgCom ));
    qint64 re = m_pHandleSerial->writeByte(pReq, sizeof(UpgradeREQCom));
    QThread::msleep(100);
    if(!re){
        emit updateStatus(tr("升级请求发送失败，停止升级."));
    }
    setTimeOutEnable(true);
    return re;
}

bool HandleSerialProtocol::upgradeDevData()
{
    if(m_index < m_fileBuf.length()){
        auto buf = m_fileBuf.mid(m_index, m_pkgLen);
        USARTIapDataREQCom dataReq;
        dataReq.header[0] = 0x2E;
        dataReq.header[1] = 0x30;
        dataReq.len = sizeof(USARTIapDataREQCom ) - 2;
        dataReq.cmd = 0xF5;
        dataReq.id[0] = (m_pkgIndex & 0xFF000000) >> 24;
        dataReq.id[1] = (m_pkgIndex & 0x00FF0000) >> 16;
        dataReq.id[2] = (m_pkgIndex & 0x0000FF00) >> 8;
        dataReq.id[3] = m_pkgIndex & 0x000000FF;
        dataReq.pkgLen = static_cast<uchar>(buf.length());
        memcpy(dataReq.data, buf.data(), static_cast<size_t>(buf.length()));
        uchar *pReq = reinterpret_cast<uchar *>(&dataReq);
        pReq[sizeof(USARTIapDataREQCom)-1] = this->checkSum(pReq+2, sizeof(USARTIapDataREQCom)-3);

        m_pHandleSerial->setNeedLen(sizeof(GeneralPkgCom));
        qint64 re = m_pHandleSerial->writeByte(pReq, sizeof(USARTIapDataREQCom));
        setTimeOutEnable(true);
        QThread::msleep(100);
        m_pkgIndex++;
        m_index += buf.length();
        if(!re){
            emit updateStatus(tr("升级请求发送失败，停止升级."));
            return false;
        }
        emit processBarV(m_index);
        qDebug()<< "m_index: " << m_index << " sum: " << m_fileBuf.length()
                << " m_isReq: " << m_isReq;
        return re;
    }else{
        m_isUpgrade = false;
        setTimeOutEnable(false);
    }
    return false;
}

bool HandleSerialProtocol::onUpgradeDevReq(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgCom *pRead = reinterpret_cast<GeneralPkgCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x09){
        QString code = QString::asprintf("r%02X:c%02X", rCheckSum, pRead->checkSum);
        emit updateStatus(tr("升级请求响应, 校验错误(%1), 停止升级.").arg(code));
        return false;
    }
    // 发送第一包数据
    return this->upgradeDevData();
}

bool HandleSerialProtocol::onUpgradeDevData(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgCom *pRead = reinterpret_cast<GeneralPkgCom *>(bytes.data());
    uchar rCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(rCheckSum != pRead->checkSum || pRead->cmd != 0x0A){
        QString code = QString::asprintf("r%02X:c%02X", rCheckSum, pRead->checkSum);
        emit updateStatus(tr("数据发送响应, 校验错误(%1), 停止升级.").arg(code));
        return false;
    }
    return this->upgradeDevData();
}

bool HandleSerialProtocol::onUpgradeDevStatus(QByteArray &bytes)
{
    uchar *pAck = reinterpret_cast<uchar*>(bytes.data());
    GeneralPkgReCom *pRe = reinterpret_cast<GeneralPkgReCom *>(bytes.data());
    uchar reCheckSum = this->checkSum(pAck+2, static_cast<int>(bytes.length()-3));
    if(reCheckSum != pRe->checkSum || pRe->cmd != 0x0B || pRe->re == 0x00){
        return false;
    }
    GeneralPkgCom reAck;
    reAck.header[0] = 0x2E; reAck.header[1] = 0x30;
    reAck.len = sizeof(GeneralPkgCom) - 2;
    reAck.cmd = 0xF4;
    uchar *pReAck = reinterpret_cast<uchar *>(&reAck);
    pReAck[sizeof(GeneralPkgCom)-1] = this->checkSum(pReAck+2, sizeof(GeneralPkgCom)-3);
    if(m_pHandleSerial->writeByte(pReAck, sizeof(GeneralPkgCom)) <=0){
        return false;
    }
    emit updateResult(true);
    return true;
}







