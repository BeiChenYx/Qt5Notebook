import pdb
import sys

import pyqtgraph as pg
import numpy as np
import pandas as pd
from PyQt5.QtCore import QPointF, QDateTime, QRectF, Qt
from PyQt5.QtGui import QPainter, QPen, QColor, QMouseEvent, QFont, QPicture
from PyQt5.QtWidgets import (QApplication, QGraphicsItem, QWidget, QGraphicsRectItem,
        QLabel, QHBoxLayout, QVBoxLayout, QGridLayout,
        QGraphicsProxyWidget, QSplitter, QPushButton)


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
        p.setPen(pg.mkPen('w'))
        # 两个坐标点的1/3, 两个点的距离为横坐标的差
        # 如果使用时间来，会出现间隔不连续，因为股票数据时间本身是不连续的
        w = 1/3
        for row in self._data.itertuples():
            _t = getattr(row, 'Index')
            vol = getattr(row, 'vol') / 10000
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

"""
    def itemChange(self, change, value):
        if change == QGraphicsItem.ItemPositionChange and self.scene():
            newPos = value.toPointF();
            rect = QRectF(0, self.pos().y(), self.scene().width(), 0); # 水平移动
            # rect = QRectF(0, 0, 0, self.scene().height()); # 垂直移动
            # QRectF rect(0, this->pos().x(), scene()->width(), 0); # 向右下方移动
            # QRectF rect(0, -this->pos().x(), scene()->width(), 0); # 向右上方移动
            if not rect.contains(newPos):
                newPos.setX(min(rect.right(), max(newPos.x(), rect.left())));
                newPos.setY(min(rect.bottom(), max(newPos.y(), rect.top())));
                return newPos;
        return QGraphicsRectItem.itemChange(self, change, value);
"""
        

class KWidget(QWidget):
    
    def __init__(self):
        super(KWidget, self).__init__()
        self._data = None
        self._pw = None
        self._xdict = None
        self._axis_date = None
        self._label_info = None
        self._v_line = None
        self._h_line = None
        self._view_box = None
        self._proxy = None
        self._k_item = None

        self.read_tick_data()
        self.init_ui()
        self.plot_k()

    def read_tick_data(self):
        """
        生成蜡烛图的数据
        蜡烛图需要 open, high, low, close,timestamp
        从csv文件中获取数据
        """
        self._data = pd.read_csv('../ts_code_000001_sz_202003010807.csv', encoding='utf-8')
        self._data['trade_date'] = self._data['trade_date'].apply(lambda x: str(x))
        self._xdict = dict(enumerate(self._data['trade_date']))
        # 按照50步长来去data的日期
        self._axis_date = [(i, self._data['trade_date'].iloc[i]) 
                for i in range(0, self._data.shape[0], 50)]

    def init_ui(self):
        self._pw = pg.PlotWidget(title='平安银行')
        hLayout = QHBoxLayout() 
        hLayout.addWidget(self._pw)
        self.setLayout(hLayout)

    def plot_k(self):
        axis_date = pg.AxisItem(orientation='bottom')
        axis_date.setTicks([self._axis_date])
        self._pw.setAxisItems({'bottom': axis_date})

        self._k_item = VolItem(self._data) 
        self._pw.addItem(self._k_item)

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
        _y_max = self._data['vol'].max() / 10000
        top_left = QPointF(_x_left * 0.99, _y_max * 1.01)
        bottom_right = QPointF(_x_right * 1.01, 0)
        # 设置可视范围
        self._view_box.setRange(rect=QRectF(top_left, bottom_right))
        # 设置y轴可视范围，调整padding
        self._view_box.setYRange(0, _y_max * 1.01, padding=0)
        # 设置鼠标禁用状态
        self._view_box.setMouseEnabled(x=True, y=False)

        # self._proxy 代理必须存在，不能用局部变量
        self._proxy = pg.SignalProxy(self._pw.scene().sigMouseMoved, rateLimit=30, slot=self.mouse_moved)

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

            self._v_line.setPos(mousePoint.x())
            self._h_line.setPos(mousePoint.y())



if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = KWidget()
    view.show()
    sys.exit(app.exec_())
