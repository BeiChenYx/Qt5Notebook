#ifndef SETPARAMETER_H
#define SETPARAMETER_H

#include <QWidget>
#include "handleserialprotocol.h"
#include "handlehid.h"

namespace Ui {
class SetParameter;
}

class SetParameter : public QWidget
{
    Q_OBJECT

public:
    explicit SetParameter(QWidget *parent = nullptr);
    ~SetParameter();
    void setBaudrates(QStringList &canBaudrates, QStringList &serialPortBaudrates);
    void setHandle(HandleHid *pHanle){
        m_pHidWorkThread = pHanle;
    }
    void setHandleSerial(HandleSerialProtocol *pHanle){
        m_pHandleSerial = pHanle;
    }
    void onCanBaudrate(QString &baudrate);
public slots:
    void onIsRMcuPar(TransiteParameterUi &canPar, SerialPortParameterUi &serialPar);
    void onIsRSerialPar(QString &baudrate, QString &id, QString &frameFormat);
    void on_pushButton_read_can_clicked();

    void onSerialStatus(bool status);
    void onHidStatus(bool status);

private slots:
    void on_pushButton_set_clicked();
    void on_pushButton_model_clicked();
    void on_pushButton_reset_clicked();
    void on_pushButton_reset_error_clicked();
    void on_pushButton_com_par_clicked();
    void on_pushButton_com_read_par_clicked();
    void on_pushButton_ts_clicked();
    void on_pushButton_set_can_clicked();

    void on_comboBox_ts_model_currentIndexChanged(int index);

private:
    Ui::SetParameter *ui;
    HandleHid *m_pHidWorkThread;
    HandleSerialProtocol *m_pHandleSerial;
    bool m_isSerial = false;
    bool m_isHid = false;
};

#endif // SETPARAMETER_H
