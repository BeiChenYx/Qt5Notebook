import sys
import time
import random
from PySide2.QtGui import QGuiApplication
from PySide2.QtQml import QQmlApplicationEngine
from PySide2.QtCore import QUrl, QObject, Signal, Slot, Property


# class NumberGenerator(QObject):
    # """信号和槽的方式"""
    # nextNumber = Signal(int)

    # def __init__(self):
        # QObject.__init__(self)

    # @Slot()
    # def giveNumber(self):
        # time.sleep(2) 
        # self.nextNumber.emit(random.randint(0, 99))

class NumberGenerator(QObject):

    numberChanged = Signal(int)
    def __init__(self):
        QObject.__init__(self)
        # 只读属性，保存最新的随机数
        self.__number = 42
        # 可读可写属性，保存最大的值
        self.__max_number = 99

    def set_max_number(self, val):
        if val < 0:
            val = 0

        if self.__max_number != val:
            self.__max_number = val

        if self.__number > self.__max_number:
            self.__set_number(self.__max_number)

    def get_max_number(self):
        return self.__max_number

    def __set_number(self, val):
        if self.__number != val:
            self.__number = val
            self.numberChanged.emit(self.__number)

    def get_number(self):
        return self.__number

    @Signal
    def maxNumberChanged(self):
        pass

    @Slot(int)
    def setMaxNumber(self, val):
        self.set_max_number(val)

    @Slot()
    def updateNumber(self):
        self.__set_number(random.randint(0, self.__max_number))

    # 实例化属性
    number = Property(int, get_number, notify=numberChanged)
    maxNumber = Property(int, get_max_number, set_max_number, notify=maxNumberChanged)


if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()
    number_generator = NumberGenerator()
    engine.rootContext().setContextProperty('numberGenerator', number_generator)
    engine.load(QUrl("main.qml"))

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())
