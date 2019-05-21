#ifndef COMCONFIG_H
#define COMCONFIG_H

#include <QWidget>
#include <QListView>
#include <QTimer>
#include "IM_modbus.h"

const int DEVICE_COUNTS = 10;

namespace Ui {
class ComConfig;
}

class ComConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ComConfig(QWidget *parent = nullptr);
    ~ComConfig();

signals:
    void openCom(QVariant msg);
    void closeCom(QVariant msg);

    void readHumiture(QVariant msg);

private slots:
    void on_pushButton_com_clicked();

    void onOpenResult(bool result, QVariant msg);
    void onExitResult();

    void timesReadHumiture();

private:
    Ui::ComConfig *ui;

    QTimer          *homeTimer;
};

#endif // COMCONFIG_H
