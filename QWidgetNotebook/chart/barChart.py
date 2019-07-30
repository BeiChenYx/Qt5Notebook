import sys
import random

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtChart import *


class DisplayChart(QtWidgets.QWidget):
    """ 显示曲线 """
    def __init__(self):
        super(DisplayChart, self).__init__()
        self.initUi()
        self.initConnections()
        self.onBtnAdd()
    
    def initUi(self):
        self.months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun']
        self.resize(800, 700)

        # 初始化图表
        self._series = QBarSeries()

        self._chart = QChart()
        self._chart.addSeries(self._series)
        self._chart.setTitle("Simple barchart example")
        self._chart.setAnimationOptions(QChart.SeriesAnimations)

        self._axisX = QBarCategoryAxis()
        self._axisX.append(self.months)

        self._axisY = QValueAxis()
        self._axisY.setRange(-10, 15)

        self._chart.addAxis(self._axisY, QtCore.Qt.AlignLeft)
        self._chart.addAxis(self._axisX, QtCore.Qt.AlignBottom)
        self._series.attachAxis(self._axisX)
        self._series.attachAxis(self._axisY)
        self._chart.legend().setVisible(True)
        self._chart.legend().setAlignment(QtCore.Qt.AlignBottom)

        self._chartVew = QChartView(self._chart)
        self._chartVew.setRenderHint(QtGui.QPainter.Antialiasing)

        layout = QtWidgets.QVBoxLayout(self) 
        layout.addWidget(self._chartVew)
        self._btnAdd = QtWidgets.QPushButton('update')
        layout.addWidget(self._btnAdd)
        self.setLayout(layout)

    def initConnections(self):
        self._btnAdd.clicked.connect(self.onBtnAdd)

    def onBtnAdd(self):
        self._series.clear()
        barSetRed = QBarSet('Red')
        # barSetRed.setColor(QtCore.Qt.red)
        for vol in self.months:
            v = random.randint(-10, 14)
            if v < 0:
                barSetRed.setColor(QtCore.Qt.green)
            else:
                barSetRed.setColor(QtCore.Qt.red)
            barSetRed.append(v)
            self._series.append(barSetRed)


if __name__ == "__main__":
   app = QtWidgets.QApplication(sys.argv)
   view = DisplayChart()
   view.show()
   sys.exit(app.exec_())
