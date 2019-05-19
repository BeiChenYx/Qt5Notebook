#include "humiture.h"
#include "ui_humiture.h"
#include <QtDebug>

Humiture::Humiture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Humiture)
{
    ui->setupUi(this);
    this->initUI();

}

Humiture::~Humiture()
{
    delete ui;
}

void Humiture::initUI()
{
//    this->resize(960, 540);
    this->resize(1366, 768);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->windowStatus = WindowStatus::NORMAL;
    this->isPressed = false;
    this->stretchState = WindowStretchRectState::NO_SELECT;
    // 开启鼠标跟踪
    this->setMouseTracking(true);

    this->pHome = new Home(this);
    this->pComConfig = new ComConfig(this);
    this->pModbusTest = new ModbusTest(this);
    this->pReadCmd = new ReadCmd(this);
    this->pModifyCmd = new ModifyCmd(this);
    ui->stackedWidget->insertWidget(0, this->pHome);
    ui->stackedWidget->insertWidget(1, this->pComConfig);
    ui->stackedWidget->insertWidget(2, this->pModbusTest);
    ui->stackedWidget->insertWidget(3, this->pReadCmd);
    ui->stackedWidget->insertWidget(4, this->pModifyCmd);
    ui->stackedWidget->setCurrentIndex(0);

}


void Humiture::resizeEvent(QResizeEvent *e)
{
    // 执行 Humiture 默认操作
    QWidget::resizeEvent(e);

    qDebug() << this->width() << ": " << this->height();
    ui->widget_title->setMaximumHeight(this->height() / 9);
}

void Humiture::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(this->windowStatus == WindowStatus::MAX){
        this->showNormal();
        this->windowStatus = WindowStatus::NORMAL;
    }else if(windowStatus == WindowStatus::NORMAL){
        this->showMaximized();
        this->windowStatus = WindowStatus::MAX;
    }
    QWidget::mouseDoubleClickEvent(event);
}

void Humiture::mousePressEvent(QMouseEvent *event)
{
    if(this->windowStatus != WindowStatus::MAX){
        this->startMovePos = event->globalPos();
        this->isPressed = true;
    }

    if(this->stretchState != WindowStretchRectState::NO_SELECT && event->button() == Qt::LeftButton){
        this->isPressed = true;
        this->startPoint = this->mapToGlobal(event->pos());
        this->windowRectBeforeStrech = this->geometry();
    }

    QWidget::mousePressEvent(event);
}

void Humiture::mouseMoveEvent(QMouseEvent *event)
{
    if(this->isPressed){
        QPoint movePoint = event->globalPos() - this->startMovePos;
        QPoint widgetPos = this->pos();
        this->startMovePos = event->globalPos();
        this->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());

        // 拉伸: 如果左键已经按下，则记录第二个点的位置，并且更新窗口大小；
        if(this->stretchState != WindowStretchRectState::NO_SELECT){
            this->endPoint = this->mapToGlobal(event->pos());
            updateWindowSize();
        }
    }else {
        // 鼠标未按下更新鼠标样式
        QPoint cursorPos = event->pos();
        this->stretchState = getCurrentStretchState(cursorPos);
        updateMouseStyle(this->stretchState);
    }
    QWidget::mouseMoveEvent(event);
}

void Humiture::mouseReleaseEvent(QMouseEvent *event)
{
    this->isPressed = false;
    calculateMarginsRect();
    QWidget::mouseReleaseEvent(event);
}
void Humiture::showEvent(QShowEvent *event)
{
    calculateMarginsRect();
    QWidget::showEvent(event);
}

void Humiture::calculateMarginsRect()
{
    this->leftTopRect = QRect(0, 0, MARGINS, MARGINS);
    this->leftRect = QRect(0, MARGINS, MARGINS, this->height() - 2 * MARGINS);
    this->leftBottomRect = QRect(0, this->height() - MARGINS, MARGINS, MARGINS);
    this->topRect = QRect(MARGINS, 0, this->width() - 2 * MARGINS, MARGINS);
    this->bottomRect = QRect(MARGINS, this->height() - MARGINS, this->width() - 2 * MARGINS, MARGINS);
    this->rightTopRect = QRect(this->width() - MARGINS, 0, MARGINS, MARGINS);
    this->rightRect = QRect(this->width() - MARGINS, MARGINS, MARGINS, this->height() - 2 * MARGINS);
    this->rightBottomRect = QRect(this->width() - MARGINS, this->height() - MARGINS, MARGINS, MARGINS);
}

