#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QTableView>
#include <QListWidget>
#include <QLabel>
#include <QFile>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QDomNode>
#include <QDomDocument>
#include <QSqlRecord>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//    MainWindow(QWidget *parent = nullptr);
    MainWindow(const QString &factoryTable,
               const QString &carTable,
               QFile *carDetails, QWidget *parent=nullptr);
    ~MainWindow();

private slots:
    void addCar();
    void changeFactory(QModelIndex index);
    void delCar();
    void showCarDetails(QModelIndex index);
    void showFactoryProfile(QModelIndex index);

private:
    void createMenuBar();
    QGroupBox *createCarGroupBox();
    QGroupBox *createFactoryGroupBox();
    QGroupBox *createDetailGroupBox();

    void decreaseCarCount(QModelIndex index);
    void getAttribList(QDomNode car);
    QModelIndex indexOfFactory(const QString &factory);
    void readCarData();
    void removeCarFromDatabase(QModelIndex index);
    void removeCarFromFile(int id);

private:
    QTableView *carView;
    QTableView *factoryView;
    QListWidget *attribList;
    QLabel *profileLable;
    QLabel *titleLabel;

    QDomDocument carData;
    QFile *file;
    QSqlRelationalTableModel *carModel;
    QSqlTableModel *factoryModel;
};

#endif // MAINWINDOW_H
