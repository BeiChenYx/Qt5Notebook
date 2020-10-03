#include <QHBoxLayout>
#include "FrameLessWidget/framelesswidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pBtnGroupLeftNav(new QButtonGroup(this)),
    m_pVersionWidget(new VersionInfoWidget("版本号: V1.0.14 ")),
    m_pDataTest(new DataTest(this)),
    m_pSetPar(new SetParameter(this)),
    m_pSetFilter(new SetFilter(this)),
    m_pUpgradeDev(new UpgradeDevice(this)),
    m_pConectingDev(new ConnectingDevices(this)),
    m_pHandleHid(new HandleHid()),
    m_pHandleSerial(new HandleSerialProtocol(this)),
    m_pTxTimer(new QTimer(this)),
    m_pDevStatus(new QToolButton(this))
{
    // 框架   --start
    ui->setupUi(this);
    qRegisterMetaType<QVector<int> >("vector_int");
    qRegisterMetaType<QList<QStringList> >("filter");
    qRegisterMetaType<TransiteParameterUi>("TransiteParameterUi");
    qRegisterMetaType<SerialPortParameterUi >("SerialPortParameterUi ");

    this->initUi();
    this->initConnetion();
    // 框架   --end

    setCallBack(DataTest::canDataFunc, m_pDataTest);

    m_pHandleHid->start();
    m_pSetPar->setHandle(m_pHandleHid);
    m_pSetPar->setHandleSerial(m_pHandleSerial);
    m_pSetFilter->setHandle(m_pHandleHid);
    m_pSetFilter->setHandleSerial(m_pHandleSerial);
    m_pUpgradeDev->setHandle(m_pHandleHid);
    m_pUpgradeDev->setHandleSerial(m_pHandleSerial);

    m_pConectingDev->setHandle(m_pHandleSerial);
}

MainWindow::~MainWindow()
{
    m_pHandleHid->exitWorkThread();
    m_pHandleHid->quit();
    m_pHandleHid->wait(200);
    delete ui;
}

void MainWindow::initUi()
{
    // 框架   --start
    // 初始侧边导航为100px, 导航和内容的比例为 1 : 9
    QList<int> splitterList{100, this->width() - 108};
    ui->splitter->setSizes(splitterList);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 9);
    ui->statusbar->addPermanentWidget(m_pVersionWidget);
    for(auto pComboBox: findChildren<QComboBox*>()){
        pComboBox->setView(new QListView());
    }
    // 框架   --end

    m_hardwareLabel.setText(tr("  设备版本: V***"));
    m_hardwareLabel.setStyleSheet("color: #FFFFFF;");
    ui->statusbar->addWidget(&m_hardwareLabel);
    this->addNavStackWidget("DataTest", tr("通信测试"), m_pDataTest);
    this->addNavStackWidget("SetParameter", tr("参数设置"), m_pSetPar);
    this->addNavStackWidget("SetFilter", tr("滤波设置"), m_pSetFilter);
    this->addNavStackWidget("UpgradeDevice", tr("设备升级"), m_pUpgradeDev);
    this->addNavStackWidget("ConnectingDevices", tr("设备连接"), m_pConectingDev);

    auto canBaudrates = m_pHandleHid->canBaudrates();
    auto serialPortBaudrates = m_pHandleHid->serialPortBaudrates();
    m_pSetPar->setBaudrates(canBaudrates, serialPortBaudrates);

    m_pDevStatus->setEnabled(false);
    m_pDevStatus->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pDevStatus->setCheckable(true);
    m_pDevStatus->setChecked(false);
    m_pDevStatus->setObjectName("DevStatus");
    m_pDevStatus->setStyleSheet("#DevStatus:!enabled,\
                                #DevStatus{\
                                    background-color:none;\
                                    border:none;\
                                    color: #FFFFFF;\
                                }\
                                #DevStatus:checked{\
                                    image: url(:/images/connect.png);\
                                }\
                                #DevStatus:!checked{\
                                    image: url(:/images/disconnect.png);\
                                }");
    ui->statusbar->insertPermanentWidget(1, m_pDevStatus);
}

