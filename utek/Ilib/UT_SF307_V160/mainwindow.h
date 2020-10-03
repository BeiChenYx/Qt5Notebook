#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include "homepage.h"
#include "datarecordwidget.h"
#include "scandevices.h"
#include "parsetone.h"
#include "parsettwo.h"
#include "parsetthree.h"
#include "parsetfour.h"
#include "parsetfive.h"
#include "handlemodbus.h"
#include "statuswidget.h"
#include "updatedevice.h"

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

signals:
    void updateDevice();
    void setAlarmHandle(std::weak_ptr<HandleModbus> handle);
    void alarmPageInfo(int page, int remainder);

protected:
    void initUi();
    void initConnection();

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

    /*
     * @brief: 导航栏中主页和参数设置的按键进行切换显示
     * @status: true 显示主页且隐藏参数设置，false 隐藏主页且显示主页
     */
    void setHomeSetSwitch(bool status);

private slots:
    // 状态栏四个按键的槽函数
    void on_toolButton_close_window_clicked();
    void on_toolButton_max_reset_clicked();
    void on_toolButton_min_clicked();
    void on_toolButton_full_clicked();

    // 按钮组触发的信号槽
    void onButtonGroupClicked(QAbstractButton *);

    void on_toolButton_par_set_clicked();
    void on_toolButton_goto_home_clicked();
    void on_toolButton_switch_device_clicked();

    // 滑动设备IP控件的内容发生改变的槽
    void onTumblerValueChanged(QString value, QString oldValue);
    void onTumblerIndexChanged(int index, int oldIndex);

    // 获取固件版本号，MAC地址
    void getHardWareMac();

    // 用户管理
    void on_pushButton_load_clicked();
    void on_pushButton_unload_clicked();
    void on_pushButton_auto_unload_clicked();
    void on_pushButton_modify_clicked();
    void setBtnStatus(bool flag);

    void openDevice(QString ip);
    void closeDevice();

    void on_pushButton_5_clicked();
    void on_pushButton_update_clicked();
    void setUpdateBtnStatus(bool status);

    // 报警记录查询
//    void on_toolButton_pre_day_clicked();
//    void on_toolButton_next_day_clicked();
//    void selectAlarm(QDate date);
    // 获取报警总个数
    void getAlarmCount();
    void getAlarmLog(int minIndex, int maxIndex);
    void on_pushButton_pre_day_clicked();
    void on_pushButton_next_day_clicked();

private:
    Ui::MainWindow *ui;

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

    HomePage *m_pHomePage;
    DataRecordWidget *m_pDataRecordPage;
    ScanDevices *m_pScanDevicesPage;
    StatusWidget *m_pStatusWidget;

    std::shared_ptr<HandleModbus> m_pHandleModbus;

    QString m_mac;
    QString m_ip;
    QString m_hardware;

    QTimer *m_pAutoUnLoadTimer;

    // 升级
    std::shared_ptr<UpdateDevice> m_pUpdateHandle;
    QThread* m_pUpdateThread = nullptr;

    // 报警记录
//    std::shared_ptr<AlarmTaskTrhead> m_pAlarmHandle;
//    QThread* m_pAlarmThread = nullptr;
    // 商和余数
    int m_n;
    int m_m;
};

#endif // MAINWINDOW_H
