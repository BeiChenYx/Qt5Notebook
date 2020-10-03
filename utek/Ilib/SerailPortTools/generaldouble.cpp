#include "generaldouble.h"
#include "ui_generaldouble.h"
#include <QFileInfo>
#include <QSettings>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>

GeneralDouble::GeneralDouble(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::generalDouble)
{
    ui->setupUi(this);
    for (int i=1; i<=LINE_EDIT_COUNT; ++i) {
        auto checkBox = new QCheckBox("HEX", this);
        auto lineEidt = new QLineEdit(this);
        auto btn= new QPushButton(QString::number(i), this);
        checkBox->setObjectName(QString("checkBox_%1x").arg(i));
        lineEidt->setObjectName(QString("lineEdit_%1").arg(i));
        btn->setObjectName(QString("btn_%1").arg(i));
        QHBoxLayout *hLayOut = new QHBoxLayout();
        hLayOut->addWidget(checkBox);
        hLayOut->addWidget(lineEidt);
        hLayOut->addWidget(btn);
        hLayOut->setSpacing(6);
        ui->gridLayout->addItem(hLayOut, (i-1)/2, (i-1)%2);
        qDebug() << (i-1)/2 << " : " << (i-1)%2;
        connect(btn, &QPushButton::clicked, this, [this, checkBox, lineEidt](){
            emit sendData(lineEidt->text().trimmed(), checkBox->isChecked());
        });
        connect(checkBox, &QCheckBox::clicked, this, [checkBox, lineEidt](){
            if(checkBox->isChecked()){
                QString buf = lineEidt->text().trimmed();
                if(!buf.isEmpty()){
                    QByteArray byteBuf(buf.toUtf8());
                    lineEidt->setText(byteBuf.toHex(' ').toLower());
                }
            }else{
                QString buf = lineEidt->text().trimmed().replace(" ", "");
                lineEidt->setText(QByteArray::fromHex(buf.toLocal8Bit()));
            }
        });
    }
}

GeneralDouble::~GeneralDouble()
{
    delete ui;
}

bool GeneralDouble::writeInitFile(){
    /*
     * 初始化配置文件
     * 如果配置文件不存在则使用初始化值来初始化界面和文件
    */
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    for(int i = 1; i <= LINE_EDIT_COUNT; i++){
        auto lineEidt = this->findChild<QLineEdit*>(QString("lineEdit_%1").arg(i));
        auto checkBox = this->findChild<QCheckBox*>(QString("checkBox_%1x").arg(i));
        settings.setValue("GeneralDouble/SendBuf" + QString("%1").arg(i), lineEidt->text());
        settings.setValue("GeneralDouble/BufCheck" + QString("%1").arg(i), checkBox->isChecked());
    }
    return true;
}
bool GeneralDouble::readInitFile(){
    QString iniFilePath = "./ComDebug.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("GBK");
    for (int i=1; i<=LINE_EDIT_COUNT; ++i) {
        auto checkBox = this->findChild<QCheckBox*>(QString("checkBox_%1x").arg(i));
        auto lineEidt = this->findChild<QLineEdit*>(QString("lineEdit_%1").arg(i));
        if(checkBox == nullptr || lineEidt == nullptr){continue;}
        checkBox->setChecked(settings.value(QString("GeneralDouble/BufCheck%1").arg(i)).toBool());
        lineEidt->setText(settings.value(QString("GeneralDouble/SendBuf%1").arg(i)).toString());
    }
    return true;
}


void GeneralDouble::statusEnabled(bool status)
{
    for (int i=1; i<=LINE_EDIT_COUNT; ++i) {
        auto btn = this->findChild<QPushButton*>(QString("btn_%1").arg(i));
        btn->setEnabled(status);
    }
}