WindowStretchRectState Humiture::getCurrentStretchState(QPoint cursorPos)
{
    WindowStretchRectState stretchState;
    if (this->leftTopRect.contains(cursorPos)){
        stretchState = LEFT_TOP_RECT;
    }else if (this->rightTopRect.contains(cursorPos)){
        stretchState = RIGHT_TOP_RECT;
    }else if (this->rightBottomRect.contains(cursorPos)){
        stretchState = RIGHT_BOTTOM_RECT;
    }else if (this->leftBottomRect.contains(cursorPos)){
        stretchState = LEFT_BOTTOM_RECT;
    }else if (this->topRect.contains(cursorPos)){
        stretchState = TOP_BORDER;
    }else if (this->rightRect.contains(cursorPos)){
        stretchState = RIGHT_BORDER;
    }else if (this->bottomRect.contains(cursorPos)){
        stretchState = BOTTOM_BORDER;
    }else if (this->leftRect.contains(cursorPos)){
        stretchState = LEFT_BORDER;
    }else{
        stretchState = NO_SELECT;
    }
    return stretchState;
}

void Humiture::updateMouseStyle(WindowStretchRectState stretchState)
{
    switch (stretchState)
    {
    case NO_SELECT:
        setCursor(Qt::ArrowCursor);
        break;
    case LEFT_TOP_RECT:
    case RIGHT_BOTTOM_RECT:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TOP_BORDER:
    case BOTTOM_BORDER:
        setCursor(Qt::SizeVerCursor);
        break;
    case RIGHT_TOP_RECT:
    case LEFT_BOTTOM_RECT:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case LEFT_BORDER:
    case RIGHT_BORDER:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void Humiture::updateWindowSize()
{
    // 拉伸时要注意设置窗口最小值;
    QRect windowRect = this->windowRectBeforeStrech;
    int delValue_X = this->startPoint.x() - this->endPoint.x();
    int delValue_Y = this->startPoint.y() - this->endPoint.y();

    if (this->stretchState == LEFT_BORDER){
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setX(topLeftPoint.x() - delValue_X);
        windowRect.setTopLeft(topLeftPoint);
    }else if (this->stretchState == RIGHT_BORDER){
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setX(bottomRightPoint.x() - delValue_X);
        windowRect.setBottomRight(bottomRightPoint);
    }else if (this->stretchState == TOP_BORDER){
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setY(topLeftPoint.y() - delValue_Y);
        windowRect.setTopLeft(topLeftPoint);
    }else if (this->stretchState == BOTTOM_BORDER){
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setY(bottomRightPoint.y() - delValue_Y);
        windowRect.setBottomRight(bottomRightPoint);
    }else if (this->stretchState == LEFT_TOP_RECT){
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setX(topLeftPoint.x() - delValue_X);
        topLeftPoint.setY(topLeftPoint.y() - delValue_Y);
        windowRect.setTopLeft(topLeftPoint);
    }else if (this->stretchState == RIGHT_TOP_RECT){
        QPoint topRightPoint = windowRect.topRight();
        topRightPoint.setX(topRightPoint.x() - delValue_X);
        topRightPoint.setY(topRightPoint.y() - delValue_Y);
        windowRect.setTopRight(topRightPoint);
    }else if (this->stretchState == RIGHT_BOTTOM_RECT){
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setX(bottomRightPoint.x() - delValue_X);
        bottomRightPoint.setY(bottomRightPoint.y() - delValue_Y);
        windowRect.setBottomRight(bottomRightPoint);
    }else if (this->stretchState == LEFT_BOTTOM_RECT){
        QPoint bottomLeftPoint = windowRect.bottomLeft();
        bottomLeftPoint.setX(bottomLeftPoint.x() - delValue_X);
        bottomLeftPoint.setY(bottomLeftPoint.y() - delValue_Y);
        windowRect.setBottomLeft(bottomLeftPoint);
    }
    this->setGeometry(windowRect);
}

void Humiture::on_pushButton_min_clicked()
{
    this->showMinimized();
    this->windowStatus = WindowStatus::MIN;
}

void Humiture::on_pushButton_max_clicked()
{
    if(this->windowStatus == WindowStatus::MAX){
        this->showNormal();
        ui->pushButton_max->setStyleSheet("border-image: url(:/image/max.png);");
    }else if (this->windowStatus == WindowStatus::NORMAL) {
        this->showMaximized();
        ui->pushButton_max->setStyleSheet("border-image: url(:/image/reset.png);");
    }
}

void Humiture::on_pushButton_close_clicked()
{
    // 关闭程序
    this->close();
}

void Humiture::on_pushButton_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Humiture::on_pushButton_comSet_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Humiture::on_pushButton_Modbus_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Humiture::on_pushButton_readCmd_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Humiture::on_pushButton_modifyCmd_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}
