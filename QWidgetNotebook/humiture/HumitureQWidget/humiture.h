#ifndef HUMITURE_H
#define HUMITURE_H

#include <QWidget>
#include <QMouseEvent>
#include <QShowEvent>
#include <QRect>
#include <QPainter>
#include <QStyleOption>

#include "home.h"
#include "comconfig.h"
#include "modbustest.h"
#include "readcmd.h"
#include "modifycmd.h"
#include "IM_modbus.h"

namespace Ui {
class Humiture;
}

enum WindowStatus {
    MAX = 0,
    NORMAL = 1,
    MIN = 2,
};

// 定义当前鼠标所处状态;
enum WindowStretchRectState
{
    NO_SELECT = 0,              // 鼠标未进入下方矩形区域;
    LEFT_TOP_RECT,              // 鼠标在左上角区域;
    TOP_BORDER,                 // 鼠标在上边框区域;
    RIGHT_TOP_RECT,             // 鼠标在右上角区域;
    RIGHT_BORDER,               // 鼠标在右边框区域;
    RIGHT_BOTTOM_RECT,          // 鼠标在右下角区域;
    BOTTOM_BORDER,              // 鼠标在下边框区域;
    LEFT_BOTTOM_RECT,           // 鼠标在左下角区域;
    LEFT_BORDER                 // 鼠标在左边框区域;
};


// 边框可拉伸的 边距
const int MARGINS = 5;

class Humiture : public QWidget
{
    Q_OBJECT

public:
    explicit Humiture(QWidget *parent = nullptr);
    ~Humiture();
    virtual void resizeEvent(QResizeEvent* e);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *event);

    // 计算拉伸的 8 个位置的Rect
    void calculateMarginsRect();
    WindowStretchRectState getCurrentStretchState(QPoint cursorPos);
    void updateMouseStyle(WindowStretchRectState stretchState);
    // 拉伸窗口过程中，根据记录的坐标更新窗口大小;
    void updateWindowSize();


    void initUI();
    void initConnections();

private slots:
    void on_pushButton_min_clicked();
    void on_pushButton_max_clicked();
    void on_pushButton_close_clicked();

    void on_pushButton_home_clicked();
    void on_pushButton_comSet_clicked();
    void on_pushButton_Modbus_clicked();
    void on_pushButton_readCmd_clicked();
    void on_pushButton_modifyCmd_clicked();

    // 工作线程的结果信号对应的槽
//    void onOpenResult(bool result, QVariant msg);
//    void onCloseResult();
    void onHomeResult(QVariant msg);
//    void onModbusTestResult(QVariant msg);
//    void onReadCmdResult(QVariant msg);
//    void onModifyCmdResult(QVariant msg);
    void onRecordResult(QVariant msg);
    void onExitModbus();

    // 其他页面发过来的任务，需要通过工作线程的接口发布任务
    void onTask(QVariant msg);

private:
    Ui::Humiture *ui;
    WindowStatus    windowStatus;
    bool            isPressed;
    // 计算拖动时保存移动开始的位置
    QPoint          startMovePos;

    // 计算拉伸的8个位置
    QRect           leftTopRect;
    QRect           leftRect;
    QRect           leftBottomRect;
    QRect           topRect;
    QRect           bottomRect;
    QRect           rightTopRect;
    QRect           rightRect;
    QRect           rightBottomRect;

    WindowStretchRectState  stretchState;
    // 记录拉伸鼠标当前位置
    QPoint                  startPoint;
    QPoint                  endPoint;
    // 记录拉伸前窗口位置和大小
    QRect                   windowRectBeforeStrech;

    Home                    *pHome;
    ComConfig               *pComConfig;
    ModbusTest              *pModbusTest;
    ReadCmd                 *pReadCmd;
    ModifyCmd               *pModifyCmd;

    // 工作线程, 主要是 modbus 通信
    WorkThread              *pWorkThread;
};

#endif // HUMITURE_H
