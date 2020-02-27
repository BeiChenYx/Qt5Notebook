import sys
import time
import datetime

import pandas as pd

from PyQt5.QtChart import QChartView, QChart, QLineSeries, QLegend, QCategoryAxis, QCandlestickSeries, QCandlestickSet,\
    QStackedBarSeries, QBarSet
from PyQt5.QtCore import Qt, QPointF, QPoint, pyqtSignal, QRectF, QMargins, QEvent, QObject
from PyQt5.QtGui import QPainter, QPen, QColor, QMouseEvent, QFont
from PyQt5.QtWidgets import QApplication, QGraphicsLineItem, QWidget, QLabel, QHBoxLayout, QVBoxLayout, QGridLayout, \
    QGraphicsProxyWidget, QSplitter, QPushButton


def read_tick_data():
    """
    生成蜡烛图的数据
    蜡烛图需要 open, high, low, close,timestamp
    从csv文件中获取数据
    """
    sw_50 = pd.read_csv('sw_daily_201907162109.csv', encoding='utf-8')
    sw_50['trade_date'] = sw_50['trade_date'].apply(lambda x: str(x))
    return sw_50


class ToolTipWidget(QWidget):
    """ 构建悬浮提示框 """

    def __init__(self):
        super(ToolTipWidget, self).__init__()
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setStyleSheet(
            """
            ToolTipWidget{background: rgba(50, 50, 50, 100);}
            QLabel{color: white;}
            """
        )
        layout = QGridLayout()
        layout.setContentsMargins(10, 10, 10, 10)
        label_date = QLabel("时间:", self)
        self.label_date_str = QLabel("****-**-**", self)
        label_open = QLabel("开盘价:", self)
        self.label_open_str = QLabel("****", self)
        label_close = QLabel("收盘价:", self)
        self.label_close_str = QLabel("****", self)
        label_high = QLabel("最高价:", self)
        self.label_high_str = QLabel("****", self)
        label_low = QLabel("最低价:")
        self.label_low_str = QLabel("****", self)
        layout.addWidget(label_date, 0, 0)
        layout.addWidget(self.label_date_str, 0, 1)
        layout.addWidget(label_open, 1, 0)
        layout.addWidget(self.label_open_str, 1, 1)
        layout.addWidget(label_close, 2, 0)
        layout.addWidget(self.label_close_str, 2, 1)
        layout.addWidget(label_high, 3, 0)
        layout.addWidget(self.label_high_str, 3, 1)
        layout.addWidget(label_low, 4, 0)
        layout.addWidget(self.label_low_str, 4, 1)
        self.setLayout(layout)

    def updateUi(self, trade_date, openT, closeT, highT, lowT):
        """ 更新数据 """
        self.label_date_str.setText(trade_date)
        self.label_open_str.setText(openT)
        self.label_close_str.setText(closeT)
        self.label_high_str.setText(highT)
        self.label_low_str.setText(lowT)


class GraphicsProxyWidget(QGraphicsProxyWidget):
    """ ToolTipWidget的代理Widget """
    def __init__(self, *args, **kwargs):
        super(GraphicsProxyWidget, self).__init__(*args, **kwargs)
        self.setZValue(200)
        self.tipWidget = ToolTipWidget()
        self.setWidget(self.tipWidget)
        self.hide()

    def width(self):
        return self.size().width()

    def height(self):
        return self.size().height()

    def show(self, trade_date, openT, closeT, highT, lowT, position):
        """ 显示窗口及更新数据 """
        self.setGeometry(QRectF(position, self.size()))
        self.tipWidget.updateUi(trade_date, openT, closeT, highT, lowT)
        super(GraphicsProxyWidget, self).show()


