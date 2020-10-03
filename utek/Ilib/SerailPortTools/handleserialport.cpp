#include "handleserialport.h"
#include <QDateTime>
#include <QDebug>


const QStringList HandleSerialPort::m_baudRateKeys = {
    "600", "1200", "2400", "4800", "9600", "19200", "38400",
    "57600", "115200", "230400",  "460800", "921600",
    "1843200", "3686400",  "7812500", "8928571",
    "10416666", "12500000", "15625000"
};
const QMap<QString, QSerialPort::Parity> HandleSerialPort::m_parityMap = {
    {QObject::tr("无校验"), QSerialPort::NoParity},
    {QObject::tr("偶检验"), QSerialPort::EvenParity},
    {QObject::tr("奇校验"), QSerialPort::OddParity},
    {QObject::tr("空格"), QSerialPort::SpaceParity},
    {QObject::tr("标记"), QSerialPort::MarkParity}
};
const QMap<QString, QSerialPort::DataBits> HandleSerialPort::m_dataBitsMap = {
    {"5", QSerialPort::Data5},
    {"6", QSerialPort::Data6},
    {"7", QSerialPort::Data7},
    {"8", QSerialPort::Data8}
};
const QMap<QString, QSerialPort::StopBits> HandleSerialPort::m_stopBitsMap = {
    {"1", QSerialPort::OneStop},
    {"1.5", QSerialPort::OneAndHalfStop},
    {"2", QSerialPort::TwoStop}
};
const QMap<QString, QSerialPort::FlowControl> HandleSerialPort::m_flowControlMap = {
    {QObject::tr("无"), QSerialPort::NoFlowControl},
    {QObject::tr("硬件流控"), QSerialPort::HardwareControl},
    {QObject::tr("软件流控"), QSerialPort::SoftwareControl}
};


HandleSerialPort::HandleSerialPort(QObject *parent)
    : QObject(parent),
      m_pSerial(new QSerialPort),
      m_pPinoutTimer(new QTimer(this))
{
    for(auto baudRate: m_baudRateKeys){
        m_baudRateMap.insert(baudRate, static_cast<qint32>(baudRate.toInt()));
    }
    connect(m_pPinoutTimer, &QTimer::timeout, this, [this](){
        auto pinout = m_pSerial->pinoutSignals();
        if(m_pSerial->isWritable()){
            pinout |= QSerialPort::TransmittedDataSignal;
        }
        if(m_pSerial->isReadable()){
            pinout |= QSerialPort::ReceivedDataSignal;
        }
        emit serialPortStatus(static_cast<uint>(pinout));
    });
}
HandleSerialPort::~HandleSerialPort()
{
    if(m_pSerial->isOpen()){
        m_pSerial->close();
        delete m_pSerial;
        m_pSerial = nullptr;
    }
}

QStringList HandleSerialPort::getSerialPortNames()
{
    QStringList names;
    for(auto &info : QSerialPortInfo::availablePorts()){
        if(!info.isBusy()){
            names.append(info.portName());
        }
    }
    std::sort(names.begin(), names.end(), [](QString pre, QString item) -> bool{
        if(pre.length() == item.length()){
            QString preNums, itemNums;
            for (int i=0; i<pre.length(); ++i) {
                if(pre.at(i).isDigit()){ preNums.append(pre.at(i)); }
                if(item.at(i).isDigit()){ itemNums.append(item.at(i)); }
            }
            return preNums.toInt() < itemNums.toInt();
        }else if(pre.length() < item.length()){
            return true;
        }
        return false;
    });
    return names;
}

bool HandleSerialPort::setBaudRate(QString baudRate)
{
    qint32 _baudRate = 0;
    if(m_baudRateMap.keys().contains(baudRate)){
        _baudRate = m_baudRateMap.value(baudRate);
    }else{
        _baudRate = static_cast<qint32>(baudRate.toInt());
        m_baudRateMap.insert(baudRate, _baudRate);
    }
    return m_pSerial->setBaudRate(_baudRate);
}

QStringList HandleSerialPort::getBaudRateKeys()
{
    return HandleSerialPort::m_baudRateKeys;
}

bool HandleSerialPort::setParity(QString parity)
{
    return m_pSerial->setParity(HandleSerialPort::m_parityMap.value(parity));
}

QStringList HandleSerialPort::getParityKeys()
{
    return HandleSerialPort::m_parityMap.keys();
}

bool HandleSerialPort::setDataBits(QString dataBits)
{
    return m_pSerial->setDataBits(HandleSerialPort::m_dataBitsMap.value(dataBits));
}

QStringList HandleSerialPort::getDataBitsKeys()
{
    return HandleSerialPort::m_dataBitsMap.keys();
}

