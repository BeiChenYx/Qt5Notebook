#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(Qt::CustomizeWindowHint);
//    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_NoBackground);
    this->initChart1();
    this->initChart2();
    this->initChart3();
    this->initChart4();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initChart1()
{
    // 初始化QCPhart1
    QVector<double> x(101), y(101);
    for(int i = 0; i < 101; ++i){
        x[i] = i / 50.0 - 1;
        y[i] = x[i] * x[i];
    }
    ui->widget_1->addGraph();
    ui->widget_1->graph(0)->setData(x, y);
    ui->widget_1->xAxis->setLabel("x");
    ui->widget_1->yAxis->setLabel("y");
    ui->widget_1->xAxis->setRange(-1, 1);
    ui->widget_1->yAxis->setRange(0, 1);
    ui->widget_1->replot();
}

void MainWindow::initChart2(){
    // 初始化QCPChart2
    ui->widget_2->addGraph();
    ui->widget_2->graph(0)->setPen(QPen(Qt::blue));
    ui->widget_2->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->widget_2->addGraph();
    ui->widget_2->graph(1)->setPen(QPen(Qt::red));
    QVector<double> x(251), y0(251), y1(251);
    for(int i = 0; i < 251; ++i){
        x[i] = i;
        y0[i] = qExp(-i / 150.0) * qCos(i / 10.0);
        y1[i] = qExp(-i / 150.0);
    }
    ui->widget_2->xAxis2->setVisible(true);
    ui->widget_2->xAxis2->setTickLabels(false);
    ui->widget_2->yAxis2->setVisible(true);
    ui->widget_2->yAxis2->setTickLabels(false);
    connect(ui->widget_2->xAxis, SIGNAL(rangeChanged(QCPRange)),\
            ui->widget_2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget_2->yAxis, SIGNAL(rangeChanged(QCPRange)),\
            ui->widget_2->yAxis2, SLOT(setRange(QCPRange)));
    ui->widget_2->graph(0)->setData(x, y0);
    ui->widget_2->graph(1)->setData(x, y1);
    ui->widget_2->graph(0)->rescaleAxes();
    ui->widget_2->graph(1)->rescaleAxes(true);
    ui->widget_2->setInteractions(\
                QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}

void MainWindow::initChart3(){
    ui->widget_3->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    ui->widget_3->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9);
    ui->widget_3->legend->setFont(legendFont);
    ui->widget_3->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    ui->widget_3->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom | Qt::AlignRight);

    ui->widget_3->addGraph(ui->widget_3->yAxis, ui->widget_3->xAxis);
    ui->widget_3->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->widget_3->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->widget_3->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->widget_3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->widget_3->graph(0)->setName("Left maxwell function");

    ui->widget_3->addGraph();
    ui->widget_3->graph(1)->setPen(QPen(Qt::red));
    ui->widget_3->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->widget_3->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    ui->widget_3->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 5));
    ui->widget_3->graph(1)->setName("Left maxwell function");
    QCPErrorBars *errorBars = new QCPErrorBars(ui->widget_3->xAxis, ui->widget_3->yAxis);
    errorBars->removeFromLegend();
    errorBars->setDataPlottable(ui->widget_3->graph(1));

    ui->widget_3->addGraph(ui->widget_3->xAxis2, ui->widget_3->yAxis2);
    ui->widget_3->graph(2)->setPen(QPen(Qt::blue));
    ui->widget_3->graph(2)->setName("High frequency sine");

    ui->widget_3->addGraph(ui->widget_3->xAxis2, ui->widget_3->yAxis2);
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setStyle(Qt::DotLine);
    blueDotPen.setWidth(4);
    ui->widget_3->graph(3)->setPen(blueDotPen);
    ui->widget_3->graph(3)->setName("Sine envelope");

    ui->widget_3->addGraph(ui->widget_3->yAxis2, ui->widget_3->xAxis2);
    ui->widget_3->graph(4)->setPen(QColor(50, 50, 50, 255));
    ui->widget_3->graph(4)->setLineStyle(QCPGraph::lsNone);
    ui->widget_3->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    QVector<double> x0(25), y0(25);
    QVector<double> x1(15), y1(15), y1err(15);
    QVector<double> x2(250), y2(250);
    QVector<double> x3(250), y3(250);
    QVector<double> x4(250), y4(250);
    for(int i = 0; i < 25; ++i){
        x0[i] = 3 * i / 25.0;
        y0[i] = qExp(-x0[i] * x0[i] * 0.8) * (x0[i] * x0[i] + x0[i]);
    }
    for(int i = 0; i < 15; ++i){
        x1[i] = 3 * i / 15.0;
        y1[i] = qExp(-x1[i] * x1[i]) * (x1[i] * x1[i]) * 2.6;
        y1err[i] = y1[0] * 0.25;
    }
    for(int i = 0; i < 250; ++i){
        x2[i] = i / 250.0 * 3 * M_PI;
        x3[i] = x2[i];
        x4[i] = i / 250.0 * 100 - 50;
        y2[i] = qSin(x2[i] * 12) * qCos(x2[i]) * 10;
        y3[i] = qCos(x3[i]) * 10;
        y4[i] = 0.01 * x4[i] * x4[i] + 1.5 * (rand() / (double)RAND_MAX - 0.5) + 1.5 * M_PI;
    }
    ui->widget_3->graph(0)->setData(x0, y0);
    ui->widget_3->graph(1)->setData(x1, y1);
    errorBars->setData(y1err);
    ui->widget_3->graph(2)->setData(x2, y2);
    ui->widget_3->graph(3)->setData(x3, y3);
    ui->widget_3->graph(4)->setData(x4, y4);
    ui->widget_3->xAxis2->setVisible(true);
    ui->widget_3->yAxis2->setVisible(true);
    ui->widget_3->xAxis->setRange(0, 2.7);
    ui->widget_3->yAxis->setRange(0, 2.6);
    ui->widget_3->xAxis2->setRange(0, 3.0 * M_PI);
    ui->widget_3->yAxis2->setRange(-70, 35);

    ui->widget_3->xAxis2->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));
    ui->widget_3->plotLayout()->insertRow(0);
    ui->widget_3->plotLayout()->addElement(0, 0, new QCPTextElement(ui->widget_3, "Way too many graphs in one plot", QFont("sans", 12, QFont::Bold)));

    ui->widget_3->xAxis->setLabel("Bottom axis with outward ticks");
    ui->widget_3->yAxis->setLabel("Left axis label");
    ui->widget_3->xAxis2->setLabel("Top axis lebel");
    ui->widget_3->yAxis2->setLabel("Right axis label");

    ui->widget_3->xAxis->setTickLength(0, 5);
    ui->widget_3->xAxis->setSubTickLength(0, 3);
    ui->widget_3->yAxis2->setTickLength(3, 3);
    ui->widget_3->yAxis2->setSubTickLength(1, 1);
}

