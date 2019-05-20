#ifndef MODBUSTEST_H
#define MODBUSTEST_H

#include <QWidget>
#include <QTimer>
#include "IM_modbus.h"

namespace Ui {
class ModbusTest;
}

class ModbusTest : public QWidget
{
    Q_OBJECT

public:
    explicit ModbusTest(QWidget *parent = nullptr);
    ~ModbusTest();

signals:
    void readHumiture(QVariant msg);

private slots:
    void on_pushButton_Read_clicked();

    void onModbusTestResult(QVariant msg);

    void timesReadHumiture();

    void on_pushButton_times_clicked();

private:
    Ui::ModbusTest *ui;
    QTimer          *humitureTimer;
};

#endif // MODBUSTEST_H
