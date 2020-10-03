#include <QDebug>
#include <QGraphicsLayout>
#include "datarecordwidget.h"
#include "ui_datarecordwidget.h"


ToolTipWidget::ToolTipWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("ToolTipWidget{background: rgba(255, 255, 255, 0.5);\
                        border: 1px solid #dfe6eb;} QLabel{color: black;}" );
    auto playout = new QGridLayout;
    playout->setContentsMargins(10, 10, 10, 10);
    auto labelDate = new QLabel(tr("时间:"), this);
    m_pDate = new QLabel("***", this);

    auto labelAimT = new QLabel(tr("目标温度:"), this);
    m_pAimT = new QLabel("***", this);

    auto labelInT = new QLabel(tr("室内温度:"), this);
    m_pInT = new QLabel("***", this);

    auto labelOutT = new QLabel(tr("室外温度:"), this);
    m_pOutT = new QLabel("***", this);

    auto labelDuctT = new QLabel(tr("风管温度:"), this);
    m_pDuctT = new QLabel("***", this);

    auto labelHumidity = new QLabel(tr("室内湿度:"), this);
    m_pHumidity = new QLabel("***", this);

    auto labelInPa = new QLabel(tr("室内压力:"), this);
    m_pInPa= new QLabel("***", this);

    playout->addWidget(labelDate, 0, 0);
    playout->addWidget(m_pDate, 0, 1);

    playout->addWidget(labelAimT, 1, 0);
    playout->addWidget(m_pAimT, 1, 1);

    playout->addWidget(labelInT, 2, 0);
    playout->addWidget(m_pInT, 2, 1);

    playout->addWidget(labelOutT, 3, 0);
    playout->addWidget(m_pOutT, 3, 1);

    playout->addWidget(labelDuctT, 4, 0);
    playout->addWidget(m_pDuctT, 4, 1);

    playout->addWidget(labelHumidity, 5, 0);
    playout->addWidget(m_pHumidity, 5, 1);

    playout->addWidget(labelInPa, 6, 0);
    playout->addWidget(m_pInPa, 6, 1);
    this->setLayout(playout);
}

void ToolTipWidget::updateUi(const SeriesItem &item)
{
    QDateTime date = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(std::get<0>(item)));
    m_pDate->setText(date.toString("hh:mm:ss"));
    m_pAimT->setText(QString::number(std::get<1>(item)));
    m_pInT->setText(QString::number(std::get<2>(item)));
    m_pOutT->setText(QString::number(std::get<3>(item)));
    m_pDuctT->setText(QString::number(std::get<4>(item)));
    m_pHumidity->setText(QString::number(std::get<5>(item)));
    m_pInPa->setText(QString::number(std::get<6>(item)));
}

void RecordReadModbus::onRequest(const int dIndex, const int mIndex)
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return;}
    uint16_t buf[3]{0x01};
    buf[1] = static_cast<uint16_t>(dIndex);
    buf[2] = static_cast<uint16_t>(mIndex);
    handle->writeMultipleRegisters(0x14EC, 3, buf);
}

bool RecordReadModbus::onResponse(QVector<uint16_t> &buf)
{
    auto handle = m_pHandbleModbus.lock();
    if(!handle){return false;}
    buf.resize(24);
    if(handle->readHoldingRegisters(0x14EC, 24, buf.data()) > 0){
        if(buf[0] == 0x02){
            return  true;
        }
    }
    return false;
}

