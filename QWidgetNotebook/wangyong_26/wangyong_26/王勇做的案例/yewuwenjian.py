from PyQt5.QtChart import QChartView, QLineSeries, QChart
from PyQt5.QtGui import QPainter, QFont
from PyQt5.QtWidgets import QWidget, QApplication, QFileDialog
from jiemian import Ui_MainWindow

class My_Main_window(Ui_MainWindow, QWidget):  # 这里继承 QWidget 是因为 QFileDialog 必须要有 QWidget 才能运行，没有会出错
    def setupBusi(self, MainWindow):

        # # 设置布局
        # self.verticalLayout.addWidget(self.label)
        # self.verticalLayout.addWidget(self.pushButton)

        # 设置连接事件
        self.pushButton.clicked.connect(self.plot_)
        self.pushButton_2.clicked.connect(self.pixSave)

        self.chart = QChart()
        self.view = QChartView(self.chart)
        self.series = QLineSeries(self.chart)  # 在QChart上实例化曲线
        self.verticalLayout.addWidget(self.view)
        self.view.show()

    def plot_(self):

        # 设置图片背景颜色，默认是白色背景；比如，还可以设置为：QChart.ChartThemeDark，QChart.ChartThemeLight，QChart.ChartThemeBlueCerulean
        # QChart.ChartThemeBrownSand，QChart.ChartThemeBlueNcs，QChart.ChartThemeHighContrast，QChart.ChartThemeBlueIcy等
        self.view.chart().setTheme(QChart.ChartThemeBlueCerulean)

        self.chart.setTitle("曲线图")  # 设置图题
        self.chart.setTitleFont(QFont('SansSerif', 20))  # 设置图题字体的类型和大小
        # self.view.chart().legend().hide()  # 隐藏图例
        self.chart.removeSeries(self.series)  # 清除QChart上的指定曲线
        self.series.append(0, 6)
        self.series.append(1, 7)
        self.series.append(2, 4)
        self.series.setName('单井生产数据')  # 设置每条数据曲线的名称
        self.view.chart().legend().setFont(QFont("Arial", 17))  # 设置数据曲线名称的字体类型和大小
        self.chart.addSeries(self.series)
        self.chart.createDefaultAxes()  # 创建默认轴
        self.chart.axisX().setTitleText('时间')  # 设置横坐标标题
        self.chart.axisX().setLabelsFont(QFont('SansSerif', 13)) # 设置横坐标刻度的字体类型和大小
        self.chart.axisX().setTitleFont(QFont('SansSerif', 15))  # 设置横坐标标题字体的类型和大小
        self.chart.axisY().setTitleText('产量')  # 设置横坐标标题
        self.chart.axisY().setLabelsFont(QFont('SansSerif', 13))  # 设置横坐标刻度的字体类型和大小
        self.chart.axisY().setTitleFont(QFont('SansSerif', 15))  # 设置横坐标标题字体的类型和大小

        # self.series.remove(0)  # 清除数据，但坐标框架等还在。
        # self.series.clear()  # 清除数据，但坐标框架等还在。

        self.view.setRenderHint(QPainter.Antialiasing)  # 抗锯齿
        # self.view.show()


    def pixSave(self):

        # 用 QFileDialog 获取保存文件的全部路径（包括文件名和后缀名）
        fileName2, ok2 = QFileDialog.getSaveFileName(self, "文件保存", "/", "图片文件 (*.png);;(*.jpeg)")

        """
        下面用 Pyqt5 的截屏方法，截取指定 QChartView 上显示的东西；缺点，若显示的图像变形，
        则截图也会变形！
        """
        screen = QApplication.primaryScreen()
        pix = screen.grabWindow(self.view.winId())
        pix.save(fileName2)

