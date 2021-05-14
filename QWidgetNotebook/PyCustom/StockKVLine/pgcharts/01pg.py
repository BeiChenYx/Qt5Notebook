import pdb
import pyqtgraph as pg
import numpy as np

x = np.random.random(50)
y = np.random.random(8)

def pg_plot():
    """ 最简单的使用plot()方法直接绘制图形 """
    app = pg.QtGui.QApplication([])
    plot = pg.plot(title="yangxing的PyQt笔记")
    plot.plot(x)
    plot.plot(y, pen='r')
    app.exec_()

# pg_plot()


def pg_window_addplot():
    """
    通过GraphicsWindow方法创建空的图形窗口, 然后addPlot()添加图形
    """
    win = pg.GraphicsWindow(title='yangxing的PyQt笔记')
    plot = win.addPlot(title='窗口绘制图形')
    plot.plot(x)
    pg.QtGui.QGuiApplication.exec_()

# pg_window_addplot()

def pg_window_addplot_mul():
    """
    通过addPlot继续添加子图形
    """
    win = pg.GraphicsWindow(title='yangxing的PyQt笔记')
    x_list = np.random.randint(1000, 10000, 50)
    plot = win.addPlot(title='窗口绘制图形', row=1, col=1, rowspan=1, colspan=2)
    print(type(plot.getAxis('left')))
    y_plot1 = plot.getAxis('left')
    plot2 = win.addPlot(title='A1', row=2, col=1, rowspan=1, colspan=1)
    y_plot2 = plot2.getAxis('left')
    plot3 = win.addPlot(title='A2', row=2, col=2, rowspan=1, colspan=1)
    # 对齐Y轴, 可以获取最大长度字符串的内容计算字体的实际像素，得出最大的长度
    y_plot1.setWidth(50)
    y_plot2.setWidth(50)
    plot.plot(x_list)
    plot2.plot(x)
    plot3.plot(y)
    pg.QtGui.QGuiApplication.exec_()

pg_window_addplot_mul()

def pg_graphicsLayou_plot():
    """
    通过GraphicsLayout图形层方法绘制图形的过程相较于上面的稍微复杂:
    1. 通过实例化pyqtgraph的GraphicsView()方法创建一个图形视图;
    2. 通过实例化pyqtgraph的GraphicsLayout()方法, 创建一个图形层;
    3. 设置图形视图的中心层为第二步创建的图形层，并设置显示图形视图;
    4. 最后使用图形层的addPlot()方法添加一个图形, 再将使用图形的plot()方法将图形绘制出来;
    """
    app = pg.QtGui.QApplication([])
    view = pg.GraphicsView()
    layout = pg.GraphicsLayout()
    view.setCentralItem(layout )
    view.show()
    p1 = layout.addPlot(title='yangxing的PyQt笔记')
    p1.plot(x)
    app.exec_()

# pg_graphicsLayou_plot()

def pg_plotwidget_plot():
    """
    使用pyqtgraph的PlotWidget方法绘制图形与直接使用plot()方法相似
    """
    # print('axis: \n')
    # pdb.set_trace()
    app = pg.QtGui.QApplication([])
    pw = pg.PlotWidget(title='yangxing的PyQt笔记')
    print(type(pw.getPlotItem().getAxis('bottom')))
    b_axis =pw.getPlotItem().getAxis('left')
    b_axis.setGrid(200);
    b_axis.setLogMode(True)
    b_axis.setWidth(100)
    pw.plot(x)
    pw.show()
    app.exec_()

# pg_plotwidget_plot()

def pg_plotitem_plot():
    app = pg.QtGui.QApplication([])
    xx = pg.GraphicsView()
    plot = pg.PlotItem(title='yangxing的PyQt笔记')
    plot.plot(x)
    xx.setCentralWidget(plot)
    xx.show()
    app.exec_()

# pg_plotitem_plot()


    







