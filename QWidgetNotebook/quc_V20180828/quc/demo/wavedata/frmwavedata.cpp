#pragma execution_character_set("utf-8")

#include "frmwavedata.h"
#include "ui_frmwavedata.h"
#include "qfile.h"

frmWaveData::frmWaveData(QWidget *parent) : QWidget(parent), ui(new Ui::frmWaveData)
{
    ui->setupUi(this);
    this->initForm();
}

frmWaveData::~frmWaveData()
{
    delete ui;
}

void frmWaveData::initForm()
{
    //从资源文件中读取数据
    QString file = ":/image/data1.txt";
    QVector<float> data;
    QFile f(file);
    if (f.open(QFile::ReadOnly)) {
        QString str = f.readAll();
        QStringList list = str.split(" ");

        foreach (QString s, list) {
            data.append(s.toDouble());
        }

        ui->waveData->setData(data);
    }
}
