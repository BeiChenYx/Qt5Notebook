#include <QFontDatabase>
#include <QDebug>
#include <QHostInfo>
#include <QFileDialog>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "customdialog.h"
#include "ui_mainwindow.h"

Q_DECLARE_METATYPE(QVector<uint16_t>);
Q_DECLARE_METATYPE(uint16_t);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pHomePage(new HomePage(this)),
    m_pDataRecordPage(new DataRecordWidget(this)),
    m_pScanDevicesPage(new ScanDevices),
    m_pStatusWidget(new StatusWidget(this)),
    m_pHandleModbus(new HandleModbus),
    m_pAutoUnLoadTimer(new QTimer(this))
//    m_pAlarmHandle(new AlarmTaskTrhead),
//    m_pAlarmThread(new QThread)
{
    ui->setupUi(this);
    this->setMinimumHeight(650);

    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    this->setWindowIcon(QIcon(":/imges/SF.ico"));

    qRegisterMetaType<QVector<uint16_t> >("vector_uint16_t");
    qRegisterMetaType<uint16_t>("uint16_t");
    this->initUi();
    this->initConnection();

    // important to watch mouse move from all child widgets
    QApplication::instance()->installEventFilter(this);
    ui->toolButton_home->clicked();
    ui->stackedWidget_main->setCurrentIndex(0);
    this->resize(730, 660);
    QDesktopWidget *deskdop = QApplication::desktop();
    auto x = deskdop->width() / 2 - this->width() / 2;
    auto y = deskdop->height() / 2 - this->height() / 2;
    move(x / 2, y / 2);

    // 延时一秒后扫描网络内设备
    QTimer::singleShot(1000, this, [this](){
        this->m_pScanDevicesPage->on_pushButton_scan_clicked();
    });
    this->setWindowTitle(tr("畜禽舍环控系统"));
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_pHandleModbus->isOpen()){
        m_pHandleModbus->closeModbus();
        this->m_pStatusWidget->setInfo("***", "***", "***");
    }
    if(m_pUpdateThread != nullptr){
        m_pUpdateThread->quit();
        m_pUpdateThread->wait(100);
        delete m_pUpdateThread;
        m_pUpdateThread = nullptr;
    }
