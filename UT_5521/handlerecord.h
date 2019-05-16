#ifndef HANDLERECORD_H
#define HANDLERECORD_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QCoreApplication>


struct HumitureRecord{
    int deviceId;
    double temperature;
    double humidity;
    qint64 date;
};
Q_DECLARE_METATYPE(HumitureRecord)

class HandleRecord : public QObject
{
    Q_OBJECT
public:
    explicit HandleRecord(QObject *parent = nullptr);

    bool createDatabaseConnection();

    // deviceAddr 对应数据库 deviceId, date为日期，sqlite没有date类型
    QList<HumitureRecord> queryDatabase(int deviceAddr, QString date);

    bool insertRecord(HumitureRecord &record);

    void closeConnection();

signals:

public slots:

private:
    QSqlDatabase    db;
};

#endif // HANDLERECORD_H
