import sys
import time
import datetime

import pandas as pd

from PyQt5.QtChart import QChartView, QChart, QLineSeries, QLegend, QCategoryAxis, QCandlestickSeries, QCandlestickSet,\
    QStackedBarSeries, QBarSet
from PyQt5.QtCore import Qt, QPointF, QPoint, pyqtSignal, QRectF, QMargins
from PyQt5.QtGui import QPainter, QPen, QColor
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
        self._chart = QChart(title='蜡烛图悬浮提示')
        self.stocks = read_tick_data()
        self.category = [trade_date[4:] for trade_date in self.stocks['trade_date']]
        self._count = len(self.category)
        self.resize(800, 300)

        self.init_chart()

        self.toolTipWidget = GraphicsProxyWidget(self._chart)

        # 鼠标跟踪的十字线
        self.lineItem_h = QGraphicsLineItem(self._chart)
        self.lineItem_v = QGraphicsLineItem(self._chart)
        pen = QPen()
        pen.setStyle(Qt.DotLine)
        pen.setColor(QColor(Qt.gray))
        pen.setWidth(2)
        self.lineItem_h.setPen(pen)
        self.lineItem_v.setPen(pen)
        self.lineItem_h.setZValue(100)
        self.lineItem_v.setZValue(100)
        self.lineItem_h.hide()
        self.lineItem_v.hide()

        # 坐标轴上最大最小的值
        # x 轴是
        self.min_x, self.max_x = 0, len(self._chart.axisX().categories())
        self.min_y, self.max_y = self._chart.axisY().min(), self._chart.axisY().max()
        # y 轴最高点坐标
        self.point_y_max = self._chart.mapToPosition(QPointF(self.min_x, self.max_y))
        # x 轴最高点坐标
        self.point_x_max = self._chart.mapToPosition(QPointF(self.max_x, self.min_y))
        # self.point_x_min = self._chart.mapToPosition(QPointF(self.min_x, self.min_y))

        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self.x_width = (self.point_x_max.x() - self.point_y_max.x()) / len(self.category)
        self.x_x_min = self.point_y_max.x() - self.x_width / 2
        self.x_x_max = self.point_x_max.x() - self.x_width / 2

        # 中间位置,用来判断TipWidget放在哪里
        mid_date = self.stocks['trade_date'].iloc[len(self.stocks['trade_date']) // 2]
        self.mid_x = float(time.mktime(datetime.datetime.strptime(str(mid_date), '%Y%m%d').timetuple()))
        self.left_pos = self.point_y_max
        self.right_pos = self._chart.mapToPosition(QPointF(self.max_x, self.max_y))

    def mouseMoveEvent(self, event):
        super(KLineChartView, self).mouseMoveEvent(event)
        pos = event.pos()
        if self.x_x_min < pos.x() < self.x_x_max \
                and self.point_x_max.y() > pos.y() > self.point_y_max.y():
            self.lineItem_h.setLine(self.x_x_min, pos.y(), self.x_x_max, pos.y())
            self.lineItem_v.setLine(pos.x(), self.point_y_max.y(), pos.x(), self.point_x_max.y())
            self.lineItem_h.show()
            self.lineItem_v.show()
        else:
            self.lineItem_h.hide()
            self.lineItem_v.hide()

    def resizeEvent(self, event):
        super(KLineChartView, self).resizeEvent(event)
        # y 轴最高点坐标
        self.point_y_max = self._chart.mapToPosition(QPointF(self.min_x, self.max_y))
        # x 轴最高点坐标
        self.point_x_max = self._chart.mapToPosition(QPointF(self.max_x, self.min_y))
        # 计算x轴单个cate的宽度，用来处理横线不能画到边界
        self.x_width = (self.point_x_max.x() - self.point_y_max.x()) / len(self.category)
        self.x_x_min = self.point_y_max.x() - self.x_width / 2
        self.x_x_max = self.point_x_max.x() - self.x_width / 2
        self.left_pos = self.point_y_max
        self.right_pos = self._chart.mapToPosition(
            QPointF(self.max_x, self.max_y))

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
            _set.hovered.connect(self.handleBarHoverd)  # 鼠标悬停
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

    def handleBarHoverd(self, status):
        """ 改变画笔的风格 """
        bar = self.sender()  # 信号发送者
        pen = bar.pen()
        if not pen:
            return
        pen.setStyle(Qt.DotLine if status else Qt.SolidLine)
        bar.setPen(pen)
        if status:
            # 通过 bar 可以获取横轴坐标(timestamp)和纵轴坐标(high)
            # 然后将坐标值转换为位置，显示 TipWidget 的位置
            right_pos = QPointF(self.right_pos.x() - self.toolTipWidget.width() - self.x_width, self.right_pos.y())
            pos = self.left_pos if bar.timestamp() > self.mid_x else right_pos
            trade_date = time.strftime('%Y%m%d', time.localtime(bar.timestamp()))
            self.toolTipWidget.show(
                str(trade_date),
                str(bar.open()),
                str(bar.close()),
                str(bar.high()),
                str(bar.low()),
                pos
            )
        else:
            self.toolTipWidget.hide()


class VLineChartView(QChartView):
    def __init__(self):
        super(VLineChartView, self).__init__()
        self.stocks = read_tick_data()
        self.category = [trade_date[4:] for trade_date in self.stocks['trade_date']]
        self.resize(800, 300)
        self.initChart()

    def initChart(self):
        self._chart = QChart(title='数量')
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


class MainView(QWidget):
    """ 主页面 """

    def __init__(self):
        super(MainView, self).__init__()
        self.k_view = KLineChartView()
        self.v_view = VLineChartView()
        # self.resize(800, 600)
        self.showMaximized()

        self.v_splitter = QSplitter(Qt.Vertical, self)
        self.v_splitter.addWidget(self.k_view)
        self.v_splitter.addWidget(self.v_view)
        self.v_splitter.setStretchFactor(0, 3)
        self.v_splitter.setStretchFactor(1, 1)

        layout = QVBoxLayout()
        # layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.v_splitter)
        self.setLayout(layout)

    def resizeEvent(self, event):
        super(MainView, self).resizeEvent(event)
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
    view = MainView()
    view.show()
    sys.exit(app.exec_())