class KLineChartView(QChartView):

    def __init__(self, data: pd.DataFrame):
        super(KLineChartView, self).__init__()
        self.setRenderHint(QPainter.Antialiasing)  # 抗锯齿
        self._chart = QChart()
        self._series = QCandlestickSeries()
        self._stocks = data
        self._category = list()
        self._count = None
        self._zero_point = QPointF(0, 0)
        self._max_point = QPointF(0, 0)
        self.resize(800, 300)

        self.init_chart()
        self._zero_value = (0, self._chart.axisY().min())
        self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)

    def set_name(self, name):
        self._series.setName(name)

    def clear_series_values(self):
        self._series.clear()
        self._chart.axisY().setRange(0, 10)
        self._chart.axisX().setCategories(list())
        self._stocks = None

    def add_series_values(self, data: pd.DataFrame, is_init=False):
        self._stocks = data
        self._category = [trade_date[4:] for trade_date in self._stocks['trade_date']]
        self._count = len(self._category)
        for _, stock in self._stocks.iterrows():
            time_p = datetime.datetime.strptime(stock['trade_date'], '%Y%m%d')
            time_p = float(time.mktime(time_p.timetuple()))
            _set = QCandlestickSet(float(stock['open']), float(stock['high']),
                                   float(stock['low']), float(stock['close']), time_p, self._series)
            # _set.hovered.connect(self.handleBarHoverd)  # 鼠标悬停
            self._series.append(_set)

        if not is_init:
            axis_x = self._chart.axisX()
            axis_y = self._chart.axisY()
            axis_x.setCategories(self._category)
            max_p = self._stocks[['high', 'low']].stack().max()
            min_p = self._stocks[['high', 'low']].stack().min()
            axis_y.setRange(min_p * 0.99, max_p * 1.01)
            self._zero_value = (0, self._chart.axisY().min())
            self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
            # 计算x轴单个cate的宽度，用来处理横线不能画到边界
            self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)

    def resizeEvent(self, event):
        super(KLineChartView, self).resizeEvent(event)
        self._zero_point = self._chart.mapToPosition(QPointF(self._zero_value[0], self._zero_value[1]))
        self._max_point = self._chart.mapToPosition(QPointF(self._max_value[0], self._max_value[1]))
        self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)

    def max_point(self):
        return QPointF(self._max_point.x() - self._cate_width / 2, self._max_point.y())

    def min_point(self):
        return QPointF(self._zero_point.x() - self._cate_width / 2, self._zero_point.y())

    def init_chart(self):
        self._chart.setAnimationOptions(QChart.SeriesAnimations)
        self._series.setIncreasingColor(QColor(Qt.red))
        self._series.setDecreasingColor(QColor(Qt.green))
        self._series.setName(self._stocks['name'].iloc[0])
        self.add_series_values(self._stocks, True)
        self._chart.addSeries(self._series)

        self._chart.createDefaultAxes()
        self._chart.setLocalizeNumbers(True)
        axis_x = self._chart.axisX()
        axis_y = self._chart.axisY()
        axis_x.setGridLineVisible(False)
        axis_y.setGridLineVisible(False)
        axis_x.setCategories(self._category)
        axis_x.setLabelsVisible(False)
        max_p = self._stocks[['high', 'low']].stack().max()
        min_p = self._stocks[['high', 'low']].stack().min()
        axis_y.setRange(min_p * 0.99, max_p * 1.01)

        # chart的图例
        legend = self._chart.legend()
        # 设置图例由Series来决定样式
        legend.setMarkerShape(QLegend.MarkerShapeFromSeries)

        self.setChart(self._chart)
        # 设置外边界全部为0
        self._chart.layout().setContentsMargins(0, 0, 0, 0)
        # 设置内边界都为0
        # self._chart.setMargins(QMargins(0, 0, 0, 0))
        # 设置背景区域无圆角
        self._chart.setBackgroundRoundness(0)


