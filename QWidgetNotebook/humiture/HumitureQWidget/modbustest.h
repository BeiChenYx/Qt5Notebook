#ifndef MODBUSTEST_H
#define MODBUSTEST_H

#include <QWidget>

namespace Ui {
class ModbusTest;
}

class ModbusTest : public QWidget
{
    Q_OBJECT

public:
    explicit ModbusTest(QWidget *parent = nullptr);
    ~ModbusTest();

private:
    Ui::ModbusTest *ui;
};

#endif // MODBUSTEST_H
