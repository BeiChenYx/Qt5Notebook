#ifndef READCMD_H
#define READCMD_H

#include <QWidget>
#include "IM_modbus.h"

namespace Ui {
class ReadCmd;
}

class ReadCmd : public QWidget
{
    Q_OBJECT

public:
    explicit ReadCmd(QWidget *parent = nullptr);
    ~ReadCmd();

signals:
    void readCmd(QVariant msg);

private slots:
    void on_pushButton_read_clicked();
    void onReadCmdResult(QVariant msg);

private:
    Ui::ReadCmd *ui;
};

#endif // READCMD_H