class VLineChartView(QChartView):
    def __init__(self, data: pd.DataFrame):
        super(VLineChartView, self).__init__()
        self._stocks = data
        self._category = [trade_date[4:] for trade_date in self._stocks['trade_date']]
        self._chart = QChart()
        self._chart.setAnimationOptions(QChart.SeriesAnimations)
        self._series = QStackedBarSeries()
        self._zero_point = QPointF(0, 0)
        self._max_point = QPointF(0, 0)
        # 成交量以万股为单位
        self._vol_multiple = 10000
        self.resize(800, 300)
        self.init_chart()
        self._zero_value = (0, self._chart.axisY().min())
        self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)

    def clear_series_value(self):
        self._series.clear()
        self._stocks = None
        self._chart.axisY().setRange(0, 10)
        self._chart.axisX().setCategories(list())

    def add_series_values(self, data: pd.DataFrame, is_init=False):
        self._stocks = data
        bar_red = QBarSet('red')
        bar_red.setColor(Qt.red)
        bar_green = QBarSet('green')
        bar_green.setColor(Qt.green)
        for _, stock in self._stocks.iterrows():
            if stock['open'] < stock['close']:
                bar_red.append(stock['vol'] / self._vol_multiple)
                bar_green.append(0)
            else:
                bar_red.append(0)
                bar_green.append(stock['vol'] / self._vol_multiple)

        self._series.append(bar_red)
        self._series.append(bar_green)

        if not is_init:
            axis_x = self._chart.axisX()
            axis_y = self._chart.axisY()
            axis_x.setCategories(self._category)
            max_p = self._stocks[['vol', ]].stack().max()
            min_p = self._stocks[['vol', ]].stack().min()
            axis_y.setRange(min_p / self._vol_multiple * 0.9, max_p / self._vol_multiple * 1.1)
            self._zero_value = (0, self._chart.axisY().min())
            self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
            # 计算x轴单个cate的宽度，用来处理横线不能画到边界
            self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)

    def resizeEvent(self, event):
        super(VLineChartView, self).resizeEvent(event)
        self._zero_point = self._chart.mapToPosition(QPointF(self._zero_value[0], self._zero_value[1]))
        self._max_point = self._chart.mapToPosition(QPointF(self._max_value[0], self._max_value[1]))
        self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)

    def max_point(self):
        return QPointF(self._max_point.x() - self._cate_width / 2, self._max_point.y())

    def min_point(self):
        return QPointF(self._zero_point.x() - self._cate_width / 2, self._zero_point.y())

    def init_chart(self):
        self.add_series_values(self._stocks, True)
        self._chart.addSeries(self._series)
        self._chart.createDefaultAxes()
        self._chart.setLocalizeNumbers(True)
        axis_x = self._chart.axisX()
        axis_y = self._chart.axisY()
        axis_x.setGridLineVisible(False)
        axis_y.setGridLineVisible(False)
        axis_y.setLabelFormat("%.2f")
        axis_x.setCategories(self._category)
        axis_x.setLabelsVisible(False)
        max_p = self._stocks[['vol', ]].stack().max()
        min_p = self._stocks[['vol', ]].stack().min()
        axis_y.setRange(min_p / self._vol_multiple * 0.9, max_p / self._vol_multiple * 1.1)

        # chart的图例
        legend = self._chart.legend()
        legend.hide()
        # 设置图例由Series来决定样式
        # legend.setMarkerShape(QLegend.MarkerShapeFromSeries)

        self.setChart(self._chart)
        self._chart.layout().setContentsMargins(0, 0, 0, 0)
        # self._chart.setMargins(QMargins(0, 0, 0, 0))
        self._chart.setBackgroundRoundness(0)


