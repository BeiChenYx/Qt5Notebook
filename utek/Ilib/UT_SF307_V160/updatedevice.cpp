#include <QFile>
#include "updatedevice.h"

UpdateDevice::UpdateDevice(QObject *parent, QString ip, QString fileName)
    : QObject(parent),
      m_ip(ip),
      m_fileName(fileName)
{

}

void UpdateDevice::onUpdateDevice()
{
    if(!getFileData()){ return; }
    auto isC = connectServer();
    emit isConnected(isC);
    if(!isC){ return; }
    emit progressBarValue(1);
    if(!updateStart()){
        disConnectServer();
        return;
    }
    emit progressBarValue(2);
    if(!sendFile()){
        disConnectServer();
        return;
    }
    updateEnd();
    disConnectServer();
    emit progressBarValue(m_pageCount + 3);
    emit progressBarText(tr("升级任务结束"));
}

bool UpdateDevice::getFileData()
{
    m_fileData.clear();
    QFile bin(m_fileName);
    if(!bin.open(QIODevice::ReadOnly)){
        emit progressBarText(tr("bin文件无法打开"));
        return false;
    }
    auto buf = bin.readAll();
    for(auto ch : buf){
        m_fileData.append(ch);
    }
    bin.close();
    int quotient = static_cast<int>(m_fileData.length() / m_pageDataLen);
    int remainder = static_cast<int>(m_fileData.length() % m_pageDataLen);
    m_pageCount = quotient + (remainder > 0 ? 1 : 0);
    m_pageNo = 1;
    emit progressBarRange(0, m_pageCount + 3);
    return true;
}

bool UpdateDevice::updateStart()
{
    m_pClientSockt->flush();
    StartReq req;
    if(m_pClientSockt->write(reinterpret_cast<char*>(&req), sizeof(StartReq)) != sizeof(StartReq)){
        emit progressBarText(tr("升级请求发送错误"));
        return false;
    }
    int count = 3;
    while (count--) {
        if(!m_pClientSockt->waitForReadyRead(300)){
            continue;
        }
        if(static_cast<ulong>(m_pClientSockt->bytesAvailable()) < sizeof(UpdateAck)){
            continue;
        }
        auto result = m_pClientSockt->readAll();
        auto pAck = reinterpret_cast<UpdateAck*>(result.data());
        auto tmp = result.mid(4, sizeof(UpdateAck) - 6);
        auto crcTmp = usMBCRC16(tmp);
        uint crcRe = pAck->crc[0];
        crcRe = (crcRe << 8) | pAck->crc[1];
        // 验证校验码，数据头，功能码，子功能码，数据应答
        if(crcRe == crcTmp && pAck->header[0] == 0x5A && pAck->header[1] == 0xA5
                && pAck->cmd == 0x02 && pAck->childCmd == 0x01 && pAck->data == 0x01){
            return true;
        }else{
            emit progressBarText(tr("升级请求应答验证或校验错误"));
        }
    }
    emit progressBarText(tr("等待升级请求应答超时"));
    return false;
}

bool UpdateDevice::sendFile()
{
    if(m_fileData.isEmpty()){return false;}
    while (!m_fileData.isEmpty()) {
        int step = m_fileData.length() < m_pageDataLen ? m_fileData.length() : m_pageDataLen;
        auto tmp = m_fileData.mid(0, step);
        QByteArray pkg(tmp.data(), step);
        // 重发机制
        int count = 3;
        while (count--) {
            if(this->sendFilePage(pkg) > 0){
                m_fileData.remove(0, step);
                m_pageNo++;
                emit progressBarValue(static_cast<int>(m_pageNo) + 1);
                break;
            }else {
                emit progressBarText(tr("文件发送应答验证或校验错误"));
            }
            if(!count){
                emit progressBarText(tr("文件包发送中断"));
                return false;
            }
        }
    }
    return true;
}

qint64 UpdateDevice::sendFilePage(const QByteArray &bytearray)
{
    // 协议封装
    // 包长度: 功能码，子功能码，总包数，序号，数据，校验
    auto len = bytearray.length() + 8;
    QByteArray buf;
    // 帧头
    buf.append(char(0x5A));
    buf.append(char(0xA5));
    // 数据长度
    buf.append(char(len >> 8));
    buf.append(char(len & 0x00FF));
    // 功能码
    buf.append(0x02);
    // 子功能码
    buf.append(0x02);
    // 总包数
    buf.append(char(m_pageCount >> 8));
    buf.append(char(m_pageCount & 0x00FF));
    // 序号
    buf.append(char(m_pageNo >> 8));
    buf.append(char(m_pageNo & 0x00FF));
    // 数据区域
    buf.append(bytearray);
    // 校验码
    auto tmp = buf.mid(4);
    auto crc = usMBCRC16(tmp);
    buf.append(char(crc & 0xFF));
    buf.append(char((crc >> 8) & 0xFF));
    m_pClientSockt->flush();
    if(m_pClientSockt->write(buf.data(), buf.size()) != buf.size()){
        emit progressBarText(tr("文件包发送错误"));
        return -1;
    }

    m_pClientSockt->flush();
    // 发完后处理回应
    if(!m_pClientSockt->waitForReadyRead(1000)){
        return -1;
    }
    if(static_cast<ulong>(m_pClientSockt->bytesAvailable()) < sizeof(UpdateAck)){
        return -1;
    }
    auto result = m_pClientSockt->readAll();
    auto pAck = reinterpret_cast<UpdateAck*>(result.data());
    auto tmpRe = result.mid(4, sizeof(UpdateAck) - 6);
    auto crcTmp = usMBCRC16(tmpRe);
    uint crcRe = pAck->crc[0];
    crcRe = (crcRe << 8) | pAck->crc[1];
    // 验证校验码，数据头，功能码，子功能码，数据应答
    if(crcRe == crcTmp && pAck->header[0] == 0x5A && pAck->header[1] == 0xA5
            && pAck->cmd == 0x02 && pAck->childCmd == 0x02 && pAck->data == 0x01){
        // 返回已经发送的文件数据长度
        return bytearray.length();
    }
    return -1;
}

bool UpdateDevice::updateEnd()
{
    EndReq req;
    if(m_pClientSockt->write(reinterpret_cast<char*>(&req), sizeof(EndReq)) != sizeof(EndReq)){
        emit progressBarText(tr("升级完成命令发送错误"));
        return false;
    }
    return true;
}

bool UpdateDevice::connectServer()
{
    m_pServerIp = std::make_shared<QHostAddress>();
    m_pClientSockt = std::make_shared<QTcpSocket>();
    connect(m_pClientSockt.get(), &QTcpSocket::disconnected, this, &UpdateDevice::isDisconnected);
    m_pServerIp->setAddress(m_ip);
    this->m_pClientSockt->connectToHost(*m_pServerIp, static_cast<quint16>(m_port));
    return this->m_pClientSockt->waitForConnected(200);
}

void UpdateDevice::disConnectServer()
{
    this->m_pClientSockt->disconnectFromHost();
}

