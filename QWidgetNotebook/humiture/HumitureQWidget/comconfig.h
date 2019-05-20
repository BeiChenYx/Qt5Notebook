#ifndef COMCONFIG_H
#define COMCONFIG_H

#include <QWidget>
#include <QListView>
#include "IM_modbus.h"

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

private slots:
    void on_pushButton_com_clicked();

    void onOpenResult(bool result, QVariant msg);
    void onExitResult();

private:
    Ui::ComConfig *ui;
};

#endif // COMCONFIG_H