void RecordReadModbus::onMulRequest(QDate date, int page, QString macAddr, QString ipAddr, QString hardware)
{
    // 查询数据库，获取需要访问设备的日期时间索引
    auto mIndexVector = isSelectData(date, macAddr);
    // 根据分页选取mIndexVector的子集
    std::vector<qint64> pageMIndexVector;
    for (int i=0; i<ONE_PAGE_POINTS; ++i) {
        pageMIndexVector.push_back(i + ONE_PAGE_POINTS*(page-1));
    }
    std::vector<qint64> dateReslutVector;
    std::set_intersection(pageMIndexVector.begin(), pageMIndexVector.end(),
                        mIndexVector.begin(), mIndexVector.end(), std::back_inserter(dateReslutVector));
    if(!dateReslutVector.empty()){
        QDateTime now(QDate::currentDate(), QTime(0, 0, 0));
        auto nowSecond = now.toSecsSinceEpoch();
        auto dateSecond = QDateTime(date).toSecsSinceEpoch();
        if(dateSecond > nowSecond){return;}
        // 计算 onRequest需要的 dIndex 和 mIdex
        int dIndex = nowSecond == dateSecond ? 0 : static_cast<int>((nowSecond - dateSecond)/(3600 * 24));
        QList<QVector<uint16_t> > recordList;
        auto indexs = QVector<qint64>::fromStdVector(dateReslutVector);
        int count= 0;
        while (!indexs.isEmpty()) {
            auto mindex = indexs.first();
            this->onRequest(dIndex, static_cast<int>(mindex));
            QThread::msleep(50);
            QVector<uint16_t> buf;
            if(this->onResponse(buf)){
                indexs.removeFirst();
                recordList.append(buf);
                emit progressValue(++count);
                continue;
            }
        }
        // 插入数据库
        insertData(recordList, macAddr, ipAddr, hardware);
    }
    //  查询指定日期的的数据
    selectData(date, macAddr);
    emit progressValue(ONE_PAGE_POINTS);
}

