#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

static bool createConnection()
{
    // 连接 sqlite 数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/yx/WorkSpace/Qt5Notebook/Core/SqlModel/SqlModel/SqlModel.db");
//    db.setDatabaseName(":memory:");
    if(!db.open()){
        QMessageBox::critical(nullptr, "Cannot open database",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query(db);
//    query.exec("create table Name (id int primary key, name varchar(20))");
//    qDebug() << query.lastError();
//    query.exec("insert into Name values(0, 'LiMing')");
//    qDebug() << query.lastError();
//    query.exec("insert into Name values(1, 'Rjjong')");
//    query.exec("insert into Name values(2, 'NoMing')");

    query.exec("select id, name from Name");
    while (query.next()) {
        qDebug() << query.value(0).toInt() << ": " << query.value(1).toString();
    }
    qDebug() << query.lastError();

    return true;
}

#endif // CONNECTION_H
