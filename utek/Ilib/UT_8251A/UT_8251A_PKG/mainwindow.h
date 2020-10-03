#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QToolButton>
#include "datatest.h"
#include "setparameter.h"
#include "setfilter.h"
#include "upgradedevice.h"
#include "connectingdevices.h"
#include "handlehid.h"
#include "handleserialprotocol.h"


namespace Ui {
class MainWindow;
}

class VersionInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VersionInfoWidget(QString version="V1.0.0", QWidget *parent = nullptr)
        : QWidget(parent){
        this->setObjectName("customStatusBar");
        QHBoxLayout *hLayout = new QHBoxLayout;
        QLabel *label = new QLabel(version, this);
        hLayout->addWidget(label);
        hLayout->setContentsMargins(0, 0, 0, 0);
        this->setLayout(hLayout);
        this->setStyleSheet("#customStatusBar QLabel{color: #FFFFFF;}");
    }
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 框架   --start
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initConnetion();
    void initHidConnection();
    void initSerialConnection();
    void initUi();

    /* 添加自定义的页面到QStackWidget中，并添加左导航栏按钮
     * @name: 左导航按钮需要显示的名字
     * @widget: 需要插入的页面
     */
    void addNavStackWidget(QString objectName, QString text, QWidget *widget, QIcon ico=QIcon());
    // 向导航栏中添加水平分割线
    void addNavHLine();
    // 框架   --end

private slots:
    // 框架   --start
    void on_buttonClickedLeftNav(QAbstractButton *btn);
    // 框架   --end

    void dumpInfoBox(bool status, QString title);

private:
    // 框架   --start
    Ui::MainWindow *ui;
    QButtonGroup *m_pBtnGroupLeftNav;
    QVector<QToolButton*> m_pTBtnLeftNavVector;
    VersionInfoWidget *m_pVersionWidget;
    // 框架   --end

    // 各页面对象
    DataTest *m_pDataTest;
    SetParameter *m_pSetPar;
    SetFilter *m_pSetFilter;
    UpgradeDevice *m_pUpgradeDev;
    ConnectingDevices *m_pConectingDev;
    HandleHid *m_pHandleHid;
    HandleSerialProtocol *m_pHandleSerial;

    // 固件版本号
    QLabel m_hardwareLabel;
    QTimer *m_pTxTimer;

    QString m_id;
    QString m_frameType;
    QString m_frameFormat;
    QString m_data;

    bool m_isDataTest = false;

    // 设备连接状态
    QToolButton *m_pDevStatus;
};

#endif // MAINWINDOW_H
