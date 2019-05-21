#ifndef HUMITURERECORD_H
#define HUMITURERECORD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "humiturecharts.h"
#include "IM_modbus.h"

namespace Ui {
class HumitureRecordPage;
}

class RecordHumitureChart : public HumitureCharts
{
public:
    RecordHumitureChart(HumitureCharts *parent = nullptr);

};

class HumitureRecordPage : public QWidget
{
    Q_OBJECT

public:
    explicit HumitureRecordPage(QWidget *parent = nullptr);
    ~HumitureRecordPage();

signals:
    void readRecord(QVariant msg);

public slots:
    void onHumitureRecord(QVariant msg);
    void onQueryBtn();

private:
    Ui::HumitureRecordPage *ui;
    RecordHumitureChart     *pHumitureCharts;
    QMap<QString, QLineSeries*>  deviceNoHandle;
};

#endif // HUMITURERECORD_H
