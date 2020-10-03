#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include "handlemodbus.h"


namespace Ui {
class CustomDialog;
}



// TODO: 自定义QDialog的代码有种 移动和缩放代码和主窗口的一样，可以抽象出来做父类
class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QWidget *parent = nullptr);
    virtual ~CustomDialog();

    enum MouseBorderPos{
        NO_BORDER = 0,
        LEFT = 1,
        LEFT_TOP = 2,
        TOP = 3,
        RIGHT_TOP = 4,
        RIGHT = 5,
        RIGHT_BOTTOM = 6,
        BOTTOM = 7,
        LEFT_BOTTOM = 8
    };

    // 设置标题
    void setTitle(QString title);
    // 设置内容区域的布局
    void setLayout(QLayout *layout);

protected:
    // 自定义标题栏的所需函数  start
    MouseBorderPos isBorderPos(const QPoint &pos, bool isPressed=false);
    bool leftBorderHit(const QPoint &pos);
    bool rightBorderHit(const QPoint &pos);
    bool topBorderHit(const QPoint &pos);
    bool bottomBorderHit(const QPoint &pos);
    bool checkBorderDragging(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    // 自定义标题栏的所需函数  end

private slots:
    void on_toolButton_d_min_clicked();
    void on_toolButton_d_reset_max_clicked();
    void on_toolButton_d_close_clicked();

private:
    Ui::CustomDialog *ui;

    // 自定义标题栏的所需成员  start
    QRect m_StartGeometry;
    const quint8 CONST_DRAG_BORDER_SIZE = 10;
    bool m_bMousePressed;
    bool m_bDragTop;
    bool m_bDragLeft;
    bool m_bDragRight;
    bool m_bDragBottom;
    volatile MouseBorderPos m_borderPos;
    QPoint m_mousePos;
    QPoint m_wndPos;
    // 自定义标题栏的所需成员  end
};

// 定义自定义消息类
class CustomWarrningBox : public CustomDialog{
    Q_OBJECT
public:
    explicit CustomWarrningBox(QString title, QString info, QWidget *parent=nullptr) :
        CustomDialog(parent)
    {
        this->setWindowTitle(title);
        this->setTitle(title);
        m_pInfo = new QLabel(info);
        m_pInfo->setStyleSheet("font-size: 24px; color: rgb(255, 170, 0);");
        m_pInfo->setAlignment(Qt::AlignCenter);
        QHBoxLayout *pLayout = new QHBoxLayout();
        pLayout->addWidget(m_pInfo);
        this->setLayout(pLayout);
        this->setModal(true);
        this->resize(300, 100);
    }
    virtual ~CustomWarrningBox(){}
    void setInfo(QString info){
        m_pInfo->setText(info);
    }
    static void customWarrningBox(QString title, QString info){
        static CustomWarrningBox handle(title, info);
        handle.setInfo(info);
        handle.setTitle(title);
        handle.setWindowTitle(title);
        handle.show();
    }
private:
    QLabel *m_pInfo;
};

class CustomErrorBox : public CustomDialog{
    Q_OBJECT
public:
    explicit CustomErrorBox(QString title, QString info, QWidget *parent=nullptr) :
        CustomDialog(parent)
    {
        this->setWindowTitle(title);
        this->setTitle(title);
        m_pInfo = new QLabel(info);
        m_pInfo->setStyleSheet("font-size: 24px; color: rgb(240, 0, 0);");
        m_pInfo->setAlignment(Qt::AlignCenter);
        QHBoxLayout *pLayout = new QHBoxLayout();
        pLayout->addWidget(m_pInfo);
        setLayout(pLayout);
        this->setModal(true);
        this->resize(300, 100);
    }

    virtual ~CustomErrorBox(){}
    void setInfo(QString info){
        m_pInfo->setText(info);
    }
    static void customErrorBox(QString title, QString info){
        static CustomErrorBox handle(title, info);
        handle.setInfo(info);
        handle.setTitle(title);
        handle.setWindowTitle(title);
        handle.show();
    }
private:
    QLabel *m_pInfo;
};
class CustomInfoBox : public CustomDialog{
    Q_OBJECT
public:
    explicit CustomInfoBox(QString title, QString info, QWidget *parent=nullptr) :
        CustomDialog(parent)
    {
        this->setWindowTitle(title);
        this->setTitle(title);
        m_pInfo = new QLabel(info);
        m_pInfo->setStyleSheet("font-size: 24px; color: rgb(0, 0, 0);");
        m_pInfo->setAlignment(Qt::AlignCenter);
        QHBoxLayout *pLayout = new QHBoxLayout();
        pLayout->addWidget(m_pInfo);
        this->setLayout(pLayout);
        this->setModal(true);
        this->resize(300, 100);
    }
    virtual ~CustomInfoBox(){}
    void setInfo(QString info){
        m_pInfo->setText(info);
    }
    static void customInfoBox(QString title, QString info){
        static CustomInfoBox handle(title, info);
        handle.setInfo(info);
        handle.setTitle(title);
        handle.setWindowTitle(title);
        handle.show();
    }
private:
    QLabel *m_pInfo;
};

class Progressbar: public CustomDialog{
    Q_OBJECT
public:
    explicit Progressbar(QString title, QString info, QWidget *parent=nullptr) :
        CustomDialog(parent)
    {
        this->setWindowTitle(title);
        this->setTitle(title);
        pPro = new QProgressBar;
        pInfo = new QLabel(info);
        QHBoxLayout *pLayout = new QHBoxLayout();
        pLayout->addWidget(pInfo);
        pLayout->addWidget(pPro);
        this->setLayout(pLayout);
        this->setModal(true);
        connect(&timer, &QTimer::timeout, this, [this](){
            this->timer.stop();
            this->close();
        });
        this->resize(300, 80);
    }
    virtual ~Progressbar(){}

public slots:
    void onSetValue(int value){
        this->pPro->setValue(value);
        if(value == this->pPro->maximum()){
            this->timer.start(500);
        }
    }
    void onSetError(QString msg) {
        this->pInfo->setText(msg);
    }
    void onSetRange(int min, int max){
        this->onSetValue(0);
        this->pPro->setRange(min, max);
    }
private:
    QProgressBar *pPro;
    QLabel *pInfo;
    QTimer timer;
};

static bool isHaveAuthority(){
    if(g_indexUser < 2){
        CustomWarrningBox::customWarrningBox("权限", "没有操作权限，请登录");
        return false;
    }
    return true;
}

#endif // CUSTOMDIALOG_H