class KVWidget(QWidget):
    """ 主页面 """

    def __init__(self):
        super(KVWidget, self).__init__()
        self.setMouseTracking(True)
        # 获取数据
        self.stocks = read_tick_data()

        self.k_view = KLineChartView(self.stocks)
        self.v_view = VLineChartView(self.stocks)
        self.resize(800, 600)
        # self.showMaximized()

        btn_widget = QWidget()
        h_layout = QHBoxLayout()
        btn_clear = QPushButton('清除')
        btn_add = QPushButton('添加')
        h_layout.addStretch()
        h_layout.addWidget(btn_clear)
        h_layout.addWidget(btn_add)
        btn_widget.setLayout(h_layout)

        btn_clear.clicked.connect(self.on_clear_clicked)
        btn_add.clicked.connect(self.on_add_clicked)

        self.v_splitter = QSplitter(Qt.Vertical, self)
        self.v_splitter.addWidget(self.k_view)
        self.v_splitter.addWidget(self.v_view)
        self.v_splitter.addWidget(btn_widget)
        self.v_splitter.setStretchFactor(0, 3)
        self.v_splitter.setStretchFactor(1, 2)
        self.v_splitter.setStretchFactor(2, 1)

        layout = QVBoxLayout()
        # layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.v_splitter)
        self.setLayout(layout)

        # 鼠标跟踪的十字线
        pen = QPen()
        pen.setStyle(Qt.DotLine)
        pen.setColor(QColor(0x40, 0x99, 0xF5))
        pen.setWidth(1)
        self.k_line_h = QGraphicsLineItem(self.k_view.chart())
        self.k_line_v = QGraphicsLineItem(self.k_view.chart())
        self.v_line_h = QGraphicsLineItem(self.v_view.chart())
        self.v_line_v = QGraphicsLineItem(self.v_view.chart())
        self.k_line_h.setPen(pen)
        self.k_line_v.setPen(pen)
        self.v_line_h.setPen(pen)
        self.v_line_v.setPen(pen)
        self.k_line_h.setZValue(100)
        self.k_line_v.setZValue(100)
        self.v_line_h.setZValue(100)
        self.v_line_v.setZValue(100)
        self.k_line_h.hide()
        self.k_line_v.hide()
        self.v_line_h.hide()
        self.v_line_v.hide()

        # 事件过滤
        QApplication.instance().installEventFilter(self)

    def eventFilter(self, obj: QObject, event: QEvent):
        if event.type() == QEvent.MouseMove:
            pos = QMouseEvent(event).globalPos()
            k_chart_pos = self.k_view.chart().mapFromParent(self.k_view.mapFromGlobal(pos))
            v_chart_pos = self.v_view.chart().mapFromParent(self.v_view.mapFromGlobal(pos))
            # is_k_chart = self.k_view.chart().contains(k_chart_pos)
            # is_v_chart = self.v_view.chart().contains(v_chart_pos)
            # k线图原点和最右上点的坐标，v线图原点和最右上点的坐标
            k_zero_point = self.k_view.min_point()
            v_zero_point = self.v_view.min_point()
            k_max_point = self.k_view.max_point()
            v_max_point = self.v_view.max_point()
            if (k_zero_point.y() >= k_chart_pos.y() >= k_max_point.y()) \
                    and (k_zero_point.x() <= k_chart_pos.x() <= k_max_point.x()):
                self.k_line_h.setLine(k_zero_point.x(), k_chart_pos.y(), k_max_point.x(), k_chart_pos.y())
                self.k_line_v.setLine(k_chart_pos.x(), k_max_point.y(), k_chart_pos.x(), k_zero_point.y())
                self.v_line_v.setLine(v_chart_pos.x(), v_max_point.y(), v_chart_pos.x(), v_zero_point.y())
                self.k_line_h.show()
                self.k_line_v.show()
                self.v_line_v.show()
                self.v_line_h.hide()
            elif (v_zero_point.y() >= v_chart_pos.y() >= v_max_point.y()) \
                    and (v_zero_point.x() <= v_chart_pos.x() <= v_max_point.x()):
                self.k_line_v.setLine(k_chart_pos.x(), k_max_point.y(), k_chart_pos.x(), k_zero_point.y())
                self.v_line_h.setLine(v_zero_point.x(), v_chart_pos.y(), k_max_point.x(), v_chart_pos.y())
                self.v_line_v.setLine(v_chart_pos.x(), v_max_point.y(), v_chart_pos.x(), v_zero_point.y())
                self.k_line_h.hide()
                self.k_line_v.show()
                self.v_line_v.show()
                self.v_line_h.show()
            else:
                self.k_line_h.hide()
                self.k_line_v.hide()
                self.v_line_v.hide()
                self.v_line_h.hide()
            # TODO: 添加浮动框, 在K线里面添加所有信息，包括成交量的信息
            # TODO: 使用自绘的方式在横坐标下面写日期，并且只显示指定个数的时间, K线图隐藏横坐标轴，V线图显示横坐标轴即可
            # TODO: 在给定日期的K线图上绘制文字
            # TODO: 向左右拖动图表能显示之前或之后的图表，且坐标跟着变化
            # TODO: 能标准成本线，能计算指定两个点的涨幅度

        return super(KVWidget, self).eventFilter(obj, event)

    def resizeEvent(self, event):
        super(KVWidget, self).resizeEvent(event)
        self.update_margins()

    def update_margins(self):
        margin_k = self.k_view.chart().margins()
        margin_v = self.v_view.chart().margins()
        width_k = self.k_view.chart().plotArea().width()
        width_v = self.v_view.chart().plotArea().width()
        sub = width_k - width_v
        if sub > 0:
            self.k_view.chart().setMargins(QMargins(margin_k.left() + sub, margin_k.top(),
                                                    margin_k.right(), margin_k.bottom()))
        else:
            self.v_view.chart().setMargins(QMargins(margin_v.left() - sub, margin_v.top(),
                                                    margin_v.right(), margin_v.bottom()))
        self.update()

    def on_clear_clicked(self):
        """ 清空图表 """
        print('on clear clicked')
        self.k_view.clear_series_values()
        self.v_view.clear_series_value()
        self.k_view.set_name('')

    def on_add_clicked(self):
        """ 添加数据 """
        print('on add clicked')
        self.k_view.add_series_values(self.stocks)
        self.v_view.add_series_values(self.stocks)
        self.k_view.set_name(self.stocks['name'].iloc[0])
        self.update_margins()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = KVWidget()
    view.show()
    sys.exit(app.exec_())