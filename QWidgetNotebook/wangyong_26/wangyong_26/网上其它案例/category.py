import sys

from PyQt5.QtChart import QChartView, QChart, QLineSeries, QLegend,\
    QCategoryAxis
from PyQt5.QtCore import Qt, QPointF, QRectF, QPoint
from PyQt5.QtGui import QPainter, QPen
from PyQt5.QtWidgets import QApplication, QGraphicsLineItem, QWidget, \
    QHBoxLayout, QLabel, QVBoxLayout, QGraphicsProxyWidget, QMenu, QAction



class ToolTipItem(QWidget):

    def __init__(self, color, text, parent=None):
        super(ToolTipItem, self).__init__(parent)
        layout = QHBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        
        # 在Item前面画了一个圆 直径 12
        clabel = QLabel(self)
        clabel.setMinimumSize(12, 12)
        clabel.setMaximumSize(12, 12)
        clabel.setStyleSheet("border-radius:6px;background: rgba(%s,%s,%s,%s);" % (
            color.red(), color.green(), color.blue(), color.alpha()))
        layout.addWidget(clabel)
        self.textLabel = QLabel(text, self, styleSheet="color:white;")
        layout.addWidget(self.textLabel)

    def setText(self, text):
        self.textLabel.setText(text)


class ToolTipWidget(QWidget):

    Cache = {}

    def __init__(self, *args, **kwargs):
        super(ToolTipWidget, self).__init__(*args, **kwargs)
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setStyleSheet(
            "ToolTipWidget{background: rgba(50, 50, 50, 100);}")
        layout = QVBoxLayout(self)
        self.titleLabel = QLabel(self, styleSheet="color:white;")
        layout.addWidget(self.titleLabel)

    def updateUi(self, title, points):
        self.titleLabel.setText(title)
        for serie, point in points:
            if serie not in self.Cache:
                item = ToolTipItem(
                    serie.color(),
                    (serie.name() or "-") + ":" + str(point.y()), self)
                self.layout().addWidget(item)
                self.Cache[serie] = item
            else:
                self.Cache[serie].setText(
                    (serie.name() or "-") + ":" + str(point.y()))
            self.Cache[serie].setVisible(serie.isVisible())  # 隐藏那些不可用的项
        self.adjustSize()  # 调整大小, adjustSize() 自动调整部件的大小


class GraphicsProxyWidget(QGraphicsProxyWidget):

    def __init__(self, *args, **kwargs):
        super(GraphicsProxyWidget, self).__init__(*args, **kwargs)
        self.setZValue(999)
        self.tipWidget = ToolTipWidget()
        self.setWidget(self.tipWidget)
        self.hide()

    def width(self):
        return self.size().width()

    def height(self):
        return self.size().height()

    def show(self, title, points, pos):
        self.setGeometry(QRectF(pos, self.size()))
        self.tipWidget.updateUi(title, points)
        super(GraphicsProxyWidget, self).show()


