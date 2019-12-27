#include "digiclock.h"

DigiClock::DigiClock(QWidget *parent)
    : QLCDNumber (parent)
{
    QPalette p = this->palette();
    p.setColor(QPalette::Window, Qt::blue);
    this->setPalette(p);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.5);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    this->showTime();
    this->resize(150, 60) ;
    this->showColon = true;
}

void DigiClock::showTime()
{
    QTime time = QTime::currentTime();
    if(this->showColon){
        this->display(time.toString("hh:mm"));
        this->showColon = false;
    }else{
        this->display(time.toString("hh mm"));
        this->showColon = true;
    }
}

void DigiClock::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        this->dragPosition = event->globalPos()- this->frameGeometry().topLeft();
        event->accept();
    }
    if(event->button() == Qt::RightButton){
        this->close();
    }
}

void DigiClock::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos() - this->dragPosition);
        event->accept();
    }
}