void MainWindow::initChart4(){
    ui->widget_4->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    double now = QDateTime::currentDateTime().toTime_t();
    srand(8);
    for(int gi = 0; gi < 5; ++gi){
        ui->widget_4->addGraph();
        QColor color(20 + 200 / 4.0 * gi, 70 * (1.6 - gi / 4.0), 150, 150);
        ui->widget_4->graph()->setLineStyle(QCPGraph::lsLine);
        ui->widget_4->graph()->setPen(QPen(color.lighter(200)));
        ui->widget_4->graph()->setBrush(QBrush(color));

        QVector<QCPGraphData> timeData(250);
        for(int i = 0; i < 250; ++i){
            timeData[i].key = now + 24 * 3600 * i;
            if(i == 0){
                timeData[i].value = (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
            }else{
                timeData[i].value = qFabs(timeData[i-1].value)*(1+0.02/4.0*(4-gi)) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
            }
        }
        ui->widget_4->graph()->data()->set(timeData);
    }
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
    ui->widget_4->xAxis->setTicker(dateTicker);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(10, "a bit\nlow");
    textTicker->addTick(50, "quite\nhigh");
    ui->widget_4->yAxis->setTicker(textTicker);
    ui->widget_4->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->widget_4->xAxis->setLabel("Date");
    ui->widget_4->yAxis->setLabel("Random wobbly lines value");

    ui->widget_4->xAxis2->setVisible(true);
    ui->widget_4->yAxis2->setVisible(true);
    ui->widget_4->xAxis2->setTicks(false);
    ui->widget_4->yAxis2->setTicks(false);
    ui->widget_4->xAxis2->setTickLabels(false);
    ui->widget_4->yAxis2->setTickLabels(false);
    ui->widget_4->xAxis->setRange(now, now+24*3600*249);
    ui->widget_4->yAxis->setRange(0, 60);
    ui->widget_4->legend->setVisible(true);
    ui->widget_4->legend->setBrush(QColor(255, 255, 255, 150));
}

