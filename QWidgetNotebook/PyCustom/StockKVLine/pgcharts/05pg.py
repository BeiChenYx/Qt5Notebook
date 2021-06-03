"""
K线和成交量绘制
"""
import pdb
import sys

import pyqtgraph as pg
import numpy as np
import pandas as pd
from PyQt5.QtCore import QPointF, QDateTime, QRectF, Qt, pyqtSignal
from PyQt5.QtGui import QPainter, QPen, QColor, QMouseEvent, QFont, QPicture, QFontMetrics
from PyQt5.QtWidgets import (QApplication, QGraphicsItem, QWidget, QGraphicsRectItem,
        QLabel, QHBoxLayout, QVBoxLayout, QGridLayout,
        QGraphicsProxyWidget, QSplitter, QPushButton)


def read_tick_data():
    """
    生成蜡烛图的数据
    蜡烛图需要 open, high, low, close,timestamp
    从csv文件中获取数据
    """
    _data = pd.read_csv('../ts_code_000001_sz_202003010807.csv', encoding='utf-8')
    _data['trade_date'] = _data['trade_date'].apply(lambda x: str(x))
    return _data


class TopLegendWidget(QWidget):
    """ 顶部图例页面 """
    def __init__(self, name_value:dict=None):
        """ 
        :name_value: 需要显示的键值对, {str: xxx}
        """
        super(TopLegendWidget, self).__init__()
        _font = self.font()
        _fm = QFontMetrics(_font)
        self.setFixedHeight(_fm.height() + 4)
        # print('font height: ', _fm.height())
        self.setAttribute(Qt.WA_StyledBackground, True)
        # self.setStyleSheet("background-color: rgba(250, 0, 200, 0.5)")
        self._suffix = '_value'
        self._h_layout = QHBoxLayout()
        self._h_layout.setSpacing(12)
        self._h_layout.setContentsMargins(0, 0, 0, 0)
        _h_layout = QHBoxLayout()
        _h_layout.addLayout(self._h_layout)
        _h_layout.addStretch()
        self.setLayout(_h_layout)
        self.layout().setContentsMargins(0, 0, 0, 0)
        for name, value in name_value.items():
            self.add_name_value(name, value)

    def add_name_value(self, name, value):
        """ 添加需要展示的图例的键值对 """
        _h_tmp_layout = QHBoxLayout()
        _label_name = QLabel()
        _label_name.setText(str(name) + ': ')
        _label_name.setObjectName(str(name))
        _label_value = QLabel()
        _label_value.setText(str(value))
        _label_value.setObjectName(str(name) + self._suffix)
        _h_tmp_layout.addWidget(_label_name)
        _h_tmp_layout.addWidget(_label_value)
        _h_tmp_layout.setSpacing(0)
        self._h_layout.addLayout(_h_tmp_layout)

    def set_value_by_name(self, name, value):
        """ 通过名字来设置具体的控件的值 """
        label_value = self.findChild(QLabel, str(name + self._suffix))
        label_value.setText(str(value))
        self.update()


class CandlestickItem(pg.GraphicsObject):
    """ 自定义Item """
    def __init__(self, data: pd.DataFrame):
        super(CandlestickItem, self).__init__()
        self._data = data
        self._pic = QPicture()
        self.generatePicture()

    def generatePicture(self):
        """ 绘图 """
        p = QPainter(self._pic)
        p.setPen(pg.mkPen({'color': "CCCCCC"}))
        # 两个坐标点的1/3, 两个点的距离为横坐标的差
        # 如果使用时间来，会出现间隔不连续，因为股票数据时间本身是不连续的
        w = 1/3
        for row in self._data.itertuples():
            _t = getattr(row, 'Index')
            h_price = getattr(row, 'high')
            l_price = getattr(row, 'low')
            o_price = getattr(row, 'open')
            c_price = getattr(row, 'close')
            p.drawLine(QPointF(_t, l_price), QPointF(_t, h_price))
            
            if o_price > c_price:
                p.setBrush(pg.mkBrush('g'))
            else:
                p.setBrush(pg.mkBrush('r'))

            p.drawRect(QRectF(_t - w, o_price, w * 2, c_price - o_price))
        
        p.end()

    def paint(self, p, *args):
        p.drawPicture(0, 0, self._pic)

    def boundingRect(self):
        return QRectF(self._pic.boundingRect())


