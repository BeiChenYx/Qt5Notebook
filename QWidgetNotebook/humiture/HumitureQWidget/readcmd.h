#ifndef READCMD_H
#define READCMD_H

#include <QWidget>

namespace Ui {
class ReadCmd;
}

class ReadCmd : public QWidget
{
    Q_OBJECT

public:
    explicit ReadCmd(QWidget *parent = nullptr);
    ~ReadCmd();

private:
    Ui::ReadCmd *ui;
};

#endif // READCMD_H
