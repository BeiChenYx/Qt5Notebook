#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void getCurrentData();
    void toggleSelection();
    void updateSelection(const QItemSelection &selected, const QItemSelection &deselected);
    void changeCurrent(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::MainWindow *ui;
    QTableView *tableView;
};

#endif // MAINWINDOW_H