class KWidget(QWidget):
    """ K线的窗口 """
    sig_y_padding_changed = pyqtSignal(float)
    sig_v_pos = pyqtSignal(float)
    def __init__(self):
        super(KWidget, self).__init__()
        self._top_widget = TopLegendWidget({'M5': 0.0, 'M10': 0.0})
        self._pw = pg.PlotWidget()
        self._v_layout = QVBoxLayout()
        self._v_layout.addWidget(self._top_widget)
        self._v_layout.addWidget(self._pw)
        self._v_layout.setSpacing(0)
        self._v_layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(self._v_layout)

        self._data = read_tick_data()
        self._xdict = dict(enumerate(self._data['trade_date']))
        # 按照50步长来去data的日期
        self._axis_date = [(i, self._data['trade_date'].iloc[i]) 
                for i in range(0, self._data.shape[0], 50)]
        self._label_info = None
        self._v_line = None
        self._h_line = None
        self._view_box = None
        self._proxy = None
        self._k_item = None
        self._y_min = 0.0
        self._y_max = 0.0

        self.plot_k()
        self._view_box.sigYRangeChanged.connect(self.on_y_range_changed)
        _rect = self._view_box.targetRect()
        print('KWidget rect: ', _rect)

    def on_y_range_changed(self, obj, start_end):
        _padding = self._view_box.suggestPadding(0)
        self.sig_y_padding_changed.emit(_padding)

    def on_y_padding_changed(self, padding):
        """ y轴的padding发生改变 """
        _padding = self._view_box.suggestPadding(0)
        if padding > _padding:
            self._view_box.setYRange(self._y_min, self._y_max, padding=_padding)
            print('KWidget _padding: ', _padding, '  padding: ', padding)
            self._view_box.updateViewRange()

    def view_box(self):
        """ 返回视图 """
        return self._view_box

    def plot_k(self):
        """ 绘制K线 """
        axis_date = pg.AxisItem(orientation='bottom')
        axis_date.setTicks([self._axis_date])
        self._pw.setAxisItems({'bottom': axis_date})

        self._k_item = CandlestickItem(self._data) 
        self._pw.addItem(self._k_item)
        # 添加第二个图形, 收盘价走势线
        self._pw.plot(x=list(self._xdict.keys()), y=self._data['close'].values, 
                pen='r', name='close')

        self._label_info = pg.TextItem()
        self._pw.addItem(self._label_info)
        self._pw.showGrid(x=True, y=True, alpha=0.5)

        self._v_line = pg.InfiniteLine(angle=90, movable=False)
        self._h_line = pg.InfiniteLine(angle=0, movable=False)
        self._pw.addItem(self._v_line, ignoreBounds=True)
        self._pw.addItem(self._h_line, ignoreBounds=True)
        
        self._view_box = self._pw.getPlotItem().getViewBox()

        # 显示部分数据
        _tmp_data = self._data.iloc[-120:]
        _x_left = _tmp_data.index[0]
        _x_right = _tmp_data.index[-1]
        _y_max = _tmp_data['high'].max()
        _y_min = _tmp_data['low'].min()
        self._y_min = _y_min * 0.99
        self._y_max = _y_max * 1.01
        top_left = QPointF(_x_left * 0.99, _y_max * 1.01)
        bottom_right = QPointF(_x_right * 1.01, _y_min * 0.99)
        self._view_box.setRange(rect=QRectF(top_left, bottom_right))
        # 设置Y轴的值范围和左右的padding
        self._view_box.setYRange(self._y_min, self._y_max, padding=0.1)

        # self._proxy 代理必须存在，不能用局部变量
        self._proxy = pg.SignalProxy(self._pw.scene().sigMouseMoved, 
                rateLimit=30, slot=self.mouse_moved)

    def mouse_moved(self, evt):
        pos = evt[0]
        if self._pw.sceneBoundingRect().contains(pos):
            mousePoint = self._view_box.mapSceneToView(pos)
            index = int(mousePoint.x())
            if 0 < index < self._data.shape[0]:
                self._label_info.setHtml("<p style='color:white'>日期：{0}</p>\
                        <p style='color:white'>开盘：{1}</p>\
                        <p style='color:white'>收盘：{2}</p>\
                        <p style='color:white'>最高价：{3}</p>\
                        <p style='color:white'>最低价：{4}</p>".format(
                            self._data['trade_date'].iloc[index],
                            self._data['open'].iloc[index],
                            self._data['close'].iloc[index],
                            self._data['high'].iloc[index],
                            self._data['low'].iloc[index]))
                self._label_info.setPos(mousePoint.x(), mousePoint.y())
                self._top_widget.set_value_by_name('M5', self._data['close'].iloc[index])
                self._top_widget.set_value_by_name('M10', self._data['low'].iloc[index])

            self._v_line.setPos(mousePoint.x())
            self._h_line.setPos(mousePoint.y())
            self.sig_v_pos.emit(mousePoint.x())

    def on_v_pos(self, posx):
        self._v_line.setPos(posx)


