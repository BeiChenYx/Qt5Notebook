import sys
import time
import datetime

import pandas as pd
import numpy as np

from PyQt5.QtChart import QChartView, QChart, QLineSeries, QLegend, QCategoryAxis, QCandlestickSeries, QCandlestickSet,\
    QStackedBarSeries, QBarSet
from PyQt5.QtCore import Qt, QPointF, QPoint, pyqtSignal, QRectF, QMargins, QEvent, QObject
from PyQt5.QtGui import QPainter, QPen, QColor, QMouseEvent, QFont
from PyQt5.QtWidgets import QApplication, QGraphicsLineItem, QWidget, QLabel, QHBoxLayout, QVBoxLayout, QGridLayout, \
    QGraphicsProxyWidget, QSplitter, QPushButton, QGraphicsSimpleTextItem


def read_tick_data():
    """
    生成蜡烛图的数据
    蜡烛图需要 open, high, low, close,timestamp
    从csv文件中获取数据
    """
    tick_data = pd.read_csv('ts_code_000001_sz_202003010807.csv', encoding='utf-8')
    tick_data['trade_date'] = tick_data['trade_date'].apply(lambda x: str(x))
    return tick_data


class ToolTipWidget(QWidget):
    """ 构建悬浮提示框 """

    def __init__(self):
        super(ToolTipWidget, self).__init__()
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setStyleSheet(
            """
            ToolTipWidget{background: rgba(255, 255, 255, 0.5); border: 1px solid #dfe6eb;}
            QLabel{color: black;}
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
        label_val = QLabel('成交量:', self)
        self.label_vol_str = QLabel('****', self)
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
        layout.addWidget(label_val, 5, 0)
        layout.addWidget(self.label_vol_str, 5, 1)
        self.setLayout(layout)

    def update_ui(self, trade_date, open_v, close_v, high_v, low_v, vol_v):
        """ 更新数据 """
        self.label_date_str.setText(trade_date)
        self.label_open_str.setText(open_v)
        self.label_close_str.setText(close_v)
        self.label_high_str.setText(high_v)
        self.label_low_str.setText(low_v)
        self.label_vol_str.setText(vol_v)


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

    def show(self, trade_date, open_v, close_v, high_v, low_v, vol_v, position):
        """ 显示窗口及更新数据 """
        self.setGeometry(QRectF(position, self.size()))
        self.tipWidget.update_ui(trade_date, open_v, close_v, high_v, low_v, vol_v)
        super(GraphicsProxyWidget, self).show()


class KLineChartView(QChartView):

    # QCandlestickSeries的hovered的信号响应后传递日期出去
    candles_hovered = pyqtSignal(bool, str)

    def __init__(self, data: pd.DataFrame):
        super(KLineChartView, self).__init__()
        self.setRenderHint(QPainter.Antialiasing)  # 抗锯齿
        self._chart = QChart()
        self._series = QCandlestickSeries()
        self._stocks = data
        self._category = list()
        self._count = None

        self.init_chart()
        self._zero_value = (0, self._chart.axisY().min())
        self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
        self._zero_point = self._chart.mapToPosition(QPointF(self._zero_value[0], self._zero_value[1]))
        self._max_point = self._chart.mapToPosition(QPointF(self._max_value[0], self._max_value[1]))
        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)

        self._series.hovered.connect(self.on_series_hovered)

    def on_series_hovered(self, status, candles_set):
        trade_date = time.strftime('%Y%m%d', time.localtime(candles_set.timestamp()))
        self.candles_hovered.emit(status, trade_date)

    def set_name(self, name):
        self._series.setName(name)

    def clear_series_values(self):
        self._series.clear()
        self._chart.axisY().setRange(0, 10)
        self._chart.axisX().setCategories(list())
        self._stocks = None

    def add_series_values(self, data: pd.DataFrame, is_init=False):
        self._stocks = data
        self._category = self._stocks['trade_date']
        self._count = len(self._category)
        for _, stock in self._stocks.iterrows():
            time_p = datetime.datetime.strptime(stock['trade_date'], '%Y%m%d')
            time_p = float(time.mktime(time_p.timetuple()))
            _set = QCandlestickSet(float(stock['open']), float(stock['high']),
                                   float(stock['low']), float(stock['close']), time_p, self._series)
            self._series.append(_set)

        if not is_init:
            self._stocks = data
            self._category = self._stocks['trade_date']
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
        return QPointF(self._max_point.x() + self._cate_width / 2, self._max_point.y())

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
        axis_x.setVisible(False)
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
        # 设置内边界的bottom为0
        margins = self._chart.margins()
        self._chart.setMargins(QMargins(margins.left(), 0, margins.right(), 0))
        # 设置背景区域无圆角
        self._chart.setBackgroundRoundness(0)


class VLineChartView(QChartView):

    bar_hovered = pyqtSignal(bool, str)

    def __init__(self, data: pd.DataFrame):
        super(VLineChartView, self).__init__()
        self._stocks = data
        self._category = self._stocks['trade_date']
        self._chart = QChart()
        self._chart.setAnimationOptions(QChart.SeriesAnimations)
        self._series = QStackedBarSeries()
        # 成交量以万股为单位
        self._vol_multiple = 10000
        self.init_chart()
        self._zero_value = (0, self._chart.axisY().min())
        self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
        self._zero_point = self._chart.mapToPosition(QPointF(self._zero_value[0], self._zero_value[1]))
        self._max_point = self._chart.mapToPosition(QPointF(self._max_value[0], self._max_value[1]))
        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self._cate_width = (self._max_point.x() - self._zero_point.x()) / len(self._category)
        self._series.hovered.connect(self.on_series_hovered)

        x_index_list = np.percentile(range(len(self._category)), [0, 25, 50, 75, 100])
        self._x_axis_list = [QGraphicsSimpleTextItem(self._category[int(index)], self._chart) for index in x_index_list]
        [axis.setText(axis.text()[4:]) for axis in self._x_axis_list[1:]]
        self._v_b = QGraphicsSimpleTextItem('B', self._chart)
        self._v_b.setZValue(100)

    def on_series_hovered(self, status, index):
        self.bar_hovered.emit(status, self._category[index])

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
            self._stocks = data
            self._category = self._stocks['trade_date']
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
        # 绘制自定义X轴
        self._x_axis_list[0].setPos(self._zero_point.x() - self._cate_width, self._zero_point.y() + 10)
        self._x_axis_list[1].setPos(self._max_point.x() * 0.25, self._zero_point.y() + 10)
        self._x_axis_list[2].setPos(self._max_point.x() * 0.5, self._zero_point.y() + 10)
        self._x_axis_list[3].setPos(self._max_point.x() * 0.75, self._zero_point.y() + 10)
        self._x_axis_list[4].setPos(
            self._max_point.x() - self._x_axis_list[-1].boundingRect().width(),
            self._zero_point.y() + 10
        )
        # 20180207 这个日期的柱形图上面画一个字母b
        vol = self._stocks[self._stocks['trade_date'] == '20180207']['vol'] / self._vol_multiple
        print('vol:', vol, ' trade_date:', '20180207')
        pos = self._chart.mapToPosition(QPointF(list(self._category).index('20180207'), vol))
        pos = QPointF(pos.x() - self._cate_width / 2, pos.y() - self._v_b.boundingRect().height())
        self._v_b.setPos(pos)

    def max_point(self):
        return QPointF(self._max_point.x() + self._cate_width / 2, self._max_point.y())

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
        # 设置内边界的bottom为0
        # margins = self._chart.margins()
        # self._chart.setMargins(QMargins(margins.left(), 0, margins.right(), 0))
        self._chart.setBackgroundRoundness(0)


class KVWidget(QWidget):
    """ 主页面 """

    def __init__(self):
        super(KVWidget, self).__init__()
        self.setMouseTracking(True)

        # 获取数据
        self.stocks = read_tick_data()

        self.k_view = KLineChartView(self.stocks[:100])
        self.v_view = VLineChartView(self.stocks[:100])
        self.k_view.candles_hovered.connect(self.on_series_hovered)
        self.v_view.bar_hovered.connect(self.on_series_hovered)

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
        # self.showMaximized()

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

        # 鼠标在图表的位置, 初始化在左上角
        self.tool_tip_widget = GraphicsProxyWidget(self.k_view.chart())
        self.k_zero_point = self.k_view.min_point()
        self.k_max_point = self.k_view.max_point()
        self.v_zero_point = self.v_view.min_point()
        self.v_max_point = self.v_view.max_point()
        self._hovered_pos_left = QPointF(self.k_zero_point.x(), self.k_max_point.y())
        self._hovered_pos_right = QPointF(self.k_max_point.x() - self.tool_tip_widget.width(), self.k_max_point.y())
        self._hovered_pos = self._hovered_pos_left
        # 鼠标在中心左边
        self._is_left = True

        # 事件过滤
        QApplication.instance().installEventFilter(self)

        # TODO: 向左右拖动图表能显示之前或之后的图表，且坐标跟着变化
        # TODO: 能标准成本线，能计算指定两个点的涨幅度

    def eventFilter(self, obj: QObject, event: QEvent):
        if event.type() == QEvent.MouseMove:
            pos = QMouseEvent(event).globalPos()
            k_chart_pos = self.k_view.chart().mapFromParent(self.k_view.mapFromGlobal(pos))
            v_chart_pos = self.v_view.chart().mapFromParent(self.v_view.mapFromGlobal(pos))
            self.k_zero_point = self.k_view.min_point()
            self.k_max_point = self.k_view.max_point()
            self.v_zero_point = self.v_view.min_point()
            self.v_max_point = self.v_view.max_point()
            self.update_lines(k_chart_pos, v_chart_pos)
        elif event.type() == QEvent.Resize and obj == self:
            self.update_margins()
            self.k_zero_point = self.k_view.min_point()
            self.k_max_point = self.k_view.max_point()

        return super(KVWidget, self).eventFilter(obj, event)

    def update_lines(self, k_chart_pos, v_chart_pos):
        """ 更新跟踪的十字线 """
        if (self.k_zero_point.y() >= k_chart_pos.y() >= self.k_max_point.y()) \
                and (self.k_zero_point.x() <= k_chart_pos.x() <= self.k_max_point.x()):
            self.k_line_h.setLine(self.k_zero_point.x(), k_chart_pos.y(), self.k_max_point.x(), k_chart_pos.y())
            self.k_line_v.setLine(k_chart_pos.x(), self.k_max_point.y(), k_chart_pos.x(), self.k_zero_point.y())
            self.v_line_v.setLine(v_chart_pos.x(), self.v_max_point.y(), v_chart_pos.x(), self.v_zero_point.y())
            self.k_line_h.show()
            self.k_line_v.show()
            self.v_line_v.show()
            self.v_line_h.hide()
            self._is_left = k_chart_pos.x() < (self.k_max_point.x() + self.k_zero_point.x()) / 2
        elif (self.v_zero_point.y() >= v_chart_pos.y() >= self.v_max_point.y()) \
                and (self.v_zero_point.x() <= v_chart_pos.x() <= self.v_max_point.x()):
            self.k_line_v.setLine(k_chart_pos.x(), self.k_max_point.y(), k_chart_pos.x(), self.k_zero_point.y())
            self.v_line_h.setLine(self.v_zero_point.x(), v_chart_pos.y(), self.k_max_point.x(), v_chart_pos.y())
            self.v_line_v.setLine(v_chart_pos.x(), self.v_max_point.y(), v_chart_pos.x(), self.v_zero_point.y())
            self.k_line_h.hide()
            self.k_line_v.show()
            self.v_line_v.show()
            self.v_line_h.show()
            self._is_left = v_chart_pos.x() < (self.v_max_point.x() + self.v_zero_point.x()) / 2
        else:
            self.k_line_h.hide()
            self.k_line_v.hide()
            self.v_line_v.hide()
            self.v_line_h.hide()

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
        self.k_view.clear_series_values()
        self.v_view.clear_series_value()
        self.k_view.set_name('')

    def on_add_clicked(self):
        """ 添加数据 """
        self.k_view.add_series_values(self.stocks)
        self.v_view.add_series_values(self.stocks)
        self.k_view.set_name(self.stocks['name'].iloc[0])
        self.update_margins()

    def on_series_hovered(self, status: bool, index_date: str):
        """ QCandlestickSeries的hovered的信号响应槽 """
        if status:
            self._hovered_pos_left = QPointF(self.k_zero_point.x(), self.k_max_point.y())
            self._hovered_pos_right = QPointF(self.k_max_point.x() - self.tool_tip_widget.width(), self.k_max_point.y())
            self._hovered_pos = self._hovered_pos_right if self._is_left else self._hovered_pos_left
            tip_value_df = self.stocks[self.stocks['trade_date'] == index_date]
            if tip_value_df.empty:
                return
            self.tool_tip_widget.show(
                index_date,
                str(tip_value_df['open'].iloc[0]),
                str(tip_value_df['close'].iloc[0]),
                str(tip_value_df['high'].iloc[0]),
                str(tip_value_df['low'].iloc[0]),
                str(tip_value_df['vol'].iloc[0]),
                self._hovered_pos
            )
        else:
            self.tool_tip_widget.hide()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = KVWidget()
    view.show()
    # cx = CustomAxisX()
    # cx.show()
    sys.exit(app.exec_())