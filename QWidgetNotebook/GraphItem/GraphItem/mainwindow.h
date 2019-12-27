#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initScene();


private slots:
    void on_actionNew_triggered();

    void on_actionClear_triggered();

    void on_actionExit_triggered();

    void on_actionEllipse_triggered();

    void on_actionPolygon_triggered();

    void on_actionText_triggered();

    void on_actionRect_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
