#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_thread  = new ThreadSync(this);

    connect(m_thread, &ThreadSync::handleResult, this, &MainWindow::onHandleResult);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openBtn_clicked()
{
    m_thread->start();
}

void MainWindow::on_closeBtn_clicked()
{
    if(m_thread->isRunning()){
        m_thread->stop();
        m_thread->wait(2);
    }
}

void MainWindow::on_gernatorbtn_clicked()
{
    this->m_thread->pushToTask(this->data);
    this->data++;
}

void MainWindow::onHandleResult(int result)
{
    ui->label_result->setText(QString("%1").arg(result));
}