class VolItem(pg.GraphicsObject):
    """ 自定义Item """
    def __init__(self, data: pd.DataFrame):
        super(VolItem, self).__init__()
        self._data = data
        self._pic = QPicture()
        self.generatePicture()

    def generatePicture(self):
        """ 绘图 """
        p = QPainter(self._pic)
        p.setPen(pg.mkPen({'color': "CCCCCC"}))
        # 两个坐标点的1/3, 两个点的距离为横坐标的差
        # 如果使用时间来，会出现间隔不连续，因为股票数据时间本身是不连续的
        w = 1/3
        for row in self._data.itertuples():
            _t = getattr(row, 'Index')
            vol = getattr(row, 'vol') / 10000
            # vol = getattr(row, 'vol')
            o_price = getattr(row, 'open')
            c_price = getattr(row, 'close')
            
            if o_price > c_price:
                p.setBrush(pg.mkBrush('g'))
            else:
                p.setBrush(pg.mkBrush('r'))

            p.drawRect(QRectF(_t - w, 0, w * 2, vol))
        
        p.end()

    def paint(self, p, *args):
        p.drawPicture(0, 0, self._pic)

    def boundingRect(self):
        return QRectF(self._pic.boundingRect())


class VolWidget(QWidget):
    """ 成交量的窗口 """

    sig_y_padding_changed = pyqtSignal(float)
    sig_v_pos = pyqtSignal(float)

    def __init__(self):
        super(VolWidget, self).__init__()
        self._fm = QFontMetrics(self.font())
        self._pw = pg.PlotWidget()
        self._top_widget = TopLegendWidget({'VOL': 0.0})
        self._v_layout = QVBoxLayout()
        self._v_layout.addWidget(self._top_widget)
        self._v_layout.addWidget(self._pw)
        self._v_layout.setSpacing(0)
        self._v_layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(self._v_layout)

        self._data = read_tick_data()
        self._xdict = dict(enumerate(self._data['trade_date']))
        # 按照50步长来去data的日期
        self._axis_date = [(i, self._data['trade_date'].iloc[i]) 
                for i in range(0, self._data.shape[0], 50)]
        self._v_line = None
        self._h_line = None
        self._view_box = None
        self._proxy = None
        self._k_item = None
        self._y_min = 0.0
        self._y_max = 0.0

        self.plot_volue()

        self._view_box.sigYRangeChanged.connect(self.on_y_range_changed)
        _rect = self._view_box.targetRect()
        print('VolWidget rect: ', _rect)

    def on_y_range_changed(self, obj, start_end):
        _padding = self._view_box.suggestPadding(0)
        self.sig_y_padding_changed.emit(_padding)

    def on_y_padding_changed(self, padding):
        """ y轴的padding发生改变 """
        _padding = self._view_box.suggestPadding(0)
        if padding > _padding:
            self._view_box.setYRange(self._y_min, self._y_max, padding=_padding)
            self._view_box.updateViewRange()
            print('VolWidget _padding: ', _padding, '  padding: ', padding)

    def view_box(self):
        """ 返回视图 """
        return self._view_box

    def plot_volue(self):
        """ 绘制成交量柱形图 """
        axis_date = pg.AxisItem(orientation='bottom')
        axis_date.setTicks([self._axis_date])
        self._pw.setAxisItems({'bottom': axis_date})

        self._k_item = VolItem(self._data) 
        self._pw.addItem(self._k_item)

        self._pw.showGrid(x=True, y=True, alpha=0.5)

        self._v_line = pg.InfiniteLine(angle=90, movable=False)
        self._h_line = pg.InfiniteLine(angle=0, movable=False)
        self._pw.addItem(self._v_line, ignoreBounds=True)
        self._pw.addItem(self._h_line, ignoreBounds=True)
        
        self._view_box = self._pw.getPlotItem().getViewBox()

        # 显示部分数据
        _tmp_data = self._data.iloc[-120:]
        _x_left = _tmp_data.index[0]
        _x_right = _tmp_data.index[-1]
        _y_max = self._data['vol'].max() / 10000
        # _y_max = self._data['vol'].max()
        self._y_min = 0
        self._y_max = _y_max * 1.01
        top_left = QPointF(_x_left * 0.99, _y_max * 1.01)
        bottom_right = QPointF(_x_right * 1.01, 0)
        # 设置可视范围
        self._view_box.setRange(rect=QRectF(top_left, bottom_right))
        # 设置y轴可视范围，调整padding
        self._view_box.setYRange(self._y_min, self._y_max, padding=0)
        # 设置鼠标禁用状态
        self._view_box.setMouseEnabled(x=True, y=False)
        # self._view_box.enableAutoRange(axis=self._view_box.YAxis, enable=True)

        # self._proxy 代理必须存在，不能用局部变量
        self._proxy = pg.SignalProxy(self._pw.scene().sigMouseMoved, 
                rateLimit=30, slot=self.mouse_moved)

    def mouse_moved(self, evt):
        pos = evt[0]
        if self._pw.sceneBoundingRect().contains(pos):
            mousePoint = self._view_box.mapSceneToView(pos)
            index = int(mousePoint.x())
            if 0 < index < self._data.shape[0]:
                self._top_widget.set_value_by_name('VOL', self._data['vol'].iloc[index])

            self._v_line.setPos(mousePoint.x())
            self._h_line.setPos(mousePoint.y())
            self.sig_v_pos.emit(mousePoint.x())

    def on_v_pos(self, posx):
        self._v_line.setPos(posx)


