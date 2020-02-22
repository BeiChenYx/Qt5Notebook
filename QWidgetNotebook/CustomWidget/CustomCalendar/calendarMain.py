import sys
import calendar

from PyQt5 import QtWidgets, QtGui, QtCore

from ui_calendar import Ui_calendarWidget
from calendarItem import CalendarItem


class CalendarWidget(QtWidgets.QWidget, Ui_calendarWidget):

    def __init__(self, parent=None):
        super(CalendarWidget, self).__init__(parent)
        self.setupUi(self)
        self.now = QtCore.QDate.currentDate()
        self.layout_body = QtWidgets.QGridLayout(self.widget_calendar)
        self.layout_body.setSpacing(2)
        self.layout_body.setContentsMargins(1, 1, 1, 1)
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
        self.dateEdit_y.setDate(self.now)
        self.dateEdit_m.setDate(self.now)
        self.initCalendar()

    def initCalendar(self, date=None):
        """ 初始化日历主体 """
<<<<<<< HEAD:QWidgetNotebook/CustomWidget/CustomCalendar/calendarMain.py
        # 清空布局内的控件
        item = None;
        while True:
            item = self.layout_body.takeAt(0)
            if item is None:
                break
            item.widget().hide()
            self.layout_body.removeItem(item)
            del item
        self.layout_body.update()

        row = 0
        week_map = {0: 1, 1: 2, 2: 3, 3: 4, 4: 5, 5: 6, 6: 0}
        now = self.now if date is None else date
        _, nums = calendar.monthrange(now.year(), now.month())
        week_day = [(i, calendar.weekday(now.year(), now.month(), i)) for i in range(1, nums+1)]
        for i, week in week_day:
            col = week_map[week]
            week_end = True if col == 6 or col == 0 else False
            item = CalendarItem(i, week_end)
            self.layout_body.addWidget(item, row, col)
            row = (row + 1) if col == 6 else row

    def initConnections(self):
        self.toolButton_py.clicked.connect(self.onToolButton_py)
        self.toolButton_ny.clicked.connect(self.onToolButton_ny)
        self.toolButton_pm.clicked.connect(self.onToolButton_pm)
        self.toolButton_nm.clicked.connect(self.onToolButton_nm)
        self.pushButton_now.clicked.connect(self.onPushButton_now)
        self.dateEdit_y.dateChanged.connect(self.onDateChanged_y)
        self.dateEdit_m.dateChanged.connect(self.onDateChanged_m)

    def onToolButton_py(self):
        now = self.dateEdit_y.date().addYears(-1)
        self.dateEdit_y.setDate(now)
        self.initCalendar(now)
        self.dateEdit_y.update()
        self.dateEdit_m.update()
        self.widget_calendar.update()

    def onToolButton_ny(self):
        now = self.dateEdit_y.date().addYears(1)
        self.dateEdit_y.setDate(now)
        self.initCalendar(now)
        self.dateEdit_y.update()
        self.dateEdit_m.update()
        self.widget_calendar.update()

    def onToolButton_pm(self):
        now = self.dateEdit_m.date()
        if now.month() == 1:
            self.onToolButton_py()
        self.dateEdit_m.setDate(now.addMonths(-1))
        self.initCalendar(now.addMonths(-1))
        self.dateEdit_m.update()
        self.dateEdit_y.update()
        self.widget_calendar.update()

    def onToolButton_nm(self):
        now = self.dateEdit_m.date()
        if now.month() == 12:
            self.onToolButton_ny()
        self.dateEdit_m.setDate(now.addMonths(1))
        self.initCalendar(now.addMonths(1))
        self.dateEdit_m.update()
        self.dateEdit_y.update()
        self.widget_calendar.update()

    def onPushButton_now(self):
        self.dateEdit_y.setDate(self.now)
        self.dateEdit_m.setDate(self.now)
        self.initCalendar(self.now)
        self.dateEdit_m.update()
        self.dateEdit_y.update()
        self.widget_calendar.update()

    def onDateChanged_y(self, date):
        pass
    def onDateChanged_m(self, date):
        pass


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
