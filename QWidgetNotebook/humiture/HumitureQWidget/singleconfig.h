#ifndef SINGLECONFIG_H
#define SINGLECONFIG_H

#include <QWidget>

namespace Ui {
class SingleConfig;
}

class SingleConfig : public QWidget
{
    Q_OBJECT

public:
    explicit SingleConfig(QWidget *parent = nullptr);
    ~SingleConfig();

signals:
    void currentDeviceAddr(int deviceAddr);
private slots:
    void on_pushButton_set_clicked();

private:
    Ui::SingleConfig *ui;
};

#endif // SINGLECONFIG_H
