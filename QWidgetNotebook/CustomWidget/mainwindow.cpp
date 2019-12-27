#include <QDebug>
#include <QPainter>
#include <QScrollBar>
#include <QStyleOption>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QListView>
#include <QRegExpValidator>
#include <QRegExp>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bMousePressed(false),
    m_bDragTop(false),
    m_bDragLeft(false),
    m_bDragRight(false),
    m_bDragBottom(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
#if defined(Q_OS_WIN)
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
#endif
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);

    this->setMouseTracking(true);
    this->setWindowIcon(QIcon(":/imges/ico.png"));

    // important to watch mouse move from all child widgets
    QApplication::instance()->installEventFilter(this);
    this->initUI();
    this->initConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initUI()
{
    ui->buttonGroup_nav->setId(ui->toolButton_data, 0);
    ui->buttonGroup_nav->setId(ui->toolButton_par, 1);
    ui->buttonGroup_nav->setId(ui->toolButton_filter, 2);
    ui->buttonGroup_nav->setId(ui->toolButton_update, 3);
    ui->buttonGroup_nav->setId(ui->toolButton_dev_connect, 4);
    ui->checkBox_status->setChecked(false);

    m_pCanStatusBar = new CanStatusBar(this);
    ui->statusBar->addPermanentWidget(m_pCanStatusBar);
    m_pCanVersion = new QLabel(this);
    m_pCanVersion->setText(tr(" 设备版本: V****"));
    ui->statusBar->addWidget(m_pCanVersion);
    ui->comboBox_baude->addItems(QStringList({"5K", "10K", "20K", "50K",
                                              "100K", "125K", "250K", "500K", "800K", "1000K"}));
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->textBrowser_info->hide();
    ui->tableWidget_rcv->horizontalHeader()->setVisible(true);
    ui->tableWidget_filter_com->horizontalHeader()->setVisible(true);
    ui->tableWidget_filter_com->setColumnWidth(0, 40);
    ui->tableWidget_filter_com->setColumnWidth(1, 40);
    ui->tableWidget_filter_com->setColumnWidth(2, 150);
    ui->tableWidget_filter_com->setColumnWidth(3, 150);
    ui->tableWidget_filter_com->setColumnWidth(4, 150);
    QString regStr("[0-9a-fA-F ]{0,11}");
    ui->lineEdit_id->setValidator(new QRegExpValidator(QRegExp(regStr), this));
    // TODO: 改成模型视图方式实现
    QStringList typeStr({tr("标准帧单ID滤波"), tr("标准帧组ID滤波"), tr("扩展帧单ID滤波"), tr("扩展帧组ID滤波")});
    for (int i=0; i<10; ++i) {
        int rowCom = ui->tableWidget_filter_com->rowCount();
        ui->tableWidget_filter_com->insertRow(rowCom);
        // 序号
        QTableWidgetItem *itemIdCom = new QTableWidgetItem(QString("%1").arg(rowCom+1));
        itemIdCom->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_filter_com->setItem(rowCom, 0, itemIdCom);
        // 使能
        QTableWidgetItem *itemIsCheckedCom = new QTableWidgetItem();
        itemIsCheckedCom->setCheckState(Qt::CheckState::Unchecked);
        itemIsCheckedCom->setTextAlignment(Qt::AlignCenter);
        itemIsCheckedCom->setFlags(Qt::ItemFlag::ItemIsEnabled|Qt::ItemFlag::ItemIsUserCheckable);
        ui->tableWidget_filter_com->setItem(rowCom, 1, itemIsCheckedCom);
        // 类型
        QComboBox *typeCom = new QComboBox();
        typeCom->addItems(typeStr);
        ui->tableWidget_filter_com->setCellWidget(rowCom, 2, typeCom);
        // 开始ID
        QLineEdit *startCom = new QLineEdit();
        startCom->setPlaceholderText(tr("FA FB FC FD"));
        startCom->setValidator(new QRegExpValidator(QRegExp(regStr), this));
        ui->tableWidget_filter_com->setCellWidget(rowCom, 3, startCom);
        // 起始ID
        QLineEdit *endCom = new QLineEdit();
        endCom->setPlaceholderText(tr("FA FB FC FD"));
        endCom->setValidator(new QRegExpValidator(QRegExp(regStr), this));
        ui->tableWidget_filter_com->setCellWidget(rowCom, 4, endCom);
    }
    for(auto pComboBox: findChildren<QComboBox*>()){
        pComboBox->setView(new QListView());
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::initConnections()
{
    connect(ui->buttonGroup_nav, SIGNAL(buttonClicked(QAbstractButton *)),
            this, SLOT(onButtonGroupClicked(QAbstractButton *)));
}
void MainWindow::onButtonGroupClicked(QAbstractButton *btn)
{
    ui->stackedWidget->setCurrentIndex(ui->buttonGroup_nav->id(btn));
    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);
}

void MainWindow::on_toolButton_close_clicked(){this->close();}

void MainWindow::on_toolButton_min_clicked(){this->showMinimized();}

void MainWindow::on_toolButton_max_clicked()
{
    // Tip: toolButton_max 按钮需要在设计师里面打开 checkable 选项
    if(this->windowState().testFlag(Qt::WindowNoState)){
        this->showMaximized();
        ui->toolButton_max->setChecked(true);
        this->update();
    }else if(this->windowState().testFlag(Qt::WindowMaximized)){
        this->showNormal();
        ui->toolButton_max->setChecked(false);
        this->update();
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *) { this->on_toolButton_max_clicked(); }
bool MainWindow::checkBorderDragging(QMouseEvent *event)
{
    if (isMaximized() || isFullScreen()) { return false; }

    QPoint globalMousePos = event->globalPos();
    if (m_bMousePressed) {
        if (m_bDragTop && m_bDragRight) {
            int newHeight = m_StartGeometry.height() + m_StartGeometry.y() - globalMousePos.y();
            int newWidth = globalMousePos.x() - m_StartGeometry.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                setGeometry(m_StartGeometry.x(), globalMousePos.y(), newWidth, newHeight);
                return true;
            }
        }
        else if (m_bDragTop && m_bDragLeft) {
            int newHeight = m_StartGeometry.height() + m_StartGeometry.y() - globalMousePos.y();
            int newWidth = m_StartGeometry.width() + m_StartGeometry.x() - globalMousePos.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                setGeometry(globalMousePos.x(), globalMousePos.y(), newWidth, newHeight);
                return true;
            }
        }
        else if (m_bDragBottom && m_bDragLeft) {
            int newHeight = globalMousePos.y() - m_StartGeometry.y();
            int newWidth = m_StartGeometry.width() + m_StartGeometry.x() - globalMousePos.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                setGeometry(globalMousePos.x(), m_StartGeometry.y(), newWidth, newHeight);
                return true;
            }
        } else if (m_bDragBottom && m_bDragRight) {
            int newHeight = globalMousePos.y() - m_StartGeometry.y();
            int newWidth = globalMousePos.y() - m_StartGeometry.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                resize(newWidth, newHeight);
                return true;
            }
        } else if (m_bDragTop) {
            int newHeight = m_StartGeometry.height() + m_StartGeometry.y() - globalMousePos.y();
            if (newHeight > this->minimumHeight()) {
                setGeometry(m_StartGeometry.x(), globalMousePos.y(), m_StartGeometry.width(),newHeight);
                return true;
            }
        } else if (m_bDragLeft) {
            int newWidth = m_StartGeometry.width() + m_StartGeometry.x() - globalMousePos.x();
            if (newWidth > this->minimumWidth()) {
                setGeometry(globalMousePos.x(), m_StartGeometry.y(), newWidth, m_StartGeometry.height());
                return true;
            }
        } else if (m_bDragRight) {
            int newWidth = globalMousePos.x() - m_StartGeometry.x();
            if (newWidth > this->minimumWidth()) {
                resize(newWidth, m_StartGeometry.height());
                return true;
            }
        } else if (m_bDragBottom) {
            int newHeight = globalMousePos.y() - m_StartGeometry.y();
            if (newHeight > this->minimumHeight()) {
                resize(m_StartGeometry.width(), newHeight);
                return true;
            }
        } else {
            return true;
        }
    } else {
        // no mouse pressed
        if (m_borderPos == MouseBorderPos::LEFT_TOP) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (m_borderPos == MouseBorderPos::RIGHT_TOP) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (m_borderPos == MouseBorderPos::LEFT_BOTTOM) {
            setCursor(Qt::SizeBDiagCursor);
        } else if(m_borderPos == MouseBorderPos::RIGHT_BOTTOM){
            setCursor(Qt::SizeFDiagCursor);
        }else if (m_borderPos == MouseBorderPos::TOP) {
            setCursor(Qt::SizeVerCursor);
        } else if (m_borderPos == MouseBorderPos::LEFT) {
            setCursor(Qt::SizeHorCursor);
        } else if (m_borderPos == MouseBorderPos::RIGHT) {
            setCursor(Qt::SizeHorCursor);
        } else if (m_borderPos == MouseBorderPos::BOTTOM) {
            setCursor(Qt::SizeVerCursor);
        }else {
            m_bDragTop = false;
            m_bDragLeft = false;
            m_bDragRight = false;
            m_bDragBottom = false;
            setCursor(Qt::ArrowCursor);
        }
    }
    return false;
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMousePressed){
        this->move(m_wndPos+ (event->globalPos() - m_mousePos));
    }
    update();
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(isMaximized() || isFullScreen()){return;}

    m_bMousePressed = true;
    m_StartGeometry = this->geometry();
    m_mousePos = event->globalPos();
    m_wndPos = this->pos();

    if (m_borderPos == MouseBorderPos::LEFT_TOP) {
        m_bDragTop = true;
        m_bDragLeft = true;
    } else if (m_borderPos == MouseBorderPos::RIGHT_TOP) {
        m_bDragRight = true;
        m_bDragTop = true;
    } else if (m_borderPos == MouseBorderPos::LEFT_BOTTOM) {
        m_bDragLeft = true;
        m_bDragBottom = true;
    } else if (m_borderPos == MouseBorderPos::RIGHT_BOTTOM){
        m_bDragRight = true;
        m_bDragBottom = true;
    } else if (m_borderPos == MouseBorderPos::TOP) {
        m_bDragTop = true;
    } else if (m_borderPos == MouseBorderPos::LEFT) {
        m_bDragLeft = true;
    } else if (m_borderPos == MouseBorderPos::RIGHT) {
        m_bDragRight = true;
    } else if (m_borderPos == MouseBorderPos::BOTTOM) {
        m_bDragBottom = true;
    } else {
        ;
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (isMaximized() || isFullScreen()) {
        return;
    }

    m_bMousePressed = false;
    bool bSwitchBackCursorNeeded = m_bDragTop || m_bDragLeft || m_bDragRight || m_bDragBottom;
    m_bDragTop = false;
    m_bDragLeft = false;
    m_bDragRight = false;
    m_bDragBottom = false;
    if (bSwitchBackCursorNeeded) {
        setCursor(Qt::ArrowCursor);
    }
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (this->isMaximized() || this->isFullScreen()){
        return QWidget::eventFilter(obj, event);
    }
    // check mouse move event when mouse is moved on any object
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
        m_borderPos = isBorderPos(pMouse->globalPos());
        bool re = false;
        if (pMouse) {
            re = checkBorderDragging(pMouse);
        }
        if((m_borderPos != MouseBorderPos::NO_BORDER || !re) && obj == this){
            return true;
        }
    }
    // press is triggered only on main window
    else if (event->type() == QEvent::MouseButtonPress && obj == this) {
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
        mousePressEvent(pMouse);
        if(m_borderPos != MouseBorderPos::NO_BORDER){ return true; }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (m_bMousePressed) {
            QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
            if (pMouse) { mouseReleaseEvent(pMouse); }
        }
    }
    return QWidget::eventFilter(obj, event);
}