//    if(m_pAlarmThread != nullptr){
//        m_pAlarmThread->quit();
//        m_pAlarmThread->wait(100);
//        delete m_pAlarmThread;
//        m_pAlarmThread = nullptr;
//    }
}
void MainWindow::initUi(){
    auto homePage = new QGridLayout;
    homePage->addWidget(m_pHomePage);
    ui->barton_page->setLayout(homePage);
    auto dataRecordPage = new QGridLayout;
    dataRecordPage->addWidget(m_pDataRecordPage);
    ui->data_log_page->setLayout(dataRecordPage);
    auto scanDevicePage = new QGridLayout;
    scanDevicePage->addWidget(m_pScanDevicesPage);
    ui->scan_page->setLayout(scanDevicePage);

    // 添加字体文件, 创建字体
    QFont font;
    int fontId = QFontDatabase::addApplicationFont(":/images/fontawesome-webfont.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    font.setFamily(fontFamilies.at(0));
    font.setPointSize(10);
    // 标题栏四个按键
    ui->toolButton_close_window->setFont(font);
    ui->toolButton_max_reset->setFont(font);
    ui->toolButton_min->setFont(font);
    ui->toolButton_full->setFont(font);
    ui->toolButton_close_window->setText(QChar(0xf00d));
    ui->toolButton_max_reset->setText(QChar(0xf065));
    ui->toolButton_min->setText(QChar(0xf068));
    ui->toolButton_full->setText(QChar(0xf0b2));
    ui->toolButton_full->hide();

    // 左导航栏
    this->setHomeSetSwitch(true);
    ui->buttonGroup_nav->setId(ui->toolButton_home, 0);
    ui->buttonGroup_nav->setId(ui->toolButton_data_log, 1);
    ui->buttonGroup_nav->setId(ui->toolButton_alarm_log, 2);
    ui->buttonGroup_nav->setId(ui->toolButton_set_1, 3);
    ui->buttonGroup_nav->setId(ui->toolButton_set_2, 4);
    ui->buttonGroup_nav->setId(ui->toolButton_set_3, 5);
    ui->buttonGroup_nav->setId(ui->toolButton_set_4, 6);
    ui->buttonGroup_nav->setId(ui->toolButton_set_5, 7);
    ui->buttonGroup_nav->setId(ui->toolButton_set_6, 8);
    ui->buttonGroup_nav->setId(ui->toolButton_user, 9);
    ui->buttonGroup_nav->setId(ui->toolButton_device_update, 10);
    ui->buttonGroup_nav->setId(ui->toolButton_scan_device, 11);
    connect(ui->buttonGroup_nav, SIGNAL(buttonClicked(QAbstractButton *)),
                this, SLOT(onButtonGroupClicked(QAbstractButton *)));
    ui->toolButton_home->setChecked(true);

    // 主页导航栏设备列表
    QList<QString> localIPList;
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address,info.addresses()){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            localIPList.append(address.toString());
        }
    }
    // Tip: Tumbler控件要求最少3个值
    QString localIp = localIPList.isEmpty() ? " " : localIPList.at(0);
    QStringList devices{"", "127.0.0.1", localIp, };
    ui->tumbler_device_ip->setListValue(devices);
    ui->tumbler_device_ip->setHorizontal(false);
    ui->tumbler_device_ip->setBackground(QColor(0xdd, 0xed, 0xfa));
    ui->tumbler_device_ip->setForeground(QColor(0x0, 0x0, 0x0, 100));
    ui->tumbler_device_ip->setLineColor(QColor(0xa0, 0xa0, 0xa0));
    ui->tumbler_device_ip->setTextColor(QColor(0x30, 0x96, 0xf8));
    ui->tumbler_device_ip->setCurrentValue("127.0.0.1");

    // 升级窗口的进度导航
    QStringList updateProgressNav;
    updateProgressNav << tr("选择设备") << tr("选择文件") << tr("开始升级") << tr("升级结束");
    ui->widget_progress_update->setTopInfo(updateProgressNav);
    ui->widget_progress_update->setMaxStep(updateProgressNav.length());
    ui->widget_progress_update->setCurrentStep(0);
    ui->comboBox_device_ip->addItems({"127.0.0.1", localIp});
    ui->progressBar_update->setVisible(false);

    // 状态栏
    ui->statusBar->addPermanentWidget(m_pStatusWidget);
//    m_pAlarmHandle->moveToThread(m_pAlarmThread);
//    connect(this, &MainWindow::setAlarmHandle, m_pAlarmHandle, &AlarmTaskTrhead::setHandleModbus);

//    m_pAlarmThread->start();

    // 报警记录
    ui->tableWidget_data_record->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_data_record->setColumnCount(6);
    ui->tableWidget_data_record->setHorizontalHeaderLabels({tr("序号"), tr("MAC"), tr("IP"),
                                                            tr("版本"), tr("日期"), tr("事件信息")});
    ui->tableWidget_data_record->setColumnWidth(0, 50);
    ui->tableWidget_data_record->setColumnWidth(1, 160);
    ui->tableWidget_data_record->setColumnWidth(2, 160);
    ui->tableWidget_data_record->setColumnWidth(3, 50);
    ui->tableWidget_data_record->setColumnWidth(4, 160);
    // 用户管理
    QRegExp regx("[1-9][0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, this);
    ui->lineEdit_pw_1->setValidator(validator);
    ui->lineEdit_pw_2->setValidator(validator);
    ui->lineEdit_pw_3->setValidator(validator);
    this->setBtnStatus(false);
}
void MainWindow::onButtonGroupClicked(QAbstractButton *btn)
{
    ui->stackedWidget_main->setCurrentIndex(ui->buttonGroup_nav->id(btn));
    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);

    auto updatePage = [this](QToolButton *obj){
        ui->widget_set_1->updatePageData(obj == ui->toolButton_set_1);
        ui->widget_set_2->updatePageData(obj == ui->toolButton_set_2);
        ui->widget_set_3->updatePageData(obj == ui->toolButton_set_3);
        ui->widget_set_4->updatePageData(obj == ui->toolButton_set_4);
        ui->widget_set_5->updatePageData(obj == ui->toolButton_set_5);
        ui->widget_set_6->updatePageData(obj == ui->toolButton_set_6);
        m_pDataRecordPage->updatePageData(obj == ui->toolButton_data_log);
        m_pHomePage->updatePageData(obj == ui->toolButton_home);
        if(obj == ui->toolButton_alarm_log){this->getAlarmCount();}
    };
    if(m_pHandleModbus->isOpen()){
        updatePage(obj);
    }
}

