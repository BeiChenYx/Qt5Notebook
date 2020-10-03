#include <QHBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pBtnGroupLeftNav(new QButtonGroup(this)),
    m_pVersionWidget(new VersionInfoWidget("版本号: V1.0.2")),
    m_pGeneralCom(new GeneralCom(this)),
    m_pDoubleDebug(new DoubleDebug(this)),
    m_pSigleDebug(new SigleDebug(this)),
    m_pHelp(new help(this))
{
    ui->setupUi(this);
    this->initUi();
    this->initConnetion();

    // 打包成EXE单个文件无法更新配置文件
//    QTimer::singleShot(2000, this, [this](){
//        QFileInfo file("./ComDebug.ini");
//        if(file.exists()){
//            m_pGeneralCom->ReadInitFile();
//            m_pDoubleDebug->ReadInitFile();
//            m_pSigleDebug->ReadInitFile();
//        }
//    });
}

MainWindow::~MainWindow()
{
//    m_pGeneralCom->WriteInitFile();
//    m_pDoubleDebug->WriteInitFile();
//    m_pSigleDebug->WriteInitFile();
    delete ui;
}

void MainWindow::initUi()
{
    for(auto pComboBox: findChildren<QComboBox*>()){
        pComboBox->setView(new QListView());
    }
    // 初始侧边导航为100px, 导航和内容的比例为 1 : 9
    QList<int> splitterList{100, this->width() - 108};
    ui->splitter->setSizes(splitterList);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 9);
    ui->statusbar->addPermanentWidget(m_pVersionWidget);

    this->addNavStackWidget("generalCom", tr("串口工具"), m_pGeneralCom);
    this->addNavStackWidget("sigleDebug", tr("串口自测"), m_pSigleDebug);
    this->addNavStackWidget("doubleDebug", tr("串口对测"), m_pDoubleDebug);
    this->addNavStackWidget("help", tr("帮助文档"), m_pHelp);
}

void MainWindow::initConnetion()
{
    connect(m_pBtnGroupLeftNav, SIGNAL(buttonClicked(QAbstractButton *)),
                    this, SLOT(on_buttonClickedLeftNav(QAbstractButton *)));

}

void MainWindow::on_buttonClickedLeftNav(QAbstractButton *btn)
{
    ui->stackedWidget->setCurrentIndex(m_pBtnGroupLeftNav->id(btn));
    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);
}

void MainWindow::addNavStackWidget(QString name, QString text, QWidget *widget)
{
    int btnId = m_pTBtnLeftNavVector.length();
    auto pTBtn = new QToolButton(this);
    m_pTBtnLeftNavVector.append(pTBtn);
    pTBtn->setText(text);
    pTBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    pTBtn->setCheckable(true);
    pTBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if(!name.trimmed().isEmpty()){
        pTBtn->setObjectName(name);
    }
    m_pBtnGroupLeftNav->addButton(pTBtn, btnId);
    ui->verticalLayout_left_nav->addWidget(pTBtn);
    ui->stackedWidget->addWidget(widget);
}

void MainWindow::addNavHLine()
{
    QFrame *line = new QFrame(this);
    line->setGeometry(QRect(40, 180, 400, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->raise();
    line->setObjectName("HLine");
    ui->verticalLayout_left_nav->addWidget(line);
}