void RecordReadModbus::onOneDayRequest(QDate date, QString macAddr, QString ipAddr, QString hardware)
{
    auto mIndexVector = isSelectData(date, macAddr);
    if(!mIndexVector.isEmpty()){
        QDateTime now(QDate::currentDate(), QTime(0, 0, 0));
        auto nowSecond = now.toSecsSinceEpoch();
        auto dateSecond = QDateTime(date).toSecsSinceEpoch();
        if(dateSecond > nowSecond){return;}
        // 计算 onRequest需要的 dIndex 和 mIdex
        int dIndex = nowSecond == dateSecond ? 0 : static_cast<int>((nowSecond - dateSecond)/(3600 * 24));
        QList<QVector<uint16_t> > recordList;
        while (!mIndexVector.isEmpty()) {
            auto mindex = mIndexVector.first();
            this->onRequest(dIndex, static_cast<int>(mindex));
            QThread::msleep(50);
            QVector<uint16_t> buf;
            if(this->onResponse(buf)){
                mIndexVector.removeFirst();
                recordList.append(buf);
                continue;
            }
        }
        // 插入数据库
        insertData(recordList, macAddr, ipAddr, hardware);
    }
    //  查询指定日期的的数据
    selectData(date, macAddr);
}
bool RecordReadModbus::insertData(QList<QVector<uint16_t> > &data, QString mac_addr, QString ip_addr, QString hardware)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECT_NAME);
    db.setDatabaseName(DB_NAME);
    db.open();
    QString sql = QString("INSERT INTO %1 (mac_addr, ip_addr, hardware, date, day_age, aim_t, in_t, in_h, in_pa, out_t, \
                          duct_t, t6, fp, hot,  recovery_q, recovery_e, recovery_n, back_up, level)  VALUES ").arg(DATA_TABLE_NAME);
    for (auto buf : data) {
        if(buf.length() < 24){continue;}
        QDateTime date;
        date.setDate(QDate(buf[3], buf[4], buf[5]));
        date.setTime(QTime(buf[6], buf[7], buf[8]));
        QString tmp = QString("(\"%1\", \"%2\", \"%3\", %4, %5, %6, %7, %8, %9, %10, %11, %12, \
                       %13, %14, %15, %16, %17, \"%18\", %19)")
                .arg(mac_addr).arg(ip_addr).arg(hardware)
                .arg(date.toSecsSinceEpoch()).arg(buf[9]).arg(buf[10] / 100.00)                                             // 时间戳, 日龄, 目标温度
                .arg(buf[11] / 100.00).arg(buf[12] / 10.0).arg(buf[13] / 10.0).arg(buf[14] / 100)                           // 室内温度, 室内湿度, 室内压力, 室外温度
                .arg(buf[15] / 100.00).arg(buf[16] / 100.00).arg(buf[17]).arg(buf[18])                                      // 风管温度, T6, 旁通阀, 热水阀
                .arg(buf[19]).arg(buf[20]).arg(buf[21]).arg(QString::asprintf("%.2f", buf[22] / 100.00)).arg(buf[23]);      // 回收Q, 回收E, 回收N, 备用(T7),级别
        QSqlQuery query(db);
        if(!query.exec(sql + tmp)){
            qDebug() << query.lastError().text();
            continue;
        }
    }
    db.close();
    QSqlDatabase::removeDatabase(CONNECT_NAME);
    return true;
}
bool RecordReadModbus::selectData(QDate date, QString mac_addr)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECT_NAME);
    db.setDatabaseName(DB_NAME);
    db.open();
    QDateTime tmpS(date, QTime(0, 0, 0)), tmpE(date, QTime(23, 59, 59));
    QString sql = QString("SELECT * FROM %1 WHERE mac_addr='%2' AND date>=%3 AND date<=%4;")
            .arg(DATA_TABLE_NAME ).arg(mac_addr).arg(tmpS.toSecsSinceEpoch()).arg(tmpE.toSecsSinceEpoch());
    QSqlQuery query(db);
    if(!query.exec(sql)){
        return false;
    }
    while (query.next()) {
        QVector<QVariant> result;
        for (int i=0; i<19; ++i) {
            result.append(query.value(i));
        }
        emit recordResult(result);
    }
    db.close();
    QSqlDatabase::removeDatabase(CONNECT_NAME);
    return true;
}
QVector<qint64> RecordReadModbus::isSelectData(QDate date, QString mac_addr)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECT_NAME);
    db.setDatabaseName(DB_NAME);
    db.open();
    QDateTime tmpS(date, QTime(0, 0, 0)), tmpE(date, QTime(23, 59, 59));
    QString sql = QString("SELECT date FROM %1 WHERE mac_addr='%2' AND date>=%3 AND date<=%4;")
            .arg(DATA_TABLE_NAME ).arg(mac_addr).arg(tmpS.toSecsSinceEpoch()).arg(tmpE.toSecsSinceEpoch());
    QSqlQuery query(db);
    if(!query.exec(sql)){
        QVector<qint64> result;
        return result;
    }
    std::vector<qint64> dateVector;
    while (query.next()) {
        dateVector.push_back(static_cast<qint64>(query.value(0).toReal()));
    }
    db.close();
    QSqlDatabase::removeDatabase(CONNECT_NAME);
    QDateTime tmp(date, QTime(0, 0, 0));
    std::vector<qint64> dayDateVector;
    // 生成需要的日期集合
    for (int i=0; i<288; ++i) {
        // 五分钟间隔
        dayDateVector.push_back(tmp.toSecsSinceEpoch() + 300*i);
    }
    // 计算差集
    std::vector<qint64> dateReslutVector;
    std::set_difference(dayDateVector.begin(), dayDateVector.end(),
                        dateVector.begin(), dateVector.end(), std::back_inserter(dateReslutVector));
    // 获取差集中的元素所在的索引，作为历史记录的点偏移
    QVector<qint64> mIndexVector;
    for (auto re : dateReslutVector) {
        auto iter = std::find(dayDateVector.begin(), dayDateVector.end(), re);
        mIndexVector.append(std::distance(std::begin(dayDateVector), iter));
    }
    return mIndexVector;
}

DataRecordWidget::DataRecordWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataRecordWidget),
    m_pChartView(new QChartView(this)),
    m_pAimTSeries(new QLineSeries),
    m_pInTSeries(new QLineSeries),
    m_pOutTSeries(new QLineSeries),
    m_pDuctTSeries(new QLineSeries),
    m_pInHumiditySeries(new QLineSeries),
    m_pInPaSeries(new QLineSeries),
    m_pRecordHandle(new RecordReadModbus),
    m_pro(new Progressbar(tr("进度"), ""))
{
    ui->setupUi(this);
    m_pRecordThread = new QThread(this);
    m_pRecordHandle->moveToThread(m_pRecordThread);
    m_pRecordThread->start();

    this->initUi();
    this->initConnection();
    this->resize(300, 400);

    /*
     * TODO: 数据查询优先查询数据库，如果数据库没有则提示用户是否需要
     *  向请求当天的数据
    */
}

