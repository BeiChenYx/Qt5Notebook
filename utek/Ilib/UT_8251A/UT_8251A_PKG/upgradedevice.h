#ifndef UPGRADEDEVICE_H
#define UPGRADEDEVICE_H

#include <QWidget>
#include "handlehid.h"
#include "handleserialprotocol.h"

namespace Ui {
class UpgradeDevice;
}

class UpgradeDevice : public QWidget
{
    Q_OBJECT

public:
    explicit UpgradeDevice(QWidget *parent = nullptr);
    ~UpgradeDevice();
    void setHandle(HandleHid *pHanle){
        m_pHidWorkThread = pHanle;
    }
    void setHandleSerial(HandleSerialProtocol *pHanle){
        m_pHandleSerial = pHanle;
    }
public slots:
    void onUpdateStatus(QString msg);
    void onUpdateResult(bool status);
    void onProcessRange(int min, int max);
    void onProcessBarV(int v);

    void onSerialStatus(bool status){
        m_isSerial = status;
    }
    void onHidStatus(bool status){
        m_isHid = status;
    }

private slots:
    void on_pushButton_update_clicked();
    void on_pushButton_file_clicked();

private:
    Ui::UpgradeDevice *ui;
    HandleHid *m_pHidWorkThread;
    HandleSerialProtocol *m_pHandleSerial;
    bool m_isSerial = false;
    bool m_isHid = false;
};

#endif // UPGRADEDEVICE_H
