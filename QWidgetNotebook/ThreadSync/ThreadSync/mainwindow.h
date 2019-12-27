#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "threadsync.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openBtn_clicked();

    void on_closeBtn_clicked();

    void on_gernatorbtn_clicked();

    void onHandleResult(int result);

private:
    Ui::MainWindow  *ui;
    ThreadSync      *m_thread;

    int data = 0;
};

#endif // MAINWINDOW_H
