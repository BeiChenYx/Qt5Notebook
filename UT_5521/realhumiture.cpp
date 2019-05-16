#include "realhumiture.h"


RealHumiture::RealHumiture(QObject *parent) : QObject(parent)
{

}

void RealHumiture::onHomeResult(QVariant msg)
{
    Task value = msg.value<Task>();
    int deviceAddr = value.m_deviceAddr;
    double temperature = value.ReadHolding.at(0) / 100.00;
    double humidity = value.ReadHolding.at(1) / 100.00;
    qint64 time = QDateTime::currentMSecsSinceEpoch();
    emit humiture(deviceAddr, time, temperature, humidity);
}

