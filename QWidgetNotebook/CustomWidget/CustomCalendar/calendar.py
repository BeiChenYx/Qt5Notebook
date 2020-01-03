import sys

from PyQt5 import QtWidgets, QtGui, QtCore

from ui_calendar import Ui_calendarWidget
from calendarItem import CalendarItem


class CalendarWidget(QtWidgets.QWidget, Ui_calendarWidget):

    def __init__(self, parent=None):
        super(CalendarWidget, self).__init__(parent)
        self.setupUi(self)
        self.dayItems = list()
        self.initUi()
        self.initConnections()

    def initUi(self):
        fontId = QtGui.QFontDatabase.addApplicationFont(":/fontawesome-webfont.ttf")
        fontName = QtGui.QFontDatabase.applicationFontFamilies(fontId)
        if(len(fontName) > 0):
            self.iconFont = QtGui.QFont(fontName[0])
            self.iconFont.setPixelSize(15)
        else:
            print('Load fontawesome-webfont.ttf error.')
        self.toolButton_py.setFont(self.iconFont)
        self.toolButton_ny.setFont(self.iconFont)
        self.toolButton_pm.setFont(self.iconFont)
        self.toolButton_nm.setFont(self.iconFont)
        self.toolButton_py.setText(chr(0xf060))
        self.toolButton_ny.setText(chr(0xf061))
        self.toolButton_pm.setText(chr(0xf060))
        self.toolButton_nm.setText(chr(0xf061))
        self.dateEdit_y.setDate(QtCore.QDate.currentDate())
        self.dateEdit_m.setDate(QtCore.QDate.currentDate())
        self.initCalendar()

    def initCalendar(self):
        """ 初始化日历主体 """
        layoutBody = QtWidgets.QGridLayout(self.widget_calendar)
        layoutBody.setContentsMargins(1, 1, 1, 1)
        layoutBody.setHorizontalSpacing(0)
        layoutBody.setVerticalSpacing(0)
        for i in range(0, 42):
            dayItem = CalendarItem(self)
            layoutBody.addWidget(dayItem, i/7, i%7)
            self.dayItems.append(dayItem)

    def initConnections(self):
        self.toolButton_py.clicked.connect(self.onToolButton_py)
        self.toolButton_ny.clicked.connect(self.onToolButton_ny)
        self.toolButton_pm.clicked.connect(self.onToolButton_pm)
        self.toolButton_nm.clicked.connect(self.onToolButton_nm)
        self.pushButton_now.clicked.connect(self.onPushButton_now)
        self.dateEdit_y.dateChanged.connect(self.onDateChanged_y)
        self.dateEdit_m.dateChanged.connect(self.onDateChanged_m)

    def onToolButton_py(self):
        now = self.dateEdit_y.date()
        self.dateEdit_y.setDate(now.addYears(-1))
    def onToolButton_ny(self):
        now = self.dateEdit_y.date()
        self.dateEdit_y.setDate(now.addYears(1))
    def onToolButton_pm(self):
        now = self.dateEdit_m.date()
        if now.month() == 1:
            self.onToolButton_py()
        self.dateEdit_m.setDate(now.addMonths(-1))
    def onToolButton_nm(self):
        now = self.dateEdit_m.date()
        if now.month() == 12:
            self.onToolButton_ny()
        self.dateEdit_m.setDate(now.addMonths(1))
    def onPushButton_now(self):
        self.dateEdit_y.setDate(QtCore.QDate.currentDate())
        self.dateEdit_m.setDate(QtCore.QDate.currentDate())
    def onDateChanged_y(self, date):
        pass
    def onDateChanged_m(self, date):
        pass

    def drawWidget(self, widget: QtWidgets.QWidget, dayNum):
        """ 绘制某个Widget，绘制日期和农历 """
        width = widget.width()
        height = widget.height()
        side = width if width < height else height
        painter = QtGui.QPainter(widget)
        painter.save()
        font = QtGui.QFont()
        font.setPixelSize(side / 2.7)
        painter.setFont(font)
        dayRect = QtCore.QRect(0, 0, width, height / 1.7)
        painter.drawText(dayRect, QtCore.Qt.AlignHCenter | QtCore.Qt.AlignBottom, str(dayNum))
        painter.restore()

        

def ui():
    """ 显示UI界面，会阻塞线程，开启Qt事件循环 """
    app = QtWidgets.QApplication(sys.argv)
    ui = CalendarWidget()
    ui.show()
    sys.exit(app.exec_())


def main():
    ui()


if __name__ == "__main__":
    main()