DataRecordWidget::~DataRecordWidget()
{
    delete ui;
    if(m_pRecordThread != nullptr){
        m_pRecordThread->quit();
        m_pRecordThread->wait(1000);
        delete m_pRecordThread;
        m_pRecordThread = nullptr;
    }
    delete m_pro;
}

void DataRecordWidget::mousePoint(QPoint point)
{
    auto pos = m_pChart->mapFromParent(m_pChartView->mapFromGlobal(point));
    m_zeroPoint = m_pChart->mapToPosition(QPointF(m_pAxisX->min().toMSecsSinceEpoch(), m_pAxisYL->min()));
    m_maxPoint = m_pChart->mapToPosition(QPointF(m_pAxisX->max().toMSecsSinceEpoch(), m_pAxisYL->max()));
    QPointF posLeft = QPointF(m_zeroPoint.x(), m_maxPoint.y());
    QPointF posRight = QPointF(m_maxPoint.x() - m_pToolTipWidget->width(), m_maxPoint.y());
    if((pos.y() >= m_maxPoint.y() && pos.y() <= m_zeroPoint.y())
            && (pos.x() >= m_zeroPoint.x() && pos.x() <= m_maxPoint.x())){
        m_pLineH->setLine(m_zeroPoint.x(), pos.y(), m_maxPoint.x(), pos.y());
        m_pLineV->setLine(pos.x(), m_maxPoint.y(), pos.x(), m_zeroPoint.y());
        m_pLineH->show();
        m_pLineV->show();
        m_isLeft = pos.x() < (m_maxPoint.x() + m_zeroPoint.x()) / 2;
        auto posTool = m_isLeft ? posRight : posLeft;
        auto x = m_pChart->mapToValue(pos, m_pAimTSeries).x();
        // 根据横坐标获取数据 通过 aimTv 的x坐标找到对应的索引
        int index = 0;
        auto xVector = m_pAimTSeries->pointsVector();
        if(xVector.isEmpty()){return;}
        for (int i=0; i<xVector.length(); ++i) {
            if(fabs(x - xVector[i].x()) <= 1e-12){
                index = i;
                break;
            }else if(x < xVector[i].x()){
                index = i - 1;
                break;
            }else {
                index = -1;
            }
        }
        if(index == -1){
            m_pToolTipWidget->show(posTool, {x, 0, 0, 0, 0, 0, 0});
        }else{
            m_pToolTipWidget->show(posTool, {x, m_pAimTSeries->at(index).y(), m_pInTSeries->at(index).y(),
                                             m_pOutTSeries->at(index).y(), m_pDuctTSeries->at(index).y(),
                                             m_pInHumiditySeries->at(index).y(), m_pInPaSeries->at(index).y()});
        }
    }else{
        m_pLineH->hide();
        m_pLineV->hide();
        m_isLeft = pos.x() < (m_maxPoint.x() + m_zeroPoint.x()) / 2;
        m_pToolTipWidget->hide();
    }
    this->update();
    return;
}

void DataRecordWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void DataRecordWidget::initUi()
{
    ui->splitter_data_record->setStretchFactor(0, 2);
    ui->splitter_data_record->setStretchFactor(1, 1);
    this->initChartView();
    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setColor(QColor(0x40, 0x99, 0xF5));
    pen.setWidth(1);
    m_pLineH = new QGraphicsLineItem(m_pChartView->chart());
    m_pLineV = new QGraphicsLineItem(m_pChartView->chart());
    m_pLineH->setPen(pen);
    m_pLineV->setPen(pen);
    m_pLineH->setZValue(100);
    m_pLineV->setZValue(100);
    m_pLineH->hide();
    m_pLineV->hide();

    m_pToolTipWidget = new GraphicsProxyWidget(m_pChartView->chart());
    ui->spinBox_data_page->setRange(1, ONE_PAGE_POINTS);
    ui->dateEdit_date_page->setDate(QDate::currentDate());
    QStringList recordHeadList{"MAC", "IP", "版本号", "时间日期", "日龄",
                               "目标温度", "室内温度", "室内湿度", "室内压力", "室外温度",
                               "风管温度", "废气温度T6", "旁通阀", "热水阀", "回收Q",
                               "回收E", "回收N", "预热温度T7", "级别"};
    ui->tableWidget_data_record->setColumnCount(recordHeadList.length());
    ui->tableWidget_data_record->setHorizontalHeaderLabels(recordHeadList);
    ui->tableWidget_data_record->horizontalHeader()->setVisible(true);
    ui->tableWidget_data_record->verticalHeader()->setVisible(false);
    ui->tableWidget_data_record->setColumnWidth(0, 150);
    ui->tableWidget_data_record->setColumnWidth(2, 80);
    ui->tableWidget_data_record->setColumnWidth(3, 150);
    m_pro->setModal(true);

    // TODO: 暂时不做
    ui->pushButton_day->setVisible(false);
    ui->pushButton_db_page->setVisible(false);
}

void DataRecordWidget::initChartView()
{
    QHBoxLayout *playout = new QHBoxLayout;
    playout->addWidget(m_pChartView);
    playout->setMargin(0);
    ui->widget_chart->setLayout(playout);

    m_pChart = new QChart();
    m_pAxisX = new QDateTimeAxis;
    auto minTime = QDateTime::currentDateTime();
    minTime.setTime(QTime(0, 0, 0));
    auto maxTime = QDateTime::currentDateTime();
    maxTime.setTime(QTime(23, 59, 59));
    m_pAxisX->setRange(minTime, maxTime);
    m_pAxisX->setFormat("hh:mm");
    m_pAxisX->setTickCount(9);
    m_pChart->addAxis(m_pAxisX, Qt::AlignBottom);

    m_pAxisYL = new QValueAxis();
    m_pAxisYL->setRange(-20,40);
    m_pChart->addAxis(m_pAxisYL, Qt::AlignLeft);
    m_pAxisYR = new QValueAxis();
    m_pAxisYR->setRange(-100,100);
    m_pChart->addAxis(m_pAxisYR, Qt::AlignRight);
    m_pChart->addSeries(m_pAimTSeries);
    m_pChart->addSeries(m_pInTSeries);
    m_pChart->addSeries(m_pOutTSeries);
    m_pChart->addSeries(m_pDuctTSeries);
    m_pChart->addSeries(m_pInHumiditySeries);
    m_pChart->addSeries(m_pInPaSeries);
    // 目标温度
    m_pAimTSeries->attachAxis(m_pAxisX);
    m_pAimTSeries->attachAxis(m_pAxisYL);
    m_pAimTSeries->setName(tr("目标温度"));
    // 室内温度
    m_pInTSeries->attachAxis(m_pAxisX);
    m_pInTSeries->attachAxis(m_pAxisYL);
    m_pInTSeries->setName(tr("室内温度"));
    // 室外温度
    m_pOutTSeries->attachAxis(m_pAxisX);
    m_pOutTSeries->attachAxis(m_pAxisYL);
    m_pOutTSeries->setName(tr("室外温度"));
    // 风管温度
    m_pDuctTSeries->attachAxis(m_pAxisX);
    m_pDuctTSeries->attachAxis(m_pAxisYL);
    m_pDuctTSeries->setName(tr("风管温度"));
    // 舍内湿度
    m_pInHumiditySeries->attachAxis(m_pAxisX);
    m_pInHumiditySeries->attachAxis(m_pAxisYR);
    m_pInHumiditySeries->setName(tr("室内湿度"));
    // 舍内压力
    m_pInPaSeries->attachAxis(m_pAxisX);
    m_pInPaSeries->attachAxis(m_pAxisYR);
    m_pInPaSeries->setName(tr("室内压力"));

    m_pChart->legend()->setVisible(true);
    m_pChart->legend()->setAlignment(Qt::AlignTop);
    // 设置外边界全都为0
    m_pChart->layout()->setContentsMargins(0, 0, 0, 0);
    // 设置内边界都为0
//    pchart->setMargins(QMargins(0, 0, 0, 0));
    // 设置背景区域无圆角
    m_pChart->setBackgroundRoundness(0);
    m_pChart->setAnimationOptions(QChart::SeriesAnimations);
    m_pChartView->setChart(m_pChart);

    // 测试代码
//    QTime zero{0, 0, 0};
//    DataFrame data;
//    auto now = QDateTime::currentDateTime();
//    for (int i=0; i<288; ++i) {
//        QDateTime dt{now.date()};
//        dt.setTime(zero.addSecs(i*5*60));
//        if(i > 100){
//            data.push_back({dt.toMSecsSinceEpoch(), 10.0, 20.0, 35.0,15.0, -20, -40});
//        }else{
//            data.push_back({dt.toMSecsSinceEpoch(), 5.0, 10.0, 25.0, 30.0, -50, -80});
//        }
//    }
//    this->addSeriesData(data);
}

