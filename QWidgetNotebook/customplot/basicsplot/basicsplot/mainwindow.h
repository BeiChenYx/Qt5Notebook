#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initChart1();
    void initChart2();
    void initChart3();
    void initChart4();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
