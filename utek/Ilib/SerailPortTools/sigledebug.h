#ifndef SIGLEDEBUG_H
#define SIGLEDEBUG_H

#include <QWidget>
#include <QDebug>
#include <QRegExp>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include "handleserialport.h"


namespace Ui {
class sigleDebug;
}

class SigleDebug : public QWidget
{
    Q_OBJECT

public:
    explicit SigleDebug(QWidget *parent = nullptr);
    ~SigleDebug();

    bool WriteInitFile();
    bool ReadInitFile();

private slots:
    void on_pushButton_Connect_clicked();
    void on_pushButton_Close_clicked();
    void on_pushButton_Send_clicked();
    void on_pushButton_ClearRead_clicked();
    void on_checkBox_times_clicked();
    void on_pushButton_Scan_clicked();
    void on_textEdit_Send_textChanged();
    void on_pushButton_ClearTimes_clicked();
    void on_checkBox_16SetSend_clicked();
    void on_checkBox_16Read_clicked();
    void on_checkBox_DTR_clicked();
    void on_checkBox_RTS_clicked();

    void onReadData(QString msg, qint64 size);
    void writeLogFile(QString msg);
    void statusEnabled(bool status);

private:
    Ui::sigleDebug *ui;

    HandleSerialPort *m_pSerial;
    QTimer *m_timesSend;
};

#endif // SIGLEDEBUG_H
