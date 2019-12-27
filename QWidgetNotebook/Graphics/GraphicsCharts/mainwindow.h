#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

 using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QChart *initBar(int flag);
    void laoutCharts();

    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    QChartView  *view1;
    QChartView  *view2;
    QGraphicsView *mainView;
    QGraphicsScene *mainScene;
};

#endif // MAINWINDOW_H
