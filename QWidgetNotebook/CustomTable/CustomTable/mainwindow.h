#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPixmap>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class CustomItemDelegate;
class CustomStandardItemModel;

class CustomTableView : public QTableView
{
public:
    CustomTableView(QWidget * parent = nullptr);
    virtual ~CustomTableView(){}
protected:
    void mouseMoveEvent(QMouseEvent *event);
private:
    CustomItemDelegate *delegate;
    CustomStandardItemModel *model;
};

class CustomItemDelegate: public QItemDelegate
{
public:
    CustomItemDelegate(QObject *parent=nullptr);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index);
private:
    QPixmap favouritePixmap;
    QPixmap notFavouritePixmap;
};

class CustomStandardItemModel: public QStandardItemModel
{
public:
    CustomStandardItemModel(QObject *parent=nullptr)
        : QStandardItemModel(parent){}
    virtual ~CustomStandardItemModel(){}

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role=Qt::DisplayRole) const;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
