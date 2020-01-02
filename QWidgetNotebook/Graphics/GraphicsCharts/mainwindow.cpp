#include <QGraphicsLinearLayout>
#include <QGraphicsScene>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->laoutCharts();
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QChart *MainWindow::initBar(int flag)
{
    QBarSet *set0 = new QBarSet("Jane");
    QBarSet *set1 = new QBarSet("John");
    QBarSet *set2 = new QBarSet("Axel");
    QBarSet *set3 = new QBarSet("Mary");
    QBarSet *set4 = new QBarSet("Samantha");

    *set0 << 1 * flag << 2* flag << 3* flag << 4* flag << 5* flag << 6* flag;
    *set1 << 5 * flag<< 0* flag << 0* flag << 4* flag << 0* flag << 7* flag;
    *set2 << 3 * flag<< 5* flag << 8* flag << 13* flag << 8* flag << 5* flag;
    *set3 << 5 * flag<< 6* flag << 7* flag << 3* flag << 4* flag << 5* flag;
    *set4 << 9 * flag<< 7* flag << 5* flag << 3* flag << 1* flag << 2* flag;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple barchart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,15*flag);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

void MainWindow::laoutCharts()
{
#if 0  // QWidget 布局
    auto pChart = this->initBar(1000);
    auto pChart2 = this->initBar(1);
    view1 = new QChartView();
    view2 = new QChartView();
    view1->setChart(pChart);
    view2->setChart(pChart2);
    splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(view1);
    splitter->addWidget(view2);
    splitter->setStretchFactor(1, 2);
    splitter->setStretchFactor(2, 1);
    this->setCentralWidget(splitter);
#endif
    mainScene = new QGraphicsScene();
    auto pChart = this->initBar(1000);
    auto pChart2 = this->initBar(1);
    QGraphicsLinearLayout *windowLayout = new QGraphicsLinearLayout(Qt::Vertical);
    windowLayout->addItem(pChart);
    windowLayout->addItem(pChart2);
    windowLayout->setSpacing(0);
    windowLayout->setContentsMargins(0, 0, 0, 0);

    mainWidget = new QGraphicsWidget();
    mainWidget->setLayout(windowLayout);
    mainScene->addItem(mainWidget);

    mainScene->setBackgroundBrush(Qt::darkGray);
    mainView = new QGraphicsView();
    mainView->setScene(mainScene);
    mainView->setBackgroundBrush(Qt::darkBlue);
    this->setCentralWidget(mainView);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
#if 0  // QWidget 布局
    QMargins margin1 = view1->chart()->margins();
    QMargins margin2 = view2->chart()->margins();
    qreal width1 = view1->chart()->plotArea().width();
    qreal width2 = view2->chart()->plotArea().width();
    qreal sub = width1 - width2;
    if(sub > 0){
        view1->chart()->setMargins(QMargins(static_cast<int>(margin1.left() + sub), margin1.top(), margin1.right(), margin1.bottom()));
    }else {
        view2->chart()->setMargins(QMargins(static_cast<int>(margin2.left()-sub), margin2.top(), margin2.right(), margin2.bottom()));
    }
#endif
    mainWidget->setGeometry(QRectF(0, 0, this->width(), this->height()));
}