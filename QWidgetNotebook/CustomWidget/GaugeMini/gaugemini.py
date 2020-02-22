import sys
import math
from enum import  Enum

from PyQt5 import QtWidgets, QtGui, QtCore


class PointerStyle(Enum):
    PointerStyle_Line = 0                           # 线条指示器
    PointerStyle_Indicator = 1                      # 指针指示器
    PointerStyle_IndicatorR = 2                     # 圆角指示器
    PointerStyle_Triangle = 3                       # 三角形指示器


class GaugeMini(QtWidgets.QWidget):
    """ 绘制计量表 """
    value_changed = QtCore.pyqtSignal(float)

    def __init__(self, parent=None):
        super(GaugeMini, self).__init__(parent)
        self.min_value = -100.0                                 # 最小值
        self.max_value = 100.0                                  # 最大值
        self.value = 0.0                                        # 目标值
        self.precision = 1                                      # 精确度, 小数点后几位

        self.step = 20                                          # 刻度数量
        self.start_angle = 45                                   # 开始旋转角度
        self.end_angle = 45                                     # 结束旋转角度

        self.border_color = QtGui.QColor(0, 0, 0)               # 边框颜色
        self.bg_color = QtGui.QColor(50, 50, 50)                # 背景颜色
        self.text_color = QtGui.QColor(0, 0, 0)                 # 文字颜色
        self.percent_color = QtGui.QColor(100, 184, 255)        # 进度颜色

        self.double_percent = True                              # 是否对半等分范围值
        self.show_value = True                                  # 是否显示当前值
        self.pointer_style = PointerStyle.PointerStyle_Line     # 指针样式
        self.pressed = False

        self.setFont(QtGui.QFont("Arial", 5))

    def mousePressEvent(self, e: QtGui.QMouseEvent):
        self.pressed = True
        self.set_pressed_value(e.pos())

    def mouseReleaseEvent(self, e: QtGui.QMouseEvent):
        self.pressed = False

    def mouseMoveEvent(self, e: QtGui.QMouseEvent):
        if self.pressed:
            self.set_pressed_value(e.pos())

    def paintEvent(self, e: QtGui.QPaintEvent):
        width = self.width()
        height = self.height()
        side = min(width, height)

        # 绘制准备工作，启用反锯齿，平移坐标轴中心，等比例缩放
        painter = QtGui.QPainter(self)
        painter.setRenderHints(QtGui.QPainter.Antialiasing | QtGui.QPainter.TextAntialiasing)
        painter.translate(width / 2, height / 2)
        painter.scale(side / 200.0, side / 200.0)

        # 绘制刻度线
        self.draw_scale(painter)
        # 绘制背景圆
        self.draw_bg_circle(painter)
        # 绘制中心圆
        self.draw_center_circle(painter)

        # 根据指示器形状绘制指示器
        if self.pointer_style == PointerStyle.PointerStyle_Line:
            self.draw_pointer_line(painter)
        elif self.pointer_style == PointerStyle.PointerStyle_Indicator:
            self.draw_pointer_indicator(painter)
        elif self.pointer_style == PointerStyle.PointerStyle_IndicatorR:
            self.draw_pointer_indicator_r(painter)
        else:
            self.draw_pointer_triangle(painter)

        # 绘制当前值
        self.draw_value(painter)

    def draw_scale(self, painter: QtGui.QPainter):
        """ 绘制刻度线 """
        radius = 96
        offset = 10
        painter.save()

        painter.rotate(self.start_angle)
        angle_step = (360 - self.start_angle - self.end_angle) / self.step
        deg_rotate = (360 - self.start_angle - self.end_angle) / (self.max_value - self.min_value) * (self.value - self.min_value)
        pen = QtGui.QPen()
        pen.setWidth(5)
        pen.setCapStyle(QtCore.Qt.RoundCap)

        rotate = 0.0
        for i in range(self.step+1):
            if self.double_percent:
                right = i >= (self.step / 2)
                if right:
                    pen.setColor(self.percent_color if rotate <= deg_rotate else self.text_color)
                else:
                    pen.setColor(self.text_color if rotate < deg_rotate else self.percent_color)

                # 矫正左侧值为最小值时中间第一格颜色
                if self.value == self.min_value and not right:
                    pen.setColor(self.percent_color)

                # 矫正左侧值时中间格颜色
                if self.value <= ((self.max_value - self.min_value) / 2 + self.min_value) \
                    and i == self.step / 2:
                    pen.setColor(self.percent_color)

                # 矫正中间值中间格颜色
                if self.value == ((self.max_value - self.min_value) / 2 + self.min_value):
                    pen.setColor(self.text_color)
            else:
                if rotate < deg_rotate:
                    pen.setColor(self.percent_color)
                else:
                    pen.setColor(self.text_color)

                # 矫正左侧值为最小值时第一格颜色
                if self.value == self.min_value:
                    pen.setColor(self.text_color)

            painter.setPen(pen)
            painter.drawLine(0, radius - offset, 0, radius)
            painter.rotate(angle_step)
            rotate += angle_step

        painter.restore()

    def draw_bg_circle(self, painter: QtGui.QPainter):
        """ 绘制背景圆 """
        radius = 75
        painter.save()

        pen = QtGui.QPen()
        pen.setWidth(5)
        pen.setColor(self.border_color)

        painter.setPen(pen)
        painter.setBrush(self.bg_color)
        painter.drawEllipse(-radius, -radius, radius * 2, radius * 2)

        painter.restore()

    def draw_center_circle(self, painter: QtGui.QPainter):
        """ 绘制中心圆 """
        radius = 15
        painter.save()

        color = self.percent_color
        if self.double_percent:
            center = (self.value == (self.max_value - self.min_value) / 2 + self.min_value)
            color = self.border_color if center else self.percent_color

        painter.setPen(QtCore.Qt.NoPen)
        painter.setBrush(color)
        painter.drawEllipse(-radius, -radius, radius * 2, radius * 2)

        painter.restore()

    def draw_pointer_line(self, painter: QtGui.QPainter):
        """ 绘制指示器, 样式line """
        radius = 62
        painter.save()

        painter.rotate(self.start_angle)
        deg_rotate = (360 - self.start_angle - self.end_angle) / (self.max_value - self.min_value) * (self.value - self.min_value)
        painter.rotate(deg_rotate)

        color = self.percent_color
        if self.double_percent:
            center = (self.value == (self.max_value - self.min_value) / 2 + self.min_value)
            color = self.border_color if center else self.percent_color

        painter.setPen(QtGui.QPen(color, 10, QtCore.Qt.SolidLine, QtCore.Qt.RoundCap))
        painter.drawLine(0, 0, 0, radius)

        painter.restore()

    def draw_pointer_indicator(self, painter: QtGui.QPainter):
        """ 绘制指示器, 样式indicator """
        radius = 62
        offset = 8
        painter.save()
        painter.setOpacity(0.8)

        # QPolygon 类提供了一个使用整数精度的点向量，QPoint的集合
        pts = QtGui.QPolygon()
        pts.setPoints(3, -offset, 0, offset, 0, 0, radius)

        painter.rotate(self.start_angle)
        deg_rotate = (360 - self.start_angle - self.end_angle) / (self.max_value - self.min_value) * (self.value - self.min_value)
        painter.rotate(deg_rotate)

        color = self.percent_color
        if self.double_percent:
            center = (self.value == (self.max_value - self.min_value) / 2 + self.min_value)
            color = self.border_color if center else self.percent_color

        painter.setPen(color)
        painter.setBrush(color)
        painter.drawConvexPolygon(pts)

        painter.restore()

    def draw_pointer_indicator_r(self, painter: QtGui.QPainter):
        """ 绘制指示器, 样式indicator_r """
        radius = 62
        offset = 8
        painter.save()
        painter.setOpacity(0.6)

        # QPolygon 类提供了一个使用整数精度的点向量，QPoint的集合
        pts = QtGui.QPolygon()
        pts.setPoints(3, -offset, 0, offset, 0, 0, radius)

        painter.rotate(self.start_angle)
        deg_rotate = (360 - self.start_angle - self.end_angle) / (self.max_value - self.min_value) * (self.value - self.min_value)
        painter.rotate(deg_rotate)

        color = self.percent_color
        if self.double_percent:
            center = (self.value == (self.max_value - self.min_value) / 2 + self.min_value)
            color = self.border_color if center else self.percent_color

        pen = QtGui.QPen()
        pen.setColor(pen)
        painter.setPen(pen)
        painter.setBrush(color)
        painter.drawConvexPolygon(pts)

        # 增加绘制圆角直线，与之前三角形重叠，形成圆角指针
        pen.setCapStyle(QtCore.Qt.RoundCap)
        pen.setWidth(offset - 1)
        painter.setPen(pen)
        painter.drawLine(0, 0, 0, radius)

        painter.restore()

    def draw_pointer_triangle(self, painter: QtGui.QPainter):
        """ 绘制指示器, 样式triangle """
        radius = 25
        offset = 40
        painter.save()

        # QPolygon 类提供了一个使用整数精度的点向量，QPoint的集合
        pts = QtGui.QPolygon()
        pts.setPoints(3, -radius / 2, offset, radius / 2, offset, 0, radius + offset)

        painter.rotate(self.start_angle)
        deg_rotate = (360 - self.start_angle - self.end_angle) / (self.max_value - self.min_value) * (self.value - self.min_value)
        painter.rotate(deg_rotate)

        color = self.percent_color
        if self.double_percent:
            center = (self.value == (self.max_value - self.min_value) / 2 + self.min_value)
            color = self.border_color if center else self.percent_color

        painter.setPen(color)
        painter.setBrush(color)
        painter.drawConvexPolygon(pts)

        painter.restore()

    def draw_value(self, painter: QtGui.QPainter):
        """ 绘制当前值 """
        if not self.show_value:
            return

        radius = 100
        painter.save()

        painter.setPen(QtCore.Qt.white)
        font = QtGui.QFont()
        font.setPixelSize(radius * 0.3)
        painter.setFont(font)
        rect = QtCore.QRect(-radius, -radius, radius * 2, radius * 1.65)
        painter.drawText(rect, QtCore.Qt.AlignHCenter | QtCore.Qt.AlignBottom, str(round(self.value, self.precision)))

        painter.restore()

    def set_pressed_value(self, pressed_point: QtCore.QPointF):
        """ 根据鼠标按下的坐标设置当前按下坐标处的值 """
        # 计算总角度
        length = 360 - self.start_angle -self.end_angle

        # 计算最近的刻度
        point = pressed_point - self.rect().center()
        # 余切角度
        theta = -math.atan2(-point.x(), -point.y()) * 180 / math.pi
        theta = theta + length / 2

        # 计算每一个角度对应值移动多少
        increment = (self.max_value - self.min_value) / length
        current_value = (theta * increment) + self.min_value

        # 过滤圆弧外的值
        if current_value <= self.min_value:
            current_value = self.min_value
        elif current_value >= self.max_value:
            current_value = self.max_value

        self.set_value(current_value)

    def set_value(self, value):
        """ 设置当前值 """
        if value < self.min_value or value > self.max_value or value == self.value:
            return None

        self.value = value
        self.value_changed.emit(value)
        self.update()

    def set_range(self, min_value, max_value):
        self.min_value = min_value
        self.max_value = max_value

    def set_precision(self, precision):
        self.precision = precision

    def set_step(self, step):
        self.step = step


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    ui = GaugeMini()
    ui.show()
    sys.exit(app.exec_())
