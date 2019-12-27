import sys
import time
import datetime
from random import randint

import pandas as pd

from PyQt5.QtChart import QChartView, QChart, QLineSeries, QLegend,\
    QCategoryAxis, QCandlestickSeries, QCandlestickSet
from PyQt5.QtCore import Qt, QPointF, QRectF, QPoint
from PyQt5.QtGui import QPainter, QPen, QColor
from PyQt5.QtWidgets import QApplication, QGraphicsLineItem, QWidget, \
    QHBoxLayout, QLabel, QVBoxLayout, QGridLayout, QGraphicsProxyWidget,\
    QMenu, QAction, QSplitter, QGraphicsScene

from candlestick import CandlestickChartView


class VolChartView(QChartView):
    """" 用来显示柱形图的视图 """
    
    def __init__(self):
        super(VolChartView, self).__init__()
        self.resize(800, 300)

        self.initUi()

    def initUi(self):
        self._chart = QChart(title='红绿柱形图')
        self._chart.setAnimationOptions(QChart.SeriesAnimations)



class MainWidget(QWidget):
    """
    主界面，用来包含柱形图和蜡烛图，
    使用QGraphicscene来管理
    """

    def __init__(self):
        super(MainWidget, self).__init__()
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setWindowTitle('多图表展示')

        self.initUi()
    
    def initUi(self):
        self.candlestickChart = CandlestickChartView() 
        self.volChart = VolChartView()

        self._splitter = QSplitter(Qt.Vertical, self)
        self._splitter.addWidget(self.candlestickChart)
        self._splitter.addWidget(self.volChart)
        self._splitter.setStretchFactor(7, 3)

        vlayout = QVBoxLayout(self)
        vlayout.addWidget(self._splitter)
        self.setLayout(vlayout)

        



if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = MainWidget()
    view.show()
    sys.exit(app.exec_())