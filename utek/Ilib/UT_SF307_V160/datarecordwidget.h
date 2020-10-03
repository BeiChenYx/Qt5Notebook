#ifndef DATARECORDWIDGET_H
#define DATARECORDWIDGET_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDateTimeAxis>
#include <QGraphicsLineItem>
#include <QLineSeries>
#include <QValueAxis>
#include <QList>
#include <QThread>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <algorithm>
#include <cmath>
#include "handlemodbus.h"
#include "customdialog.h"

using namespace QtCharts;

namespace Ui {
class DataRecordWidget;
}

/*
 * 数据记录的间隔是5分钟，一天有288个数据
 * 分成6页，每页则有 288 / 6 = 48 数据
 */
const int ONE_PAGE_POINTS = 48;
const QString DB_NAME = "SF307.db";
const QString CONNECT_NAME = "ut_sf307";
const QString DATA_TABLE_NAME = "data_record";
const QString ALARM_TABLE_NAME = "alarm_record";



/* 数据记录的矩阵数据结构
 * 列分别为:
 *   时间, 目标温度, 室内温度, 室外温度, 风管温度, 舍内湿度, 舍内压力
 */
typedef std::tuple<qint64, double, double, double,
                             double, double, double> SeriesItem;
typedef std::vector<SeriesItem>  DataFrame;

class GraphicsProxyWidget;
class ToolTipWidget;
class RecordReadModbus;


class DataRecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataRecordWidget(QWidget *parent = nullptr);
    ~DataRecordWidget();
    void initUi();
    void initConnection();
    void initChartView();
    void mousePoint(QPoint point);

    void addSeriesData(const DataFrame &data);
    void clearSeriesData();

    void setHandleModbus(std::shared_ptr<HandleModbus> handle);
    void updatePageData(bool flag=true);

    // 设置mac ip hardware信息
    void setDevInfo(QString macAddr, QString ipAddr, QString hardware) {
        this->m_mac = macAddr;
        this->m_ip = ipAddr;
        this->m_hardware = hardware;
    }

signals:
    void mulRequest(QDate date, int page, QString macAddr, QString ipAddr, QString hardware);


protected:
    void resizeEvent(QResizeEvent *event);
    void clearUi();

private slots:
    void on_spinBox_data_page_editingFinished();
    void on_toolButton_next_page_clicked();
    void on_toolButton_pre_page_clicked();
    void on_dateEdit_date_page_editingFinished();
    void on_toolButton_next_day_clicked();
    void on_toolButton_pre_day_clicked();

    void on_pushButton_day_clicked();

private:
    Ui::DataRecordWidget *ui;
    QChartView  *m_pChartView;
    QChart *m_pChart;
    QDateTimeAxis *m_pAxisX;
    QValueAxis *m_pAxisYL;
    QValueAxis *m_pAxisYR;
    QGraphicsLineItem *m_pLineH;
    QGraphicsLineItem *m_pLineV;
    GraphicsProxyWidget *m_pToolTipWidget;

    // 目标温度
    QLineSeries *m_pAimTSeries;
    // 室内温度
    QLineSeries *m_pInTSeries;
    // 室外温度
    QLineSeries *m_pOutTSeries;
    // 风管温度
    QLineSeries *m_pDuctTSeries;
    // 舍内湿度
    QLineSeries *m_pInHumiditySeries;
    // 舍内压力
    QLineSeries *m_pInPaSeries;

    // 最大的点坐标，chart的右上角, QChart坐标系
    QPointF m_maxPoint;
    // 最小点的坐标, chart的坐标原点, QChart坐标系
    QPointF m_zeroPoint;
    // 鼠标在中心左边
    bool m_isLeft = true;

    RecordReadModbus *m_pRecordHandle;
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
    QThread *m_pRecordThread;

    QString m_mac;
    QString m_ip;
    QString m_hardware;

    Progressbar  *m_pro;
};


// 构建悬浮提示框
class ToolTipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolTipWidget(QWidget *parent = nullptr);

    void updateUi(const SeriesItem &item);
private:
    QLabel *m_pDate;
    QLabel *m_pAimT;
    QLabel *m_pInT;
    QLabel *m_pOutT;
    QLabel *m_pDuctT;
    QLabel *m_pHumidity;
    QLabel *m_pInPa;
};

// ToolTipWidget的代理Widget
class GraphicsProxyWidget: public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    explicit GraphicsProxyWidget(QGraphicsItem *parent)
        : QGraphicsProxyWidget(parent){
        this->setZValue(200);
        this->m_pTipWidget = new ToolTipWidget;
        this->setWidget(m_pTipWidget);
        this->hide();
    }

    qreal width(){return this->size().width();}
    qreal height(){return this->size().height();}

    void show(QPointF pos, const SeriesItem &data){
        this->setGeometry(QRectF(pos, this->size()));
        this->m_pTipWidget->updateUi(data);
        QGraphicsWidget::show();
    }

private:
    ToolTipWidget *m_pTipWidget;
};

Q_DECLARE_METATYPE(QVector<QVariant>);
// 使用modbus来访问设备，读取历史记录和报警记录
class RecordReadModbus : public QObject
{
    Q_OBJECT
public:
    RecordReadModbus(QWidget *parent=nullptr) : QObject(parent){
        qRegisterMetaType<QVector<QVariant> >("RecordData");
    }
    ~RecordReadModbus(){ }
    void setHandleModbus(std::shared_ptr<HandleModbus> handle){
        this->m_pHandbleModbus = handle;
    }

    bool insertData(QList<QVector<uint16_t> > &data, QString mac_addr, QString ip_addr, QString hardware);
    bool selectData(QDate date, QString mac_addr);
    // 根据日期和mac地址查询数据库目前有的数据时间集合
    QVector<qint64> isSelectData(QDate date, QString mac_addr);

public slots:
    /*
     * 根据分页来读取多个点的记录，每次读取一页，
     * 一页的数据为 288 / ONE_PAGE_POINTS
     * 从1 开始 到 6 页
     * @date: 日期
     * @page: 页码
     */
    void onMulRequest(QDate date, int page, QString macAddr, QString ipAddr, QString hardware);
    // 获取全天288条数据
    void onOneDayRequest(QDate date, QString macAddr, QString ipAddr, QString hardware);

signals:
    void recordResult(QVector<QVariant> buf);
    void recordError(QString msg);
    void progressValue(int value);

protected:
    /*
     * 和request函数配合去读取历史记录
     * @[out] 通过信号将数据的结果发送出去
     */
    bool onResponse(QVector<uint16_t> &buf);

    /*
     * @brief:负责发送读取请求, 在地址0x14EC的位置写入三个寄存器表示要读取的数据
     *  100毫秒后查询标志位是否是回应标志，如果是就读取后面的数据，不是就再等等
     * @dIndex: 偏移位置，根据当前日期向前偏移的天数，最大180天
     * @mIndex: 点偏移, 时间索引，间隔5分钟，点偏移0-288
     */
    void onRequest(const int dIndex, const int mIndex);

private:
    std::weak_ptr<HandleModbus> m_pHandbleModbus;
};

#endif // DATARECORDWIDGET_H
