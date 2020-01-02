import sys
from enum import Enum

import sxtwl
from PyQt5 import QtWidgets, QtGui, QtCore


class DayType(Enum):
    MONTH_PRE = 0       # 上月剩余的天
    MONTH_NEXT = 1      # 下个月的天
    MONTH_CURRENT = 2   # 当月的天
    WEEK_END = 3        # 周末


class CalendarItem(QtWidgets.QWidget):

    def __init__(self, parent=None):
        super(CalendarItem, self).__init__(parent)
        self.day = '1'
        self.lunar = '初一'
        self.borderColor = QtGui.QColor(180, 180, 180)
        self.lunarColor = QtGui.QColor(55, 156, 238)
        self.currentTextColor = QtGui.QColor(0, 0, 0)
        self.selectTextColor = QtGui.QColor(255, 255, 255)
        self.hoverTextColor = QtGui.QColor(250, 250, 250)
        self.otherTextColor = QtGui.QColor(200, 200, 200)
        self.currentBgColor = QtGui.QColor(255, 255, 255)
        self.otherBgColor = QtGui.QColor(240, 240, 240)
        self.selectBgColor = QtGui.QColor(208,47,18)
        self.hoverBgColor = QtGui.QColor(204, 183, 180)
        self.hoverLunarColor = QtGui.QColor(250, 250, 250)
        self.selectLunarColor = QtGui.QColor(255, 255, 255)
        self.otherLunarColor = QtGui.QColor(200, 200, 200)
        self.currentLunarColor = QtGui.QColor(150, 150, 150)
        self.dayType = DayType.MONTH_CURRENT
        self.hover = False
        self.pressed = False
        self.select = False
        self.nowDate = QtCore.QDate.currentDate()

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
        elif self.hover:
            self.drawBgCurrent(painter, self.hoverBgColor)
        else:
            self.drawBgCurrent(painter, self.currentBgColor)
        self.drawDay(painter)
        self.drawLunar(painter)

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
        side = width if width < height else height
        font = QtGui.QFont()
        font.setPixelSize(side / 2.7)
        painter.setFont(font)
        dayRect = QtCore.QRect(0, 0, width, height / 1.7)
        painter.drawText(dayRect, QtCore.Qt.AlignHCenter | QtCore.Qt.AlignBottom, self.day)
        painter.restore()

    def drawBgCurrent(self, painter: QtGui.QPainter, color: QtGui.QColor):
        width = self.width()
        height = self.height()
        side = width if width < height else height
        painter.save()
        painter.setPen(QtCore.Qt.NoPen)
        painter.setBrush(color)
        painter.drawRect(self.rect())
        painter.restore()

    def drawLunar(self, painter: QtGui.QPainter):
        width = self.width()
        height = self.height()
        side = width if width < height else height
        painter.save()
        if self.select:
            color = self.selectTextColor
        elif self.hover:
            color = self.hoverTextColor
        else:
            color = self.lunarColor
        painter.setPen(color)
        font = QtGui.QFont()
        font.setPixelSize(side / 5)
        painter.setFont(font)
        lunarRect = QtCore.QRect(0, height / 2, width, height / 2) 
        painter.drawText(lunarRect, QtCore.Qt.AlignHCenter | QtCore.Qt.AlignBottom, self.lunar)
        painter.restore()

    def getLunarDay(self) -> str:
        Gan = ["甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"]
        Zhi = ["子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]
        ShX = ["鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"]
        numCn = ["零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"]
        jqmc = ["冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满",
                "芒种", "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"]
        ymc = ["十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十"]
        rmc = ["初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", "十一", "十二", "十三", "十四", "十五",
               "十六", "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"]
        lunar = sxtwl.Lunar()
        day = lunar.getDayBySolar(self.nowDate.year, self.nowDate.month, self.nowDate.day)



def ui():
    """ 显示UI界面，会阻塞线程，开启Qt事件循环 """
    app = QtWidgets.QApplication(sys.argv)
    ui = CalendarItem()
    ui.show()
    sys.exit(app.exec_())


def main():
    ui()


if __name__ == "__main__":
    main()