void MainWindow::on_toolButton_close_window_clicked() { this->close(); }

void MainWindow::on_toolButton_max_reset_clicked()
{
    if(this->windowState().testFlag(Qt::WindowNoState)){
        this->showMaximized();
        ui->toolButton_max_reset->setText(QChar(0xf066));
        ui->toolButton_full->show();
    }else if(this->windowState().testFlag(Qt::WindowMaximized)){
        this->showNormal();
        ui->toolButton_max_reset->setText(QChar(0xf065));
        ui->toolButton_full->hide();
    }
    this->update();
}

void MainWindow::on_toolButton_min_clicked() { this->showMinimized(); }

void MainWindow::on_toolButton_full_clicked()
{
    if(!this->windowState().testFlag(Qt::WindowFullScreen)){
        this->showFullScreen();
        ui->toolButton_full->setText(QChar(0xf05b));
        ui->toolButton_min->hide();
        ui->toolButton_close_window->hide();
        ui->toolButton_max_reset->hide();
    }else{
        this->showMaximized();
        ui->toolButton_full->setText(QChar(0xf0b2));
        ui->toolButton_min->show();
        ui->toolButton_close_window->show();
        ui->toolButton_max_reset->show();
    }
    this->update();
}
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
void MainWindow::mouseDoubleClickEvent(QMouseEvent *) { this->on_toolButton_max_reset_clicked(); }
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
    if(event->type() == QEvent::KeyPress || event->type() == QEvent::MouseMove
            || event->type() == QEvent::MouseButtonPress ||event->type() == QEvent::Wheel){
        g_LogOut = true;
    }
    // TODO: 事件处理存在问题，导致子控件无法接受到鼠标移动，因此
    // 图表记录先用这种方式临时处理, 自定义标题栏的事件处理应该独立出去，
    // 不应该影响其他控件的事件处理
    if(ui->toolButton_data_log->isChecked()){
        if(event->type() == QEvent::MouseMove){
            QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
            m_pDataRecordPage->mousePoint(pMouse->globalPos());
        }
    }
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

MainWindow::MouseBorderPos MainWindow::isBorderPos(const QPoint &pos, bool isPressed)
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


void MainWindow::setHomeSetSwitch(bool status)
{
    ui->toolButton_home->setVisible(status);
    ui->toolButton_data_log->setVisible(status);
    ui->toolButton_alarm_log->setVisible(status);
    ui->toolButton_par_set->setVisible(status);
    ui->toolButton_goto_home->setVisible(!status);
    ui->toolButton_set_1->setVisible(!status);
    ui->toolButton_set_2->setVisible(!status);
    ui->toolButton_set_3->setVisible(!status);
    ui->toolButton_set_4->setVisible(!status);
    ui->toolButton_set_5->setVisible(!status);
    ui->toolButton_set_6->setVisible(!status);
    this->update();
}

void MainWindow::initConnection()
{
    connect(ui->tumbler_device_ip, &Tumbler::currentIndexChanged, this, &MainWindow::onTumblerIndexChanged);
    connect(ui->tumbler_device_ip, &Tumbler::currentValueChanged, this, &MainWindow::onTumblerValueChanged);
    connect(m_pAutoUnLoadTimer, &QTimer::timeout, this, [this](){
        if(g_LogOut){
            g_LogOut = false;
        }else{
            this->on_pushButton_unload_clicked();
        }
    });
    connect(m_pScanDevicesPage, &ScanDevices::openDevice, this, &MainWindow::openDevice);
    connect(m_pScanDevicesPage, &ScanDevices::closeDevice, this, &MainWindow::closeDevice);
    connect(m_pScanDevicesPage, &ScanDevices::deviceIp, this, [this](QString ip){
        ui->tumbler_device_ip->addValue(ip);
        ui->comboBox_device_ip->addItem(ip);
    });
}

