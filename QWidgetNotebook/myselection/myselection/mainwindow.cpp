#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(7, 4, this);
    for (int row=0; row<7; ++row) {
        for (int column=0; column<4; ++column) {
            QStandardItem *item = new QStandardItem(QString("%1").arg(row * 4 + column));
            model->setItem(row, column, item);
        }
    }
    tableView = new QTableView(this);
    tableView->setModel(model);
    setCentralWidget(tableView);
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    QModelIndex topLeft = model->index(1, 1, QModelIndex());
    QModelIndex bottomRight = model->index(5, 2, QModelIndex());
    QItemSelection selection(topLeft, bottomRight);
    selectionModel->select(selection, QItemSelectionModel::Select);

    ui->mainToolBar->addAction(tr("当前项目"), this, &MainWindow::getCurrentData);
    ui->mainToolBar->addAction(tr("切换选择"), this, &MainWindow::toggleSelection);

    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::updateSelection);
    connect(selectionModel, &QItemSelectionModel::currentChanged, this, &MainWindow::changeCurrent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getCurrentData()
{
    qDebug() << tr("当前项目的内容") << tableView->selectionModel()->currentIndex().data().toString();
}
void MainWindow::toggleSelection()
{
    QModelIndex topLeft = tableView->model()->index(0, 0,QModelIndex());
    QModelIndex bottomRight = tableView->model()->index(
                tableView->model()->rowCount(QModelIndex())-1,
                tableView->model()->columnCount(QModelIndex())-1,
                QModelIndex());
    QItemSelection curSelection(topLeft, bottomRight);
    tableView->selectionModel()->select(curSelection, QItemSelectionModel::Toggle);
}

void MainWindow::updateSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndex index;
    QModelIndexList list = selected.indexes();
    foreach(index, list){
        QString text = QString("%1.%2").arg(index.row()).arg(index.column());
        tableView->model()->setData(index, text);
    }
    list = deselected.indexes();
    foreach(index, list){
        tableView->model()->setData(index, "");
    }
}
void MainWindow::changeCurrent(const QModelIndex &current, const QModelIndex &previous)
{
    qDebug() << tr("move (%1.%2) to (%3.%4)")
                .arg(previous.row()).arg(previous.column())
                .arg(current.row()).arg(current.column());
}
