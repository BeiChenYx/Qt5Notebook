#include "handlerecord.h"
#include <QDebug>

HandleRecord::HandleRecord(QObject *parent) : QObject(parent)
{

}

bool HandleRecord::createDatabaseConnection()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QCoreApplication::applicationDirPath();
    db.setDatabaseName(path + "/SqlModel.db");
    if(!db.open())
    {
        qDebug() << "Can not open database;";
        return false;
    }
    return true;
}

QList<HumitureRecord> HandleRecord::queryDatabase(int deviceAddr, QString date)
{
    QList<HumitureRecord> result;
    QSqlQuery query(db);
    QString sql = QString("select * from humiture where deviceId=%1 and date='%2'")
            .arg(deviceAddr).arg(date);
    query.exec(sql);
    while (query.next()) {
        HumitureRecord tmp;
        tmp.deviceId = query.value(0).toInt();
        tmp.temperature = query.value(1).toDouble();
        tmp.humidity = query.value(2).toDouble();
        tmp.date = static_cast<qint64>(query.value(3).toULongLong());
        result.append(tmp);
    }
    return result;
}

bool HandleRecord::insertRecord(HumitureRecord &record)
{
    QSqlQuery query(db);
    int deviceId = record.deviceId;
    double temperature = record.temperature;
    double humidity = record.humidity;
    qint64 date = record.date;
    QString sql = QString("insert into humiture values (%1, %2, %3, '%4')")
            .arg(deviceId).arg(temperature).arg(humidity).arg(date);
    query.exec(sql);
    if(query.lastError().isValid())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

void HandleRecord::closeConnection()
{
    this->db.close();
}
