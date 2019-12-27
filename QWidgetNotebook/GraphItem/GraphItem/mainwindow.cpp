#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->scene = new QGraphicsScene();
    this->scene->setSceneRect(-200, -200, 400, 400);

    this->initScene();

    QGraphicsView *view = new QGraphicsView();
    view->setScene(scene);
    view->setMinimumSize(400, 400);
    view->show();

    this->setCentralWidget(view);
    this->resize(550, 450);
    this->setWindowTitle(tr("Graphics Items"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initScene()
{
    int i;
    for(i = 0; i < 3; i++){
        this->on_actionEllipse_triggered();
    }
    for(i = 0; i < 3; i++){
        this->on_actionPolygon_triggered();
    }
    for(i = 0; i < 3; i++){
        this->on_actionText_triggered();
    }
    for(i = 0; i < 3; i++){
        this->on_actionRect_triggered();
    }
}

void MainWindow::on_actionNew_triggered()
{
    this->on_actionClear_triggered();
    this->initScene();
    MainWindow *newWin = new MainWindow();
    newWin->show();
}

void MainWindow::on_actionClear_triggered()
{
    QList<QGraphicsItem *> listItem = this->scene->items();
    while (!listItem.empty()) {
        this->scene->removeItem(listItem.at(0));
        listItem.removeAt(0);
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionEllipse_triggered()
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(QRectF(0, 0, 80, 60));
    item->setPen(Qt::NoPen);
    item->setBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
    item->setFlag(QGraphicsItem::ItemIsMovable);

    this->scene->addItem(item);
    item->setPos((qrand() % int(scene->sceneRect().width())) - 200,
                 (qrand() % int(scene->sceneRect().height())) - 200);
}

void MainWindow::on_actionPolygon_triggered()
{
    QVector<QPoint> v;
    v << QPoint(30, -15) << QPoint(0, -30)
      << QPoint(-30, -15) << QPoint(-30, 15)
      << QPoint(0, 30) << QPoint(30, 15);

    QGraphicsPolygonItem *item = new QGraphicsPolygonItem(QPolygonF(v));
    item->setBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    this->scene->addItem(item);
    item->setPos((qrand() % int(scene->sceneRect().width())) - 200,
                 (qrand() % int(scene->sceneRect().height())) - 200);
}

void MainWindow::on_actionText_triggered()
{
    QFont font("Times", 16);
    QGraphicsTextItem *item = new QGraphicsTextItem("Hello Qt");
    item->setFont(font);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setDefaultTextColor(QColor(qrand() % 256, qrand() % 256, qrand() % 256));

    this->scene->addItem(item);
    item->setPos((qrand() % int(scene->sceneRect().width())) - 200,
                 (qrand() % int(scene->sceneRect().height())) - 200);
}

void MainWindow::on_actionRect_triggered()
{
    QGraphicsRectItem *item = new QGraphicsRectItem(QRectF(0, 0, 60, 60));
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
    item->setPen(pen);

    item->setBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    this->scene->addItem(item);
    item->setPos((qrand() % int(scene->sceneRect().width())) - 200,
                 (qrand() % int(scene->sceneRect().height())) - 200);
}
