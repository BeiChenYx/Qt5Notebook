#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QLabel>
#include <QMenu>
#include <QLineEdit>
#include <QAction>
#include <QTimer>
#include "canstatusbar.h"

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

enum ConnectStatus{
    NO_CONNECT = 0,
    HID_CONNECT = 1,
    COM_CONNECT = 2,
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonGroupClicked(QAbstractButton *);
    void on_toolButton_close_clicked();
    void on_toolButton_min_clicked();
    void on_toolButton_max_clicked();
    MouseBorderPos isBorderPos(const QPoint &pos, bool isPressed=false);
    bool leftBorderHit(const QPoint &pos);
    bool rightBorderHit(const QPoint &pos);
    bool topBorderHit(const QPoint &pos);
    bool bottomBorderHit(const QPoint &pos);

protected:
    bool checkBorderDragging(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

    void initUI();
    void initConnections();
    void updateConnectUI();
    void dumpInfoBox(bool status, QString title);


private:
    Ui::MainWindow *ui;
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
    CanStatusBar *m_pCanStatusBar;
    QLabel *m_pCanVersion;
};

#endif // MAINWINDOW_H
