#include <QDebug>
#include <QDate>
#include <QRegExp>
#include "humiturerecordpage.h"
#include "ui_humiturerecordpage.h"

HumitureRecordPage::HumitureRecordPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HumitureRecordPage)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    this->pHumitureCharts = new RecordHumitureChart();
    ui->vlayout_chart->addWidget(this->pHumitureCharts);

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(onQueryBtn()));
}

HumitureRecordPage::~HumitureRecordPage()
{
    delete ui;
}

void HumitureRecordPage::onQueryBtn()
{
    foreach (QString str, this->deviceNoHandle.keys()) {
        QLineSeries *line = this->deviceNoHandle.value(str);
        line->clear();
        delete line;
    }
    this->deviceNoHandle.clear();
    QString date = ui->dateEdit->text();
    //    date.replace(QRegExp("/"), "-");
    qDebug() << "date: " << date;
    QList<int> deviceList;
    for (int i = 1; i <= 10; i++) {
        QCheckBox *check = this->findChild<QCheckBox*>(QString("checkBox_%1").arg(i));
        if(check->isChecked()){
            QLineSeries *plineT = this->pHumitureCharts->createLineSerie(QString("设备%1温度").arg(i), true);
            QLineSeries *plineH = this->pHumitureCharts->createLineSerie(QString("设备%1湿度").arg(i), false);
            this->deviceNoHandle.insert(QString(QString("%1T").arg(i)), plineT);
            this->deviceNoHandle.insert(QString(QString("%1H").arg(i)), plineH);
            deviceList.append(i);
        }
    }
    foreach (int i, deviceList) {
        Task task;
        task.m_page = 0;
        task.m_isRecord = true;
        task.m_deviceAddr = i;
        task.task_type = Task::TaskType::Query_Record;
        task.m_queryDate = date;
        QVariant msg;
        msg.setValue(task);
        emit readRecord(msg);
    }


}
void HumitureRecordPage::onHumitureRecord(QVariant msg)
{
    HumitureRecord value = msg.value<HumitureRecord>();
    int deviceAddr = value.deviceId;
    double temperature = value.temperature;
    double humidity = value.humidity;
    qint64 time = value.time;
    QLineSeries *plineT = this->deviceNoHandle.value(QString("%1T").arg(deviceAddr), nullptr);
    QLineSeries *plineH = this->deviceNoHandle.value(QString("%1H").arg(deviceAddr), nullptr);
    if(plineT){
        plineT->append(time, temperature);
    }
    if(plineH){
        plineH->append(time, humidity);
    }
}

RecordHumitureChart::RecordHumitureChart(HumitureCharts *parent) : HumitureCharts(parent)
{

}