bool HandleSerialPort::setStopBits(QString stopBits)
{
    return m_pSerial->setStopBits(HandleSerialPort::m_stopBitsMap.value(stopBits));
}

QStringList HandleSerialPort::getStopBitsKeys()
{
    return HandleSerialPort::m_stopBitsMap.keys();
}

bool HandleSerialPort::setFlowControl(QString flowControl)
{
    return m_pSerial->setFlowControl(HandleSerialPort::m_flowControlMap.value(flowControl));
}

QStringList HandleSerialPort::getFlowControlKeys()
{
    return HandleSerialPort::m_flowControlMap.keys();
}

void HandleSerialPort::onReadData()
{
    qDebug() << "Serial Port Bytes Available: " << m_pSerial->bytesAvailable()
             << " m_needLen: " << m_needLen;
    while (m_pSerial->bytesAvailable() >= m_needLen) {
        QByteArray data = m_pSerial->readAll();
        QString buf = m_outHex ? data.toHex(m_setparator.toLatin1()).toUpper() : QString(data);
        if(m_outDateTime){
            auto now = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
            buf = "\r\n[" + now + "]\r\n" + buf;
        }
        qDebug() << data.toHex(m_setparator.toLatin1()).toUpper();
        emit readyReadString(buf, data.length());
        emit readyReadByte(data);
    }
}

qint64 HandleSerialPort::writeHex(const QString &msg, QChar separator)
{
    if(msg.isEmpty()){return 0;}
    auto buf = QByteArray::fromHex(msg.trimmed().replace(separator, "").toLocal8Bit());
    qDebug() << buf;
    auto re = m_pSerial->write(buf);
    if(re <= 0){
        emit writeError(m_pSerial->errorString());
    }
    return re;
}

qint64 HandleSerialPort::writeAscii(const QString &msg)
{
    if(msg.isEmpty()){return 0;}
    qDebug() << msg.toLatin1();
    auto re = m_pSerial->write(msg.toLatin1());
    if(re <= 0){
        emit writeError(m_pSerial->errorString());
    }
    return re;
}

qint64 HandleSerialPort::writeByte(const QByteArray &bytes)
{
    if(bytes.isEmpty()){return 0;}
    auto re = m_pSerial->write(bytes);
    if(re <= 0){
        emit writeError(m_pSerial->errorString());
    }
    return re;
}
qint64 HandleSerialPort::writeByte(uchar *data, int size)
{
    if(size <= 0){return 0;}
    QByteArray buf(reinterpret_cast<char*>(data), size);
    auto re = this->writeByte(buf);
    if(re <= 0){
        emit writeError(m_pSerial->errorString());
    }
    return re;
}

bool HandleSerialPort::serialPortOpen(QString portName)
{
    if(!portName.isEmpty()){
        m_pSerial->setPortName(portName);
    }
    if(!m_pSerial->isOpen()){
        if(!m_isBlock){
            connect(m_pSerial, &QSerialPort::readyRead, this, &HandleSerialPort::onReadData);
        }
        bool re = m_pSerial->open(QIODevice::ReadWrite);
        auto pinout = m_pSerial->pinoutSignals();
        if(m_pSerial->isWritable()){
            pinout |= QSerialPort::TransmittedDataSignal;
        }
        if(m_pSerial->isReadable()){
            pinout |= QSerialPort::ReceivedDataSignal;
        }
        emit serialPortStatus(static_cast<uint>(pinout));
        if(re)(m_pPinoutTimer->start(PINOUT_TIMES));
        return re;
    }
    return true;
}

void HandleSerialPort::serialPortClose()
{
    if(m_pSerial->isOpen()){
        m_pSerial->close();
        m_pPinoutTimer->stop();
    }
}

void HandleSerialPort::setPortName(QString portName)
{
    m_pSerial->setPortName(portName);
}

void HandleSerialPort::setBlock(bool isBlock)
{
    m_isBlock = isBlock;
}

QByteArray HandleSerialPort::reqAndResponseBlock(const QByteArray &bytes, int timeout)
{
    if(bytes.isEmpty()){return QByteArray();}
    // 在单次通信之前清空所有缓存和错误
    m_pSerial->clear(QSerialPort::AllDirections);
    m_pSerial->clearError();
    m_pSerial->write(bytes);
    if(m_pSerial->waitForBytesWritten(timeout)){
        if(m_pSerial->waitForReadyRead(timeout)){
            QByteArray responseData = m_pSerial->readAll();
            while(m_pSerial->waitForReadyRead(10)){
                responseData += m_pSerial->readAll();
            }
            return responseData;
        }
    }
    return QByteArray();
}



