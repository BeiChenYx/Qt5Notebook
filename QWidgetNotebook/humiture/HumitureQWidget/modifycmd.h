#ifndef MODIFYCMD_H
#define MODIFYCMD_H

#include <QWidget>

namespace Ui {
class ModifyCmd;
}

class ModifyCmd : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyCmd(QWidget *parent = nullptr);
    ~ModifyCmd();

private:
    Ui::ModifyCmd *ui;
};

#endif // MODIFYCMD_H
