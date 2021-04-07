import pdb
import sys

import pyqtgraph as pg
import numpy as np
import pandas as pd
from PyQt5.QtGui import QPainter, QPen, QColor, QMouseEvent, QFont
from PyQt5.QtWidgets import (QApplication, QGraphicsLineItem, QWidget,
        QLabel, QHBoxLayout, QVBoxLayout, QGridLayout,
        QGraphicsProxyWidget, QSplitter, QPushButton)


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
        # 按照5步长来去data的日期
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
        # 图例需要放在绘图前面
        self._pw.addLegend()
        self._pw.setAxisItems({'bottom': axis_date})

        self._pw.plot(x=list(self._xdict.keys()), y=self._data['close'].values, 
                pen='r', name='close', symbolBrush=(255, 0, 0))
        self._pw.plot(x=list(self._xdict.keys()), y=self._data['open'].values, 
                pen='g', name='open', symbolBrush=(0, 255,0))

        self._label_info = pg.TextItem()
        self._pw.addItem(self._label_info)
        self._pw.showGrid(x=True, y=True, alpha=0.5)

        self._v_line = pg.InfiniteLine(angle=90, movable=False)
        self._h_line = pg.InfiniteLine(angle=0, movable=False)
        self._pw.addItem(self._v_line, ignoreBounds=True)
        self._pw.addItem(self._h_line, ignoreBounds=True)
        
        self._view_box = self._pw.getPlotItem().getViewBox()
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
                        <p style='color:white'>收盘：{2}</p>".format(
                            self._data['trade_date'].iloc[index],
                            self._data['open'].iloc[index],
                            self._data['close'].iloc[index]))
                self._label_info.setPos(mousePoint.x(), mousePoint.y())

            self._v_line.setPos(mousePoint.x())
            self._h_line.setPos(mousePoint.y())



if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = KWidget()
    view.show()
    sys.exit(app.exec_())
