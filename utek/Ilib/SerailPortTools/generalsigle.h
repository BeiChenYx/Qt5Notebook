#ifndef GENERALSIGLE_H
#define GENERALSIGLE_H

#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>

namespace Ui {
class generalSigle;
}

class GeneralSigle : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSigle(QWidget *parent = nullptr);
    ~GeneralSigle();

    bool writeInitFile();
    bool readInitFile();
    void statusEnabled(bool status);

signals:
    void sendData(QString msg, bool isHex);

private slots:
    void on_pushButton_send_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_scan_clicked();
    void on_pushButton_send_file_clicked();
    void on_checkBox_times_clicked();

    void on_sendFile();
    void on_checkBox_hex_clicked();
    void on_checkBox_enter_clicked();

public:
    Ui::generalSigle *ui;

    QTimer *m_timesSend;
    QTimer *m_fileSend;
    QByteArray m_fileBuf;
    int m_index = 0;
};

#endif // GENERALSIGLE_H