class ChartView(QChartView):

    def __init__(self, *args, **kwargs):
        super(ChartView, self).__init__(*args, **kwargs)
        self.resize(800, 600)
        self.setRenderHint(QPainter.Antialiasing)  # 抗锯齿
        # 自定义x轴label
        self.category = ["周一", "周二", "周三", "周四", "周五", "周六", "周日"]
        self.initChart()
        # 提示widget
        self.toolTipWidget = GraphicsProxyWidget(self._chart)

        # line
        self.lineItem = QGraphicsLineItem(self._chart)
        pen = QPen(Qt.gray)
        pen.setWidth(1)

        self.lineItem.setPen(pen)
        self.lineItem.setZValue(996)
        self.lineItem.hide()

        self.lineItemX = QGraphicsLineItem(self._chart)
        penx = QPen(Qt.gray)
        penx.setWidth(1)
        self.lineItemX.setPen(penx)
        self.lineItemX.setZValue(997)
        self.lineItemX.hide()
        # 一些固定计算，减少mouseMoveEvent中的计算量
        # 获取x和y轴的最小最大值
        axisX, axisY = self._chart.axisX(), self._chart.axisY()
        self.min_x, self.max_x = axisX.min(), axisX.max()
        self.min_y, self.max_y = axisY.min(), axisY.max()

        # 右键菜单按钮
        self.editBtn = QAction('编辑', self)
        self.delBtn = QAction('删除', self)
        self.editBtn.triggered.connect(self.onRightMenuEdit)
        self.delBtn.triggered.connect(self.onRightMenuDel)

    def initChart(self):
        self._chart = QChart(title="自定义x轴")
        # 设置Series动画
        self._chart.setAnimationOptions(QChart.SeriesAnimations)
        values = [120, 132, 101, 134, 90, 230, 210]
        series = QLineSeries(self._chart)
        for i, v in enumerate(values):
            series.append(i, v)
        series.setName("邮件营销")
        series.setPointsVisible(True)
        self._chart.addSeries(series)

        self._chart.createDefaultAxes()
        axisX = self._chart.axisX()  # x轴
        axisX.setTickCount(7)  # x轴设置7个刻度
        axisX.setGridLineVisible(False)  # 隐藏从x轴往上的线条
        axisX.hide()
        axisY = self._chart.axisY()
        axisY.setTickCount(7)  # y轴设置7个刻度
        axisY.setRange(0, 300)  # 设置y轴范围

        axis_x = QCategoryAxis(
            self._chart, labelsPosition=QCategoryAxis.AxisLabelsPositionOnValue)
        axisX = self._chart.axisX()  # x轴
        min_x = axisX.min()
        max_x = axisX.max()
        step = (max_x - min_x) / (7 - 1)  # 7个tick
        for i in range(0, 7):
            axis_x.append(self.category[i], min_x + i * step)

        self._chart.addAxis(axis_x, Qt.AlignBottom);
        series.attachAxis(axis_x)

        # chart的图例
        legend = self._chart.legend()
        # 设置图例由Series来决定样式
        legend.setMarkerShape(QLegend.MarkerShapeFromSeries)

        self.setChart(self._chart)

    def contextMenuEvent(self, event):
        """ 添加右键菜单 """
        menu = QMenu();
        menu.setFixedWidth(120);
        menu.addAction(self.editBtn)
        menu.addAction(self.delBtn)
        menu.exec(event.globalPos())

    def onRightMenuEdit(self):
        print('右键菜单编辑按下...')
    def onRightMenuDel(self):
        print('右键菜单删除按下...')

    def mouseMoveEvent(self, event):
        super(ChartView, self).mouseMoveEvent(event)
        pos = event.pos()
        # 把鼠标位置所在点转换为对应的xy值
        x = self._chart.mapToValue(pos).x()
        y = self._chart.mapToValue(pos).y()
        index = round((x - self.min_x) / self.step_x)
        points = [(serie, serie.at(index))
                  for serie in self._chart.series()
                  if self.min_x <= x <= self.max_x and
                  self.min_y <= y <= self.max_y]
        if points:
            # 根据鼠标的点获取对应曲线的坐标点，
            # pos_x = self._chart.mapToPosition(
            #     QPointF(index * self.step_x + self.min_x, self.min_y))
            # 设置鼠标的垂直线
            self.lineItem.setLine(pos.x(), self.point_top.y(),
                                  pos.x(), self.point_bottom.y())
            self.lineItem.show()

            # 设置鼠标的水平线
            self.lineItemX.setLine(self.point_top.x(), pos.y(),
                                  self.point_right_top.x(), pos.y())
            self.lineItemX.show()

            try:
                title = self.category[index]
            except:
                title = ""
            t_width = self.toolTipWidget.width()
            t_height = self.toolTipWidget.height()
            # 如果鼠标位置离右侧的距离小于tip宽度
            x = pos.x() - t_width if self.width() - \
                pos.x() - 20 < t_width else pos.x()
            # 如果鼠标位置离底部的高度小于tip高度
            y = pos.y() - t_height if self.height() - \
                pos.y() - 20 < t_height else pos.y()
            self.toolTipWidget.show(
                title, points, QPoint(x, y))
        else:
            self.toolTipWidget.hide()
            self.lineItem.hide()
            self.lineItemX.hide()

    def resizeEvent(self, event):
        super(ChartView, self).resizeEvent(event)
        # 当窗口大小改变时需要重新计算
        # 坐标系中左上角顶点
        self.point_top = self._chart.mapToPosition(
            QPointF(self.min_x, self.max_y))
        # 坐标右上点
        self.point_right_top = self._chart.mapToPosition(
            QPointF(self.max_x, self.max_y)
        )
        # 坐标右下点
        self.point_right_bottom = self._chart.mapToPosition(
            QPointF(self.max_x, self.min_y)
        )
        # 坐标原点坐标
        self.point_bottom = self._chart.mapToPosition(
            QPointF(self.min_x, self.min_y))
        self.step_x = (self.max_x - self.min_x) / \
            (self._chart.axisX().tickCount() - 1)



if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = ChartView()
    view.show()
    sys.exit(app.exec_())
        