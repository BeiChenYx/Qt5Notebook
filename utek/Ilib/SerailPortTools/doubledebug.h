#ifndef DOUBLEDEBUG_H
#define DOUBLEDEBUG_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include "handleserialport.h"

namespace Ui {
class DoubleDebug;
}

class DoubleDebug : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleDebug(QWidget *parent = nullptr);
    ~DoubleDebug();

    bool WriteInitFile();
    bool ReadInitFile();

private slots:
    void onReadData1(QString msg, qint64 size);
    void onReadData2(QString msg, qint64 size);
    void writeLogFile(QString msg);
    void statusEnabled1(bool status);
    void statusEnabled2(bool status);

    void on_pushButton_Connect1_clicked();
    void on_pushButton_Close1_clicked();
    void on_pushButton_Connect2_clicked();
    void on_pushButton_Close2_clicked();
    void on_pushButton_Send1_clicked();
    void on_pushButton_Send2_clicked();
    void on_pushButton_ClearDisplay1_clicked();
    void on_pushButton_ClearDisplay2_clicked();

    void on_checkBox_DTR_clicked();
    void on_checkBox_RTS_clicked();
    void on_checkBox_DTR2_clicked();
    void on_checkBox_RTS2_clicked();
    void on_checkBox_Times_clicked();
    void on_pushButton_Scan_clicked();
    void on_checkBox_16In1_clicked();
    void on_checkBox_16In2_clicked();
    void on_textEdit_Input1_textChanged();
    void on_textEdit_Input2_textChanged();
    void on_checkBox_16D1_clicked();
    void on_checkBox_16D2_clicked();

private:
    Ui::DoubleDebug *ui;

    HandleSerialPort *m_pSerial1;
    HandleSerialPort *m_pSerial2;
    QTimer *m_timesSend;
};

#endif // DOUBLEDEBUG_H