class MainWidget(QWidget):
    
    def __init__(self):
        super(MainWidget, self).__init__()
        self._k_widget = KWidget()
        self._vol_widget = VolWidget()
        self._spliter = QSplitter(Qt.Vertical, self)
        self.init_ui()
        self.init_connection() 
    
    def init_ui(self):
        self._spliter.addWidget(self._k_widget)
        self._spliter.addWidget(self._vol_widget)
        self._spliter.setStretchFactor(0, 8)
        self._spliter.setStretchFactor(1, 2)
        self._spliter.setHandleWidth(1)
        _layout = QHBoxLayout()
        _layout.addWidget(self._spliter)
        _layout.setContentsMargins(0, 0, 0, 0)
        _layout.setSpacing(0)
        self.setLayout(_layout)

    def init_connection(self):
        self._k_widget.view_box().setXLink(self._vol_widget.view_box())
        self._k_widget.sig_y_padding_changed.connect(self._vol_widget.on_y_padding_changed)
        self._vol_widget.sig_y_padding_changed.connect(self._k_widget.on_y_padding_changed)
        self._k_widget.sig_v_pos.connect(self._vol_widget.on_v_pos)
        self._vol_widget.sig_v_pos.connect(self._k_widget.on_v_pos)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = MainWidget()
    view.resize(500, 600)
    # view = KWidget()
    # view = VolWidget()
    view.show()
    sys.exit(app.exec_())
