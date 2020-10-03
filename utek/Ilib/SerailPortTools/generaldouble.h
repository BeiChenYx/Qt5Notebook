#ifndef GENERALDOUBLE_H
#define GENERALDOUBLE_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class generalDouble;
}

class GeneralDouble : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralDouble(QWidget *parent = nullptr);
    ~GeneralDouble();

    bool writeInitFile();
    bool readInitFile();
    void statusEnabled(bool status);
private:
    void toSend(QString data);

signals:
    void sendData(QString msg, bool isHex);

public:
    Ui::generalDouble *ui;

    const int LINE_EDIT_COUNT = 10;
};

#endif // GENERALDOUBLE_H
