#include <QFontDatabase>
#include <QMouseEvent>
#include "customdialog.h"
#include "ui_customdialog.h"


CustomDialog::CustomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    this->setWindowIcon(QIcon(":/imges/SF.ico"));

    // 添加字体文件, 创建字体
    QFont font;
    int fontId = QFontDatabase::addApplicationFont(":/images/fontawesome-webfont.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    font.setFamily(fontFamilies.at(0));
    font.setPointSize(10);
    // 标题栏四个按键
    ui->toolButton_d_close->setFont(font);
    ui->toolButton_d_reset_max->setFont(font);
    ui->toolButton_d_min->setFont(font);
    ui->toolButton_d_close->setText(QChar(0xf00d));
    ui->toolButton_d_reset_max->setText(QChar(0xf065));
    ui->toolButton_d_min->setText(QChar(0xf068));
    // important to watch mouse move from all child widgets
    QApplication::instance()->installEventFilter(this);
    this->resize(300, 400);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}
void CustomDialog::setTitle(QString title) { ui->label_d_title->setText(title); }

void CustomDialog::setLayout(QLayout *layout)
{
    ui->widget_main->setLayout(layout);
}

void CustomDialog::on_toolButton_d_min_clicked() { this->showMinimized(); }

void CustomDialog::on_toolButton_d_reset_max_clicked()
{
    if(this->windowState().testFlag(Qt::WindowNoState)){
        this->showMaximized();
        ui->toolButton_d_reset_max->setText(QChar(0xf066));
    }else if(this->windowState().testFlag(Qt::WindowMaximized)){
        this->showNormal();
        ui->toolButton_d_reset_max->setText(QChar(0xf065));
    }
    this->update();
}

void CustomDialog::on_toolButton_d_close_clicked() { this->close(); }

bool CustomDialog::checkBorderDragging(QMouseEvent *event)
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
void CustomDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMousePressed){
        this->move(m_wndPos+ (event->globalPos() - m_mousePos));
    }
    update();
}
void CustomDialog::mouseDoubleClickEvent(QMouseEvent *) { this->on_toolButton_d_reset_max_clicked(); }
void CustomDialog::mousePressEvent(QMouseEvent *event)
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
void CustomDialog::mouseReleaseEvent(QMouseEvent *event)
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
bool CustomDialog::eventFilter(QObject *obj, QEvent *event)
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

CustomDialog::MouseBorderPos CustomDialog::isBorderPos(const QPoint &pos, bool isPressed)
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
bool CustomDialog::leftBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    if(pos.x() >= rect.x() && pos.x() <= rect.x() + CONST_DRAG_BORDER_SIZE){
        return true;
    }
    return  false;
}
bool CustomDialog::rightBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    int tmp = rect.x() + rect.width();
    if (pos.x() <= tmp && pos.x() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}
bool CustomDialog::topBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    if (pos.y() >= rect.y() && pos.y() <= rect.y() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }
    return false;
}
bool CustomDialog::bottomBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    int tmp = rect.y() + rect.height();
    if (pos.y() <= tmp && pos.y() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}

