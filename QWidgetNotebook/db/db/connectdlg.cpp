#include "connectdlg.h"
#include "ui_connectdlg.h"
#include <QDebug>

ConnDlg::ConnDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnDlg)
{
    ui->setupUi(this);
    connect(ui->comboDriver, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(driverChanged(const QString &)));
    QStringList drivers = QSqlDatabase::drivers();
    ui->comboDriver->addItems(drivers);
    ui->status_label->setText(tr("准备连接数据库"));
}

ConnDlg::~ConnDlg()
{
    delete ui;
}

void ConnDlg::on_okButton_clicked()
{
    if(ui->comboDriver->currentText().isEmpty()){
        ui->status_label->setText(tr("请选择一个数据库驱动!"));
        ui->comboDriver->setFocus();
    }else if(ui->comboDriver->currentText() == "QSQLLITE"){
        addSqliteConnection();
        // 创建数据库表，如果已存在则无需执行
        creatDB();
        accept();
    }else {
        QSqlError err = addConnection(
                    driverName(), databaseName(),
                    hostName(), userName(),
                    password(), port());
        if(err.type() != QSqlError::NoError){
            ui->status_label->setText(err.text());
        }else {
            ui->status_label->setText(tr("连接数据库成功!"));
        }
        accept();
    }
}

void ConnDlg::on_cancelButton_clicked()
{
    this->close();
}

void ConnDlg::driverChanged(const QString &text)
{
    qDebug() << "driverChanged func text: " << text;
    if(text == "QSQLITE"){
        ui->editDatabase->setEnabled(false);
        ui->editUsername->setEnabled(false);
        ui->editPassword->setEnabled(false);
        ui->editHostname->setEnabled(false);
        ui->portSpinBox->setEnabled(false);
    }else{
        ui->editDatabase->setEnabled(true);
        ui->editUsername->setEnabled(true);
        ui->editPassword->setEnabled(true);
        ui->editHostname->setEnabled(true);
        ui->portSpinBox->setEnabled(true);
    }
}

QString ConnDlg::driverName() const
{
    return ui->comboDriver->currentText();
}
QString ConnDlg::databaseName() const
{
    return ui->editDatabase->text();
}
QString ConnDlg::userName() const
{
    return ui->editUsername->text();
}
QString ConnDlg::password() const
{
    return ui->editPassword->text();
}
QString ConnDlg::hostName() const
{
    return ui->editHostname->text();
}
int ConnDlg::port() const
{
    return ui->portSpinBox->value();
}

QSqlError ConnDlg::addConnection(
        const QString &driver, const QString &dbname, const QString &host,
        const QString &user, const QString &password, int port)
{
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(dbname);
    db.setHostName(host);
    db.setPort(port);
    if(!db.open(user, password)){
        err = db.lastError();
    }
    return err;
}

void ConnDlg::addSqliteConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSLITE");
    db.setDatabaseName("databasefile");
    if(!db.open()){
        ui->status_label->setText(db.lastError().text());
        return;
    }
    ui->status_label->setText(tr("创建sqlite数据库成功!"));
}

void ConnDlg::creatDB()
{
    QSqlQuery query;
    query.exec("create table factory \
                (id int primary key,\
                manufactory varchar(40),\
                address varchar(40))");
    query.exec(tr("insert into factory values(\
                   1, '一汽大众', '长春')"));
    query.exec(tr("insert into factory values(\
                   2, '二气神龙', '武汉')"));
    query.exec(tr("insert into factory values(\
                   3, '上海大众', '上海')"));
    query.exec("create table cars \
                (carid int primary key,\
                name varchar(50),\
                factoryid int,\
                year int,\
                foreign key(factoryid) references factory");
    query.exec(tr("insert into cars values(\
                   1, '奥迪A6', 1, 2005)"));
    query.exec(tr("insert into cars values(\
                   2, '捷达', 1, 1993)"));
    query.exec(tr("insert into cars values(\
                   3, '宝来', 1, 2000)"));
    query.exec(tr("insert into cars values(\
                   4, '毕加索', 2, 1999)"));
    query.exec(tr("insert into cars values(\
                   5, '富康', 2, 2004)"));
    query.exec(tr("insert into cars values(\
                   6, '标致307', 2, 2001)"));
    query.exec(tr("insert into cars values(\
                   7, '桑塔纳', 3, 1995)"));
    query.exec(tr("insert into cars values(\
                   8, '帕萨特', 3, 2000)"));

}
