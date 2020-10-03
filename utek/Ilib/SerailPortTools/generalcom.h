#ifndef GENERALCOM_H
#define GENERALCOM_H

#include <QWidget>
#include <QDebug>
#include <QRegExp>
#include <QMessageBox>
#include <QFileDialog>
#include "generalsigle.h"
#include "generaldouble.h"
#include "handleserialport.h"

namespace Ui {
class GeneralCom;
}

class GeneralCom : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralCom(QWidget *parent = nullptr);
    ~GeneralCom();

    void writeLogFile(QString msg);
    bool WriteInitFile();
    bool ReadInitFile();

private slots:
    void on_pushButton_Open_clicked();
    void on_pushButton_Close_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();
    void on_pushButton_ClearCount_clicked();
    void onReadyString(QString msg, qint64 size);
    void on_checkBox_16Set_clicked();
    void on_checkBox_dataStopFlame_clicked();
    void on_checkBox_DTR_clicked();
    void on_checkBox_RTS_clicked();

    void statusEnabled(bool status);
private:
    Ui::GeneralCom *ui;

    HandleSerialPort *m_pSerial;
    GeneralSigle    *m_pSigle;
    GeneralDouble   *m_pDouble;
};

#endif // GENERALCOM_H
