#ifndef DATATEST_H
#define DATATEST_H

#include <QWidget>
#include <QThread>
#include <QLineEdit>
#include <QTimer>
#include <QQueue>
#include "handlehid.h"

namespace Ui {
class DataTest;
}


class DataTest : public QWidget
{
    Q_OBJECT

public:
    explicit DataTest(QWidget *parent = nullptr);
    ~DataTest();

    static void canDataFunc(CanRcvData *pData, void *pUser);
    void onWriteData(int &len, QString &id, QString &frameType,
                   QString &frameFormat, QString &data, QString &timestamp);

signals:
    void txDataToCan(QString id, QString frameType, QString frameFormat, QString data);
    void txTimes(int times, QString id, QString frameType, QString frameFormat, QString data);
    void txCloseTimes();
public slots:
    void onRcvData(int len, QString id, QString frameType,
                   QString frameFormat, QString timestamp, QString data);
private slots:
    void on_pushButton_send_clicked();
    void on_checkBox_times_clicked();
    bool handleUiData(QString &id, QString &frameType, QString &frameFormat, QString &data);

    void on_pushButton_clear_clicked();

    void on_pushButton_output_clicked();

private:
    Ui::DataTest *ui;
};

#endif // DATATEST_H
