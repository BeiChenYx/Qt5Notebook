import pyqtgraph as pg
import numpy as np

x = np.arange(1000)
y = np.random.normal(size=(3, 1000))

w = pg.GraphicsLayoutWidget()
p1 = w.addPlot(row=0, col=0)
p2 = w.addPlot(row=0, col=1)
v = w.addViewBox(row=1, col=0, colspan=2)

app = pg.QtGui.QApplication([])
app.exec()
