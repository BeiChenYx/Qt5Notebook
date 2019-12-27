#include <QPainter>
#include <QTimer>
#include <QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    this->setWindowTitle(tr("时钟示例"));
    this->resize(200, 200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QColor minuteColor(0, 127, 127);
    QColor hourColor(127, 0, 127);
    // 秒钟指针, 分钟指针，时钟指针的坐标，使用矩形画三个指针
    static const QPointF secondHand[2] = {
        QPointF(-2, -80), QPointF(2, 0)
    };
    static const QPointF minuteHand[2] = {
        QPointF(-4, -60), QPointF(4, 0)
    };
    static const QPointF hourHand[2] = {
        QPointF(-6, -40), QPointF(6, 0)
    };

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 移动逻辑坐标到窗口中心
    painter.translate(this->width()/2, this->height()/2);
    // 缩放逻辑坐标保证绘画区成正方形
    int side = qMin(this->width(), this->height());
    painter.scale(side / 200.0, side / 200.0);
    // 保存painter状态
    painter.save();
    // 画表盘
    // 画分钟刻度
    for(int i = 0; i < 60; ++i){
        painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
    //先画时钟刻度
    painter.restore();// 恢复painter
    painter.setPen(hourColor);
    for(int i = 0; i < 12; ++i){
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    QTime time = QTime::currentTime();

    // 画时钟指针
    painter.restore();
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawRect(QRectF(hourHand[0], hourHand[1]));

    // 画分钟指针
//    painter.restore();
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawRect(QRectF(minuteHand[0], minuteHand[1]));

    // 画秒钟指针
//    painter.restore();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::lightGray);
    painter.rotate(6.0 * (time.second()));
    painter.drawRect(QRectF(secondHand[0], secondHand[1]));
}
