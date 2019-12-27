#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

CustomTableView::CustomTableView(QWidget * parent)
    :QTableView (parent)
{
    delegate = new CustomItemDelegate;
    model = new CustomStandardItemModel;
    model->setRowCount(6);
    model->setColumnCount(8);

    this->setModel(model);
    this->setItemDelegate(delegate);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setMouseTracking(true);
}

void CustomTableView::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

CustomItemDelegate::CustomItemDelegate(QObject *parent)
{
    Q_UNUSED(parent);
    favouritePixmap = QPixmap(":/img/max.png");
    notFavouritePixmap = QPixmap(":/img/max1.png");
}

void CustomItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() != 0){
        QItemDelegate::paint(painter, option, index);
        return;
    }
    const QAbstractItemModel *model = index.model();
    QVariant var = model->data(index, Qt::CheckStateRole);
    if(var.isNull()) var = false;
    const QPixmap & star = var.toBool() ? favouritePixmap : notFavouritePixmap;

    int width = star.width();
    int height = star.height();
    QRect rect = option.rect;
    int x = rect.x() + rect.width()/2-width/2;
    int y = rect.y() + rect.height()/2-height/2;
    painter->drawPixmap(x, y, star);
}

bool CustomItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                 const QStyleOptionViewItem &option,
                 const QModelIndex &index)
{
    Q_UNUSED(option);
    if(event->type() == QEvent::MouseButtonPress && index.column() == 0){
        QVariant var = model->data(index, Qt::CheckStateRole);
        bool isFavourite = var.isValid();
        model->setData(index, isFavourite, Qt::CheckStateRole);
    }
    return false;
}

QVariant CustomStandardItemModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    if(role==Qt::DisplayRole && column != 0) return column + 1;
    if(role == Qt::ToolTipRole && column == 0) return tr("love");
    return  QStandardItemModel::data(index, role);
}
QVariant CustomStandardItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section == 0 && orientation == Qt::Horizontal){
        if(role==Qt::DecorationRole){
            return QIcon(":/img/max.png");
        }
    }
}