void DataRecordWidget::initConnection()
{
    connect(ui->pushButton_db_page, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->pushButton_data_record_page, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(0);});
    connect(this, &DataRecordWidget::mulRequest, m_pRecordHandle, &RecordReadModbus::onMulRequest);
    connect(m_pRecordHandle, &RecordReadModbus::recordResult, this, [this](QVector<QVariant> buf){
        int nOldRowCount = ui->tableWidget_data_record->rowCount();
        ui->tableWidget_data_record->insertRow(nOldRowCount);
        for (int i=0; i<buf.length(); ++i) {
            auto date = QDateTime::fromSecsSinceEpoch(buf[i].toInt());
            if(i==3){
                auto item = new QTableWidgetItem(date.toString("yyyy-MM-dd hh:mm:ss"));
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_data_record->setItem(nOldRowCount, i, item);
            }else if(i == 13 || i == 17 || i == 8 || i == 7){
                auto item = new QTableWidgetItem("***");
                if(g_Hot){ item->setText(buf[i].toString()); }
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_data_record->setItem(nOldRowCount, i, item);
            }else{
                auto item = new QTableWidgetItem(buf[i].toString());
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_data_record->setItem(nOldRowCount, i, item);
            }
            DataFrame data{SeriesItem(date.toMSecsSinceEpoch(), buf[5].toDouble(), buf[6].toDouble(),
                                 buf[10].toDouble(), buf[11].toDouble(), buf[8].toDouble(), buf[9].toDouble())};
            this->addSeriesData(data);
        }
        ui->tableWidget_data_record->scrollToBottom();

    });
    connect(m_pRecordHandle, &RecordReadModbus::recordError, this, [this](QString msg){
        qDebug() << msg;
    });
    connect(m_pRecordHandle, &RecordReadModbus::progressValue, m_pro,  &Progressbar::onSetValue);
}

void DataRecordWidget::addSeriesData(const DataFrame &data)
{
    if(data.empty()){return;}
    // 时间, 目标温度, 室内温度, 室外温度, 风管温度, 舍内湿度, 舍内压力
    for(auto item : data){
        m_pAimTSeries->append(std::get<0>(item), std::get<1>(item));
        m_pInTSeries->append(std::get<0>(item), std::get<2>(item));
        m_pOutTSeries->append(std::get<0>(item), std::get<3>(item));
        m_pDuctTSeries->append(std::get<0>(item), std::get<4>(item));
        m_pInHumiditySeries->append(std::get<0>(item), std::get<5>(item));
        m_pInPaSeries->append(std::get<0>(item), std::get<6>(item));
    }
}

void DataRecordWidget::clearSeriesData()
{
    auto series = m_pChartView->chart()->series();
    for(auto item : series){
        QLineSeries *line = dynamic_cast<QLineSeries*>(item);
        line->clear();
    }
}