MouseBorderPos MainWindow::isBorderPos(const QPoint &pos, bool isPressed)
{
    Q_UNUSED(isPressed);
    if(leftBorderHit(pos) && topBorderHit(pos)){
        return MouseBorderPos::LEFT_TOP;
    }else if(topBorderHit(pos) && rightBorderHit(pos)){
        return MouseBorderPos::RIGHT_TOP;
    }else if(rightBorderHit(pos) && bottomBorderHit(pos)){
        return MouseBorderPos::RIGHT_BOTTOM;
    }else if(bottomBorderHit(pos) && leftBorderHit(pos)){
        return MouseBorderPos::LEFT_BOTTOM;
    }else {
        if(leftBorderHit(pos)){
            return MouseBorderPos::LEFT;
        }
        else if(rightBorderHit(pos)){
            return MouseBorderPos::RIGHT;
        }
        else if(bottomBorderHit(pos)){
            return MouseBorderPos::BOTTOM;
        }
        else if(topBorderHit(pos)){
            return MouseBorderPos::TOP;
        }
        else {
            return MouseBorderPos::NO_BORDER;
        }
    }
}
bool MainWindow::leftBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    if(pos.x() >= rect.x() && pos.x() <= rect.x() + CONST_DRAG_BORDER_SIZE){
        return true;
    }
    return  false;
}
bool MainWindow::rightBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    int tmp = rect.x() + rect.width();
    if (pos.x() <= tmp && pos.x() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}
bool MainWindow::topBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    if (pos.y() >= rect.y() && pos.y() <= rect.y() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }
    return false;
}
bool MainWindow::bottomBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    int tmp = rect.y() + rect.height();
    if (pos.y() <= tmp && pos.y() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}