void MainWindow::initConnetion()
{
    // 框架   --start
    connect(m_pBtnGroupLeftNav, SIGNAL(buttonClicked(QAbstractButton *)),
                    this, SLOT(on_buttonClickedLeftNav(QAbstractButton *)));
    // 框架   --end
    // hid定时器发送数据处理
    connect(m_pTxTimer, &QTimer::timeout, this, [this](){
        HidTaskPar par;
        par.m_id = m_id;
        par.m_frameType = m_frameType;
        par.m_frameFormat = m_frameFormat;
        par.m_data = m_data;
        m_pHandleHid->pushToTask(HidTask::WRITE_CAN, par);
    });

    // 通信测试单词发送
    connect(m_pDataTest, &DataTest::txDataToCan, this,
            [this](QString id, QString frameType, QString frameFormat, QString data){
        HidTaskPar par;
        par.m_id = id;
        par.m_frameType = frameType;
        par.m_frameFormat = frameFormat;
        par.m_data = data;
        m_pHandleHid->pushToTask(HidTask::WRITE_CAN, par);
    });

    // 通信测试定时器启动
    connect(m_pDataTest, &DataTest::txTimes, this,
            [this](int times, QString id, QString frameType, QString frameFormat, QString data){
        m_id = id;
        m_frameType = frameType;
        m_frameFormat = frameFormat;
        m_data = data;
        m_pTxTimer->start(times);
        m_isDataTest = true;
    });
    // 通信测试定时器停止
    connect(m_pDataTest, &DataTest::txCloseTimes, this, [this](){
        m_pTxTimer->stop();
        m_isDataTest = false;
    });

    // 设备连接 HID操作
    connect(m_pConectingDev, &ConnectingDevices::openHid, this, [this](QString vid, QString pid, QString serialNumber){
        HidTaskPar par = HidTaskPar();
        par.m_vid = vid;
        par.m_pid = pid;
        par.m_serialNumber = serialNumber;
        m_pHandleHid->pushToTask(HidTask::OPEN_HID, par);
    });
    connect(m_pConectingDev, &ConnectingDevices::closeHid, this, [this](){
        m_pHandleHid->pushToTask(HidTask::CLOSE_HID);
    });

    connect(m_pConectingDev, &ConnectingDevices::reqAliveHid, this, [this](){
        if(!m_isDataTest){
            m_pHandleHid->pushToTask(HidTask::IS_ALIVE_HID);
        }
    });

    // HID 读取滤波
    connect(m_pSetFilter, &SetFilter::readFilter, this, [this](){
        m_pSetPar->on_pushButton_read_can_clicked();
    });

    // 连接页面和其他页面交互HID状态
    connect(m_pConectingDev, &ConnectingDevices::hidStatus, m_pSetPar, &SetParameter::onHidStatus);
    connect(m_pConectingDev, &ConnectingDevices::hidStatus, m_pSetFilter, &SetFilter::onHidStatus);
    connect(m_pConectingDev, &ConnectingDevices::hidStatus, m_pUpgradeDev, &UpgradeDevice::onHidStatus);
    connect(m_pConectingDev, &ConnectingDevices::scanHid, this, [this](){
        m_pHandleHid->pushToTask(HidTask::SCAN_HID);
    });

    this->initHidConnection();
    this->initSerialConnection();
}
void MainWindow::initHidConnection()
{
    // HID 操作结果
    connect(m_pHandleHid, &HandleHid::isCloseHid, this, [this](){
        this->m_hardwareLabel.setText(tr("  设备版本: V***"));
        m_pConectingDev->onIsCloseHid();
        m_pConectingDev->hidStatus(false);
        m_pDevStatus->setChecked(false);
    });
    connect(m_pHandleHid, &HandleHid::isOpenHid, this, [this](bool status, QString version){
        this->m_hardwareLabel.setText(tr("  设备版本: ") + version);
        m_pDevStatus->setChecked(status);
        m_pConectingDev->onHidStatus(status);
        m_pConectingDev->onIsOpenHid();
    });
    // HID 写数据的结果返回
    connect(m_pHandleHid, &HandleHid::numCanWrite, this, [this](int num, QString id, QString frameType,
                      QString frameFormat, QString data, QString datetime){
        // TODO: 使用MVC模型插入数据
        m_pDataTest->onWriteData(num, id, frameType, frameFormat, data, datetime);
    }, Qt::QueuedConnection);

    // HID 参数设置的结果
    connect(m_pHandleHid, &HandleHid::isCanSetMode, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN工作模式设置")); });
    connect(m_pHandleHid, &HandleHid::isCanReset, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN复位")); });
    connect(m_pHandleHid, &HandleHid::isCanResetError, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN错误复位")); });
    connect(m_pHandleHid, &HandleHid::isCanSetPar, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN参数设置")); });

    connect(m_pHandleHid, &HandleHid::isFilterId, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN帧ID滤波器组设置")); });
    connect(m_pHandleHid, &HandleHid::isCanParStatus, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN参数读取")); });
    connect(m_pHandleHid, &HandleHid::isCanPar, this,
            [this](QString bauderate, QList<QStringList> filter){
        m_pSetPar->onCanBaudrate(bauderate);
        m_pSetFilter->onFilters(filter);
    });
    connect(m_pHandleHid, &HandleHid::isClearFilterId, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN滤波清除")); });

    connect(m_pHandleHid, &HandleHid::isTransitionParCan, this, [this](bool status){
        this->dumpInfoBox(status, tr("转换参数设置")); });
    connect(m_pHandleHid, &HandleHid::isSerialPar, this, [this](bool status){
        this->dumpInfoBox(status, tr("串口参数设置")); });
    connect(m_pHandleHid, &HandleHid::isParCan, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN参数设置")); });
    connect(m_pHandleHid, &HandleHid::isRMcuPar,
            this, [this](TransiteParameterUi canPar, SerialPortParameterUi serialPar){
        m_pSetPar->onIsRMcuPar(canPar, serialPar);
    });
    connect(m_pHandleHid, &HandleHid::isRSerialPar,
            this, [this](bool status, QString baudrate, QString id,
            QString frameFormat, QList<QStringList> filter){
        m_pSetPar->onIsRSerialPar(baudrate, id, frameFormat);
        // 过滤规则
        m_pSetFilter->onClearText();
        m_pSetFilter->onFilters(filter);
        this->dumpInfoBox(status, tr("串口读取MCU参数"));
    });
    connect(m_pHandleHid, &HandleHid::updateStatus, m_pUpgradeDev, &UpgradeDevice::onUpdateStatus);
    connect(m_pHandleHid, &HandleHid::updateResult, m_pUpgradeDev, &UpgradeDevice::onUpdateResult);
    connect(m_pHandleHid, &HandleHid::processRange, m_pUpgradeDev, &UpgradeDevice::onProcessRange);
    connect(m_pHandleHid, &HandleHid::processBarV, m_pUpgradeDev, &UpgradeDevice::onProcessBarV);
    connect(m_pHandleHid, &HandleHid::isScanHidDev, [this](QList<QStringList> devs){
        for (auto info: devs) { m_pConectingDev->instertScanResult(info); }
    });

    connect(m_pHandleHid, &HandleHid::isAliveStatus, this, [this](bool status){
        m_pDevStatus->setChecked(status);
        if(!status){
            this->m_hardwareLabel.setText(tr("  设备版本: V***"));
            this->m_pConectingDev->onIsCloseHid();
        }
        // 更改连接页面显示
        m_pConectingDev->onHidStatus(status);
    });
}
void MainWindow::initSerialConnection()
{
    // 串口 升级
    connect(m_pHandleSerial, &HandleSerialProtocol::updateStatus, m_pUpgradeDev, &UpgradeDevice::onUpdateStatus);
    connect(m_pHandleSerial, &HandleSerialProtocol::updateResult, m_pUpgradeDev, &UpgradeDevice::onUpdateResult);
    connect(m_pHandleSerial, &HandleSerialProtocol::processRange, m_pUpgradeDev, &UpgradeDevice::onProcessRange);
    connect(m_pHandleSerial, &HandleSerialProtocol::processBarV, m_pUpgradeDev, &UpgradeDevice::onProcessBarV);

    // 连接页面和其他页面交互串口状态
    connect(m_pConectingDev, &ConnectingDevices::serialPortStatus, m_pSetPar, &SetParameter::onSerialStatus);
    connect(m_pConectingDev, &ConnectingDevices::serialPortStatus, m_pSetFilter, &SetFilter::onSerialStatus);
    connect(m_pConectingDev, &ConnectingDevices::serialPortStatus, m_pUpgradeDev, &UpgradeDevice::onSerialStatus);
    connect(m_pHandleSerial, &HandleSerialProtocol::isInitCan, this, [this](QString version, bool status){
        if(status){
            this->m_hardwareLabel.setText(tr("  设备版本: ") + version);
            m_pDevStatus->setChecked(true);
        }else{
            this->m_hardwareLabel.setText(tr("  设备版本: V***"));
            m_pDevStatus->setChecked(false);
        }
        m_pConectingDev->onIsOpenSerial();
    });

    connect(m_pHandleSerial, &HandleSerialProtocol::isAliveCan, this, [this](bool status){
        m_pDevStatus->setChecked(status);
        if(!status){
            this->m_hardwareLabel.setText(tr("  设备版本: V***"));
            this->m_pConectingDev->onIsCloseSerial();
        }
        m_pConectingDev->onSerialStatus(status);
    });

    connect(m_pHandleSerial, &HandleSerialProtocol::isOpenCan, this, [this](){
        m_pHandleSerial->initCan();
    });
    connect(m_pHandleSerial, &HandleSerialProtocol::isTransitionParCan, this, [this](bool status){
        this->dumpInfoBox(status, tr("转换参数设置"));
    });
    connect(m_pHandleSerial, &HandleSerialProtocol::isParCan, this, [this](bool status){
        this->dumpInfoBox(status, tr("CAN参数设置"));
    });
    connect(m_pHandleSerial, &HandleSerialProtocol::isFilterCan, this, [this](bool status){
        this->dumpInfoBox(status, tr("滤波规则设置"));
    });
    connect(m_pHandleSerial, &HandleSerialProtocol::isClearFilter, this, [this](bool status){
        this->dumpInfoBox(status, tr("滤波规则清除"));
    });
    connect(m_pHandleSerial, &HandleSerialProtocol::isSerialPar, this, [this](bool status){
        this->dumpInfoBox(status, tr("串口参数设置"));
    });
    connect(m_pHandleSerial, &HandleSerialProtocol::isMcuConfig, this, [this](SerialTaskPar rst, bool re){
        this->dumpInfoBox(re, tr("读取MCU参数"));
        if(!re){return;}
        TransiteParameterUi transUi;
        SerialPortParameterUi serialUi;
        transUi.model = rst.transitionModel;
        transUi.directe = rst.transitionDirecte;
        transUi.enableSerial = rst.enableSerial;
        transUi.enableId = rst.enableID;
        transUi.offset = QString::number(rst.serialOffset);
        transUi.frameLen = QString::number(rst.serialFramLen);
        transUi.serialTimes = QString::number(rst.serialTimes);
        transUi.transiteTimes = QString::number(rst.transitionTimes);
        serialUi.baudrate = rst.bauderateSerial;
        serialUi.databit = rst.dataBit;
        serialUi.paritybit = rst.parityBit;
        serialUi.stopbit = rst.stopBit;
        serialUi.flow = rst.flowControl;
        serialUi.model = rst.serailModel;
        m_pSetPar->onIsRMcuPar(transUi, serialUi);
        m_pSetPar->onIsRSerialPar(rst.bauterateCan, rst.canID, rst.frameFormat);
        m_pSetFilter->onFilters(rst.filterData);
    });

    connect(m_pHandleSerial, &HandleSerialProtocol::readTimeout, this, [](){
        CustomWarningBox::customWarningBox(tr("通信异常"), tr("通信等待超时"));
    });
}

void MainWindow::on_buttonClickedLeftNav(QAbstractButton *btn)
{
    ui->stackedWidget->setCurrentIndex(m_pBtnGroupLeftNav->id(btn));
    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);
}

void MainWindow::addNavStackWidget(QString objectName, QString text, QWidget *widget, QIcon ico)
{
    int btnId = m_pTBtnLeftNavVector.length();
    auto pTBtn = new QToolButton(this);
    m_pTBtnLeftNavVector.append(pTBtn);
    pTBtn->setText(text);
    pTBtn->setCheckable(true);
    pTBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if(!objectName.trimmed().isEmpty()){
        pTBtn->setObjectName(objectName);
    }
    if(!ico.isNull()){
        pTBtn->setIcon(ico);
        pTBtn->setIconSize(QSize(16, 16));
        pTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }else{
        pTBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
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

void MainWindow::dumpInfoBox(bool status, QString title)
{
    QString info = status ? title + tr("成功!") : title + tr("失败!");
    if(status){
        CustomInfoBox::customInfoBox(title, info);
    }else {
        CustomWarningBox::customWarningBox(title, info);
    }
}