void DataRecordWidget::updatePageData(bool flag)
{
    if(!flag){return;}
    m_pro->onSetRange(0, ONE_PAGE_POINTS);
    m_pro->show();
    emit mulRequest(ui->dateEdit_date_page->date(), ui->spinBox_data_page->value(), m_mac, m_ip, m_hardware);
    this->clearUi();
}

void DataRecordWidget::setHandleModbus(std::shared_ptr<HandleModbus> handle)
{
    this->m_pHandbleModbus = handle;
    this->m_pRecordHandle->setHandleModbus(handle);
    this->clearUi();
}

void DataRecordWidget::on_spinBox_data_page_editingFinished()
{
    m_pro->onSetRange(0, ONE_PAGE_POINTS);
    m_pro->show();
    emit mulRequest(ui->dateEdit_date_page->date(), ui->spinBox_data_page->value(), m_mac, m_ip, m_hardware);
    this->clearUi();
}

void DataRecordWidget::on_toolButton_next_page_clicked()
{
    auto mindex = ui->spinBox_data_page->value();
    mindex += 1;
    if(mindex > ui->spinBox_data_page->maximum()){return;}
    m_pro->onSetRange(0, ONE_PAGE_POINTS);
    m_pro->show();
    ui->spinBox_data_page->setValue(mindex);
    emit mulRequest(ui->dateEdit_date_page->date(), ui->spinBox_data_page->value(), m_mac, m_ip, m_hardware);
    this->clearUi();
}

void DataRecordWidget::on_toolButton_pre_page_clicked()
{
    auto mindex = ui->spinBox_data_page->value();
    mindex -= 1;
    if(mindex < ui->spinBox_data_page->minimum()){return;}
    m_pro->onSetRange(0, ONE_PAGE_POINTS);
    m_pro->show();
    ui->spinBox_data_page->setValue(mindex);
    emit mulRequest(ui->dateEdit_date_page->date(), ui->spinBox_data_page->value(), m_mac, m_ip, m_hardware);
    this->clearUi();
}

void DataRecordWidget::on_dateEdit_date_page_editingFinished()
{
    m_pro->onSetRange(0, ONE_PAGE_POINTS);
    m_pro->show();
    emit mulRequest(ui->dateEdit_date_page->date(), ui->spinBox_data_page->value(), m_mac, m_ip, m_hardware);
    this->clearUi();
}

void DataRecordWidget::on_toolButton_next_day_clicked()
{
    auto old = ui->dateEdit_date_page->date();
    auto now = old.addDays(1);
    ui->dateEdit_date_page->setDate(now);
    m_pro->onSetRange(0, ONE_PAGE_POINTS);
    m_pro->show();
    ui->spinBox_data_page->setValue(1);
    auto date = ui->dateEdit_date_page->date();
    m_pAxisX->setRange(QDateTime(date, QTime(0, 0, 0)), QDateTime(date, QTime(23, 59, 59)));
    emit mulRequest(ui->dateEdit_date_page->date(), ui->spinBox_data_page->value(), m_mac, m_ip, m_hardware);
    this->clearUi();
}

void DataRecordWidget::on_toolButton_pre_day_clicked()
{
    auto old = ui->dateEdit_date_page->date();
    auto now = old.addDays(-1);
    ui->dateEdit_date_page->setDate(now);
    m_pro->onSetRange(0, ONE_PAGE_POINTS);
    m_pro->show();
    ui->spinBox_data_page->setValue(1);
    auto date = ui->dateEdit_date_page->date();
    m_pAxisX->setRange(QDateTime(date, QTime(0, 0, 0)), QDateTime(date, QTime(23, 59, 59)));
    emit mulRequest(ui->dateEdit_date_page->date(), ui->spinBox_data_page->value(), m_mac, m_ip, m_hardware);
    this->clearUi();
}

void DataRecordWidget::on_pushButton_day_clicked()
{

}

void DataRecordWidget::clearUi()
{
    ui->tableWidget_data_record->clearContents();
    ui->tableWidget_data_record->setRowCount(0);
    auto all = m_pChart->series();
    for(auto serie : all){
        QLineSeries  *item = dynamic_cast<QLineSeries*>(serie);
        item->clear();
    }
}
