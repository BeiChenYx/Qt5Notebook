import sys
import time
import datetime

import pandas as pd

from PyQt5.QtChart import QChartView, QChart, QLineSeries, QLegend, QCategoryAxis, QCandlestickSeries, QCandlestickSet,\
    QStackedBarSeries, QBarSet
from PyQt5.QtCore import Qt, QPointF, QPoint, pyqtSignal, QRectF, QMargins, QEvent, QObject
from PyQt5.QtGui import QPainter, QPen, QColor, QMouseEvent
from PyQt5.QtWidgets import QApplication, QGraphicsLineItem, QWidget, QLabel, QHBoxLayout, QVBoxLayout, QGridLayout, \
    QGraphicsProxyWidget, QSplitter


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

    def __init__(self):
        super(KLineChartView, self).__init__()
        self.setRenderHint(QPainter.Antialiasing)  # 抗锯齿
        self._chart = QChart()
        self.stocks = read_tick_data()
        self.category = [trade_date[4:] for trade_date in self.stocks['trade_date']]
        self._count = len(self.category)
        self._zero_point = QPointF(0, 0)
        self._max_point = QPointF(0, 0)
        self.resize(800, 300)

        self.init_chart()
        self._zero_value = (0, self._chart.axisY().min())
        self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self.cate_width = (self._max_point.x() - self._zero_point.x()) / len(self.category)

    def resizeEvent(self, event):
        super(KLineChartView, self).resizeEvent(event)
        self._zero_point = self._chart.mapToPosition(QPointF(self._zero_value[0], self._zero_value[1]))
        self._max_point = self._chart.mapToPosition(QPointF(self._max_value[0], self._max_value[1]))
        self.cate_width = (self._max_point.x() - self._zero_point.x()) / len(self.category)

    def max_point(self):
        return QPointF(self._max_point.x() - self.cate_width / 2, self._max_point.y())

    def min_point(self):
        return QPointF(self._zero_point.x() - self.cate_width / 2, self._zero_point.y())

    def init_chart(self):
        self._chart.setAnimationOptions(QChart.SeriesAnimations)
        series = QCandlestickSeries()
        series.setIncreasingColor(QColor(Qt.red))
        series.setDecreasingColor(QColor(Qt.green))
        series.setName(self.stocks['name'].iloc[0])
        for _, stock in self.stocks.iterrows():
            time_p = datetime.datetime.strptime(stock['trade_date'], '%Y%m%d')
            time_p = float(time.mktime(time_p.timetuple()))
            _set = QCandlestickSet(float(stock['open']), float(stock['high']),
                                   float(stock['low']), float(stock['close']), time_p, series)
            # _set.hovered.connect(self.handleBarHoverd)  # 鼠标悬停
            series.append(_set)
        self._chart.addSeries(series)

        self._chart.createDefaultAxes()
        self._chart.setLocalizeNumbers(True)
        axis_x = self._chart.axisX()
        axis_y = self._chart.axisY()
        axis_x.setGridLineVisible(False)
        axis_y.setGridLineVisible(False)
        axis_x.setCategories(self.category)
        max_p = self.stocks[['high', 'low']].stack().max() + 10
        min_p = self.stocks[['high', 'low']].stack().min() - 10
        axis_y.setRange(min_p, max_p)

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
    def __init__(self):
        super(VLineChartView, self).__init__()
        self.stocks = read_tick_data()
        self.category = [trade_date[4:] for trade_date in self.stocks['trade_date']]
        self._chart = QChart()
        self._zero_point = QPointF(0, 0)
        self._max_point = QPointF(0, 0)
        self.resize(800, 300)
        self.init_chart()
        self._zero_value = (0, self._chart.axisY().min())
        self._max_value = (len(self._chart.axisX().categories()), self._chart.axisY().max())
        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self.cate_width = (self._max_point.x() - self._zero_point.x()) / len(self.category)

    def resizeEvent(self, event):
        super(VLineChartView, self).resizeEvent(event)
        self._zero_point = self._chart.mapToPosition(QPointF(self._zero_value[0], self._zero_value[1]))
        self._max_point = self._chart.mapToPosition(QPointF(self._max_value[0], self._max_value[1]))
        self.cate_width = (self._max_point.x() - self._zero_point.x()) / len(self.category)

    def max_point(self):
        return QPointF(self._max_point.x() - self.cate_width / 2, self._max_point.y())

    def min_point(self):
        return QPointF(self._zero_point.x() - self.cate_width / 2, self._zero_point.y())

    def init_chart(self):
        self._chart.setAnimationOptions(QChart.SeriesAnimations)
        series = QStackedBarSeries()
        series.setName('数量')
        bar_red = QBarSet('red')
        bar_red.setColor(Qt.red)
        bar_green = QBarSet('green')
        bar_green.setColor(Qt.green)
        for _, stock in self.stocks.iterrows():
            if stock['open'] < stock['close']:
                bar_red.append(stock['vol'] / 100)
                bar_green.append(0)
            else:
                bar_red.append(0)
                bar_green.append(stock['vol'] / 100)

        series.append(bar_red)
        series.append(bar_green)
        self._chart.addSeries(series)
        self._chart.createDefaultAxes()
        self._chart.setLocalizeNumbers(True)
        axis_x = self._chart.axisX()
        axis_y = self._chart.axisY()
        axis_x.setGridLineVisible(False)
        axis_y.setGridLineVisible(False)
        axis_y.setLabelFormat("%.2f")
        axis_x.setCategories(self.category)
        max_p = self.stocks[['vol', ]].stack().max() / 100 + 10
        min_p = self.stocks[['vol', ]].stack().min() / 100 - 10
        axis_y.setRange(min_p, max_p)

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
        self.k_view = KLineChartView()
        self.v_view = VLineChartView()
        self.resize(800, 600)
        # self.showMaximized()

        self.v_splitter = QSplitter(Qt.Vertical, self)
        self.v_splitter.addWidget(self.k_view)
        self.v_splitter.addWidget(self.v_view)
        self.v_splitter.setStretchFactor(0, 3)
        self.v_splitter.setStretchFactor(1, 1)

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
            # TODO: 横坐标使用时间来显示，并且只显示指定个数的时间, K线图隐藏横坐标轴，V线图显示横坐标轴即可
            # TODO: 添加浮动框, 在K线里面添加所有信息，包括成交量的信息

        return super(KVWidget, self).eventFilter(obj, event)

    def resizeEvent(self, event):
        super(KVWidget, self).resizeEvent(event)
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


if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = KVWidget()
    view.show()
    sys.exit(app.exec_())