void MainWindow::on_toolButton_par_set_clicked()
{
    this->setHomeSetSwitch(false);
    ui->toolButton_set_1->click();
}

void MainWindow::on_toolButton_goto_home_clicked()
{
    this->setHomeSetSwitch(true);
    ui->toolButton_home->click();
}

void MainWindow::onTumblerValueChanged(QString value, QString oldValue)
{
    if(value == oldValue){return;}
}

void MainWindow::onTumblerIndexChanged(int index, int oldIndex)
{
    if(index == oldIndex){return;}
}

void MainWindow::on_toolButton_switch_device_clicked()
{
    // 切换连接设备
    QString ip = ui->tumbler_device_ip->getCurrentValue();
    this->closeDevice();
    this->openDevice(ip);
}

void MainWindow::openDevice(QString ip)
{
    if(ip.trimmed().isEmpty()){return;}
    if(m_pHandleModbus->openModbus(ip)){
        ui->widget_set_1->setHandleModbus(m_pHandleModbus);
        ui->widget_set_2->setHandleModbus(m_pHandleModbus);
        ui->widget_set_3->setHandleModbus(m_pHandleModbus);
        ui->widget_set_4->setHandleModbus(m_pHandleModbus);
        ui->widget_set_5->setHandleModbus(m_pHandleModbus);
        ui->widget_set_6->setHandleModbus(m_pHandleModbus);
        m_pDataRecordPage->setHandleModbus(m_pHandleModbus);
        m_pHomePage->setHandleModbus(m_pHandleModbus);
        this->getHardWareMac();
        m_pHomePage->updatePageData(true);
        m_pAutoUnLoadTimer->start(AUTO_UNLOAD);
        m_pScanDevicesPage->onOpenResult();
        // Tip: addValue 应该在最前面，保证Tumbler控件没有这个IP的时候可以先添加再设置
        ui->tumbler_device_ip->addValue(ip);
        ui->tumbler_device_ip->setSelectEdValue(ip);
        ui->tumbler_device_ip->setCurrentValue(ip);
        ui->comboBox_device_ip->addItem(ip);
    }
}
void MainWindow::closeDevice()
{
    if(m_pHandleModbus->isOpen()){
        m_pHandleModbus->closeModbus();
        this->m_pStatusWidget->setInfo("***", "***", "***");
        ui->tumbler_device_ip->setSelectEdValue("");
        m_pScanDevicesPage->onCloseResult();
    }
}

