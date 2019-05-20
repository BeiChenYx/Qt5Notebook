#ifndef MODIFYCMD_H
#define MODIFYCMD_H

#include <QWidget>
#include "IM_modbus.h"

namespace Ui {
class ModifyCmd;
}

class ModifyCmd : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyCmd(QWidget *parent = nullptr);
    ~ModifyCmd();

signals:
    void modifyCmd(QVariant msg);

private slots:
    void on_pushButton_write_clicked();

    void onModifyCmdResult(QVariant msg);

private:
    Ui::ModifyCmd *ui;
};

#endif // MODIFYCMD_H
