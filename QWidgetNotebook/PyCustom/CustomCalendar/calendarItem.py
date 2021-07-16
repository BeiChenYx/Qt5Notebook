import sys
from enum import Enum

from PyQt5 import QtWidgets, QtGui, QtCore


class DayType(Enum):
    MONTH_PRE = 0       # 上月剩余的天
    MONTH_NEXT = 1      # 下个月的天
    MONTH_CURRENT = 2   # 当月的天
    WEEK_END = 3        # 周末


class CalendarItem(QtWidgets.QWidget):

    def __init__(self, day: int = 1,
                 week_end=False,
                 is_now=False,
                 is_trade=True,
                 parent=None):
        """
        :day: 日期
        :week_end: 是否是周末
        :selected: 是否被选中
        """
        super(CalendarItem, self).__init__(parent)
        self.day = str(day)
        self.week_end = week_end
        self.borderColor = QtGui.QColor(180, 180, 180)
        self.currentTextColor = QtGui.QColor(0, 0, 0)
        self.selectTextColor = QtGui.QColor(255, 255, 255)
        self.hoverTextColor = QtGui.QColor(250, 250, 250)
        self.otherTextColor = QtGui.QColor(200, 200, 200)
        self.currentBgColor = QtGui.QColor(255, 255, 255)
        self.otherBgColor = QtGui.QColor(240, 240, 240)
        self.selectBgColor = QtGui.QColor(194, 194, 194)
        self.hoverBgColor = QtGui.QColor(204, 183, 180)
        self.nowBgColor = QtGui.QColor(194, 194, 194)
        self.tradeBgColor = QtGui.QColor(255, 0, 0)
        self.notradeBgColor = QtGui.QColor(194, 194, 194)
        self.dayType = DayType.MONTH_CURRENT
        self.hover = False
        self.pressed = False
        self.select = False
        self.is_now = is_now
        self.is_trade = is_trade

    def enterEvent(self, event):
        self.hover = True
        self.update()

    def leaveEvent(self, event):
        self.hover = False
        self.update()

    def mousePressEvent(self, event):
        self.pressed = True
        self.select = not self.select
        self.update()

    def mouseReleaseEvent(self, event):
        self.pressed = False
        self.update()

    def paintEvent(self, event):
        width = self.width()
        height = self.height()
        side = width if width < height else height
        painter = QtGui.QPainter(self)
        self.drawBg(painter)
        if self.select:
            self.drawBgCurrent(painter, self.selectBgColor)
        elif self.is_now:
            self.drawBgCurrent(painter, self.nowBgColor)
        elif self.hover:
            self.drawBgCurrent(painter, self.hoverBgColor)
        else:
            self.drawBgCurrent(painter, self.currentBgColor)
        self.drawDay(painter)
        self.draw_trade(painter)

    def drawBg(self, painter: QtGui.QPainter):
        painter.save()
        if self.dayType == DayType.MONTH_PRE or self.dayType == DayType.MONTH_NEXT:
            bgColor = self.otherBgColor
        else:
            bgColor = self.selectBgColor
        painter.setPen(self.borderColor)
        painter.setBrush(bgColor)
        painter.drawRect(self.rect())
        painter.restore()

    def drawDay(self, painter: QtGui.QPainter):
        painter.save()
        width = self.width()
        height = self.height()
        side = min(width, height)
        if self.select:
            color = self.selectTextColor
        elif self.hover:
            color = self.hoverTextColor
        font = QtGui.QFont()
        font.setPixelSize(int(side * 2 / 3))
        painter.setFont(font)
        if self.week_end:
            painter.setPen(QtGui.QColor(255, 0, 0))
        rect = QtCore.QRect(0, 0, width, height)
        painter.drawText(rect, QtCore.Qt.AlignCenter, self.day)
        painter.restore()

    def draw_trade(self, painter: QtGui.QPainter):
        painter.save()
        width = self.width()
        painter.setBrush(self.tradeBgColor if self.is_trade else self.notradeBgColor)
        painter.drawEllipse(width - 12, 2, 10, 10)
        painter.restore()

    def drawBgCurrent(self, painter: QtGui.QPainter, color: QtGui.QColor):
        painter.save()
        painter.setPen(QtCore.Qt.NoPen)
        painter.setBrush(color)
        painter.drawRect(self.rect())
        painter.restore()


def main():
    """ 显示UI界面，会阻塞线程，开启Qt事件循环 """
    app = QtWidgets.QApplication(sys.argv)
    ui = CalendarItem()
    ui.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()

