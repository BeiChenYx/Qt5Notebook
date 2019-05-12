#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>

namespace Ui {
class MainWindow;
}

/*
 * 数据库的操作和 model
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool createDatabaseConnection();

    bool createTable(QString tableName="student");

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_Insert_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase    db;
};

#endif // MAINWINDOW_H
