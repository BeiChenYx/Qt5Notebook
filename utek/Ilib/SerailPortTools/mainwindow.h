#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "generalcom.h"
#include "sigledebug.h"
#include "doubledebug.h"
#include "help.h"

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initConnetion();
    void initUi();

    /* 添加自定义的页面到QStackWidget中，并添加左导航栏按钮
     * @name: 左导航按钮需要显示的名字
     * @widget: 需要插入的页面
     */
    void addNavStackWidget(QString name, QString text, QWidget *widget);
    // 向导航栏中添加水平分割线
    void addNavHLine();

private slots:
    void on_buttonClickedLeftNav(QAbstractButton *btn);

private:
    Ui::MainWindow *ui;
    QButtonGroup *m_pBtnGroupLeftNav;
    QVector<QToolButton*> m_pTBtnLeftNavVector;
    VersionInfoWidget *m_pVersionWidget;

    GeneralCom  *m_pGeneralCom;
    DoubleDebug *m_pDoubleDebug;
    SigleDebug *m_pSigleDebug;
    help *m_pHelp;
};

#endif // MAINWINDOW_H