void MainWindow::getHardWareMac()
{
    uint16_t buf[4]{0};
    if(m_pHandleModbus->readInputRegisters(0x002B, 4, buf) > 0){
        QString hardware = QString("%1").arg(buf[0] / 100.00);
        int mac[6]{0};
        mac[0] = (buf[1] >> 8) & 0x00FF;
        mac[1] = buf[1] & 0x00FF;
        mac[2] = (buf[2] >> 8) & 0x00FF;
        mac[3] = buf[2] & 0x00FF;
        mac[4] = (buf[3] >> 8) & 0x00FF;
        mac[5] = buf[3] & 0x00FF;
        QString macStr = QString::asprintf("%02X-%02X-%02X-%02X-%02X-%02X",
                                           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        this->m_pStatusWidget->setInfo(hardware, macStr, m_pHandleModbus->getIp());
        this->m_pDataRecordPage->setDevInfo(macStr, m_pHandleModbus->getIp(), hardware);
        this->m_mac = macStr;
        this->m_ip = m_pHandleModbus->getIp();
        this->m_hardware = hardware;
    }else{
        CustomWarrningBox::customWarrningBox(tr("设备操作"), tr("设备信息获取错误!"));
    }
}

//void MainWindow::on_toolButton_pre_day_clicked()
//{
//    auto old = ui->dateEdit_date_page->date();
//    auto now = old.addDays(-1);
//    selectAlarm(now);
//}

//void MainWindow::on_toolButton_next_day_clicked()
//{
//    auto old = ui->dateEdit_date_page->date();
//    auto now = old.addDays(1);
//    selectAlarm(now);
    // 先做后一页
//}
void MainWindow::getAlarmCount()
{
    if(m_pHandleModbus->isOpen()){
        uint16_t alarmCount = 0;
        if(m_pHandleModbus->readHoldingRegisters(0x18D0, 1, &alarmCount) > 0){
            int n = alarmCount / ALARM_PAGE_COUNT;
            int m = alarmCount % ALARM_PAGE_COUNT;
            m_n = n;
            m_m = m;
            ui->label_a_s->setText(tr("%1").arg(n + (m>0 ? 1 : 0)));
        }
        int maxIndex = m_n >= 1 ? ALARM_PAGE_COUNT : m_m;
        this->getAlarmLog(0, maxIndex);
    }
}
#if 0 // 目前不作日期查询 -- yangxing 20200330
void MainWindow::selectAlarm(QDate date)
{
    QVector<AlarmInfo> result;
    AlarmReadDB::selectAlarm(date, this->m_mac, result);
    for (int i=0; i<result.length(); ++i) {
        int nOldRowCount = ui->tableWidget_data_record->rowCount();
        ui->tableWidget_data_record->insertRow(nOldRowCount);
        ui->tableWidget_data_record->setItem(nOldRowCount, 0, new QTableWidgetItem(QString::number(nOldRowCount)));
        ui->tableWidget_data_record->setItem(nOldRowCount, 1, new QTableWidgetItem(result[i].mac));
        ui->tableWidget_data_record->setItem(nOldRowCount, 2, new QTableWidgetItem(result[i].ip));
        ui->tableWidget_data_record->setItem(nOldRowCount, 3, new QTableWidgetItem(result[i].hardware));
        ui->tableWidget_data_record->setItem(nOldRowCount, 4, new QTableWidgetItem(result[i].datetime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidget_data_record->setItem(nOldRowCount, 5, new QTableWidgetItem(AlarmReadDB::getAlarmInfo(result[i].info)));
    }
}
#endif

void MainWindow::on_pushButton_load_clicked()
{
    if(m_pHandleModbus->isOpen()){
        uint16_t currentUser = static_cast<uint16_t>(ui->comboBox_user->currentIndex());
        uint pw = ui->lineEdit_pw_1->text().toUInt();
        uint16_t buf[3]{0};
        buf[0] = currentUser;
        buf[1] = pw >> 16;
        buf[2] = pw & 0xFFFF;
        if(m_pHandleModbus->writeMultipleRegisters(0x00FF, 3, buf) <= 0){
            return;
        }
        QThread::msleep(100);
        uint16_t reBuf = 0;
        if(m_pHandleModbus->readInputRegisters(0x0057, 1, &reBuf) > 0){
            if(reBuf > 3 || reBuf == 0){
                CustomErrorBox::customErrorBox(tr("登录"), tr("密码错误"));
                return;
            }
            for (int i=1; i<4; ++i) {
                auto comboBox = this->findChild<QComboBox*>(QString("comboBox_c_u%1").arg(i));
                comboBox->setCurrentIndex(reBuf);
            }
            ui->label_user_status->setText(ui->comboBox_c_u1->currentText());
            this->setBtnStatus(true);
            g_indexUser  = reBuf;
        }
    }
}

void MainWindow::on_pushButton_unload_clicked()
{
    if(m_pHandleModbus->isOpen()){
        uint16_t buf[1]{0x02};
        if(m_pHandleModbus->writeMultipleRegisters(0x1CB8, 1, buf) <= 0){
            return;
        }
        QThread::msleep(100);
        uint16_t reBuf = 0;
        if(m_pHandleModbus->readInputRegisters(0x0057, 1, &reBuf) > 0){
            if(reBuf > 3){
                CustomErrorBox::customErrorBox(tr("注销"), tr("注销出现错误"));
                return;
            }
            if(reBuf == 0){
                for (int i=1; i<4; ++i) {
                    auto comboBox = this->findChild<QComboBox*>(QString("comboBox_c_u%1").arg(i));
                    comboBox->setCurrentIndex(reBuf);
                }
                ui->label_user_status->setText(ui->comboBox_c_u1->currentText());
                this->setBtnStatus(false);
                g_indexUser  = reBuf;
            }
        }
    }
}

void MainWindow::on_pushButton_auto_unload_clicked()
{
    m_pAutoUnLoadTimer->stop();
    // 定时器为自动注销时间的一半
    m_pAutoUnLoadTimer->start(ui->spinBox_auto_unload->value() * 30000);
}

void MainWindow::on_pushButton_modify_clicked()
{
    if(m_pHandleModbus->isOpen()){
        uint newPw = ui->lineEdit_pw_2->text().toUInt();
        uint qPw = ui->lineEdit_pw_3->text().toUInt();
        if(newPw != qPw){
            CustomErrorBox::customErrorBox(tr("修改密码"), tr("新密码和确认密码不一致"));
            return;
        }
        uint16_t buf[4]{0};
        buf[0] = newPw >> 16;
        buf[1] = newPw & 0xFFFF;
        buf[2] = qPw >> 16;
        buf[3] = qPw & 0xFFFF;
        if(m_pHandleModbus->writeMultipleRegisters(0x0103, 4, buf) <= 0){
            return;
        }
        QThread::msleep(100);
        uint16_t reBuf = 0;
        if(m_pHandleModbus->readHoldingRegisters(0x1CB9, 1, &reBuf) > 0){
            if(reBuf == 1){
                CustomInfoBox::customInfoBox(tr("密码修改"), tr("修改成功"));
            }else{
                CustomErrorBox::customErrorBox(tr("修改密码"), tr("修改失败,请更改后重试"));
            }
        }
    }
}

void MainWindow::setBtnStatus(bool flag)
{
    ui->pushButton_load->setEnabled(!flag);
    ui->pushButton_unload->setEnabled(flag);
    ui->pushButton_auto_unload->setEnabled(flag);
    ui->pushButton_modify->setEnabled(flag);
    for (int i=1; i<4; ++i) {
        auto lineEidt = this->findChild<QLineEdit*>(QString("lineEdit_pw_%1").arg(i));
        lineEidt->clear();
    }
}

void MainWindow::setUpdateBtnStatus(bool status)
{
    ui->pushButton_5->setEnabled(status);
    ui->pushButton_update->setEnabled(status);
    ui->comboBox_device_ip->setEnabled(status);
    ui->lineEdit_bin->setEnabled(status);
    ui->progressBar_update->setVisible(!status);
}
void MainWindow::on_pushButton_5_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("升级文件"), ".", tr("*.bin"));
    ui->lineEdit_bin->setText(fileName);
    ui->widget_progress_update->setCurrentStep(2);
}

void MainWindow::on_pushButton_update_clicked()
{
    closeDevice();
    ui->textBrowser_update_info->clear();
    ui->widget_progress_update->setCurrentStep(3);
    auto ip = ui->comboBox_device_ip->currentText();
    auto fileName = ui->lineEdit_bin->text().trimmed();
    m_pUpdateHandle = std::make_shared<UpdateDevice>();
    if(m_pUpdateThread != nullptr){
        m_pUpdateThread->quit();
        m_pUpdateThread->wait(100);
        delete m_pUpdateThread;
        m_pUpdateThread = nullptr;
    }
    m_pUpdateThread = new QThread;
    m_pUpdateHandle->setInfo(ip, fileName);
    m_pUpdateHandle->moveToThread(m_pUpdateThread);
    connect(this, &MainWindow::updateDevice, m_pUpdateHandle.get(), &UpdateDevice::onUpdateDevice);
    connect(m_pUpdateHandle.get(), &UpdateDevice::isConnected, this, [this](bool status){
        this->setUpdateBtnStatus(!status);
    });
    connect(m_pUpdateHandle.get(), &UpdateDevice::isDisconnected, this, [this](){
        this->ui->textBrowser_update_info->moveCursor(QTextCursor::End);
        this->ui->textBrowser_update_info->insertPlainText(tr("连接断开"));
        this->setUpdateBtnStatus(true);
    });
    connect(m_pUpdateHandle.get(), &UpdateDevice::progressBarRange, this, [this](int min, int max){
        this->ui->progressBar_update->setRange(min, max);
    });
    connect(m_pUpdateHandle.get(), &UpdateDevice::progressBarValue, this, [this](int value){
        this->ui->progressBar_update->setValue(value);
    });
    connect(m_pUpdateHandle.get(), &UpdateDevice::progressBarText, this, [this](QString msg){
        this->ui->textBrowser_update_info->moveCursor(QTextCursor::End);
        this->ui->textBrowser_update_info->insertPlainText(msg + "\n");
    });
    m_pUpdateThread->start();
    emit updateDevice();
}

void MainWindow::getAlarmLog(int minIndex, int maxIndex)
{
    ui->tableWidget_data_record->clearContents();
    ui->tableWidget_data_record->setRowCount(0);
    if(m_pHandleModbus->isOpen()){
        for (int i=minIndex; i<maxIndex; ++i) {
            uint16_t bufReq[2]{0x01};
            bufReq[1] = static_cast<uint16_t>(i);
            if(m_pHandleModbus->writeMultipleRegisters(0x18D1, 2, bufReq) > 0){
                QThread::msleep(30);
                uint16_t buf18D1[9]{0};
                if(m_pHandleModbus->readHoldingRegisters(0x18D1, 9, buf18D1) > 0){
                    QDateTime datetime(QDate(buf18D1[2], buf18D1[3], buf18D1[4]), QTime(buf18D1[5], buf18D1[6], buf18D1[7]));
                    int no = i;
                    QString info = AlarmReadDB::getAlarmInfo(buf18D1[8]);

                    int nOldRowCount = ui->tableWidget_data_record->rowCount();
                    ui->tableWidget_data_record->insertRow(nOldRowCount);
                    auto item0 = new QTableWidgetItem(QString::number(no + 1));
                    item0->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget_data_record->setItem(nOldRowCount, 0, item0);
                    auto item1 = new QTableWidgetItem(m_mac);
                    item1->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget_data_record->setItem(nOldRowCount, 1, item1);
                    auto item2 = new QTableWidgetItem(m_ip);
                    item2->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget_data_record->setItem(nOldRowCount, 2, item2);
                    auto item3 = new QTableWidgetItem(m_hardware);
                    item3->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget_data_record->setItem(nOldRowCount, 3, item3);
                    auto item4 = new QTableWidgetItem(datetime.toString("yyyy-MM-dd hh:mm:ss"));
                    item4->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget_data_record->setItem(nOldRowCount, 4, item4);
                    auto item5 = new QTableWidgetItem(info);
                    item5->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget_data_record->setItem(nOldRowCount, 5, item5);
                }
            }
        }
    }
}

void MainWindow::on_pushButton_pre_day_clicked()
{
    // 先做前一页
    int maxPage = ui->label_a_s->text().toInt();
    int currentPage = ui->label_a_c->text().toInt();
    if(currentPage <= 1){return;}
    currentPage -= 1;
    int minIndex = (currentPage - 1) * ALARM_PAGE_COUNT;
    int maxIndex = minIndex + (currentPage == maxPage ? m_m : ALARM_PAGE_COUNT);
    this->getAlarmLog(minIndex, maxIndex);
    ui->label_a_c->setNum(currentPage);
}

void MainWindow::on_pushButton_next_day_clicked()
{
    int maxPage = ui->label_a_s->text().toInt();
    int currentPage = ui->label_a_c->text().toInt();
    if(currentPage >= maxPage){return;}
    currentPage += 1;
    int minIndex = (currentPage - 1) * ALARM_PAGE_COUNT;
    int maxIndex = minIndex + (currentPage == maxPage ? m_m : ALARM_PAGE_COUNT);
    this->getAlarmLog(minIndex, maxIndex);
    ui->label_a_c->setNum(currentPage);
}
