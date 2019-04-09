"""
将状态暴露给QML的常用方法是通过属性，而不是完全依赖于信号和插槽。一个属性是
setter、getter和通知信号的组合。setter是可选的，我们也可以有只读属性.

为了尝试这一点，我们将把NumberGenerator从最后一个示例更新为一个基于属性的版本。
它将有两个属性:number(保存最后一个随机数的只读属性)和maxNumber (具有可返回的
最大值的读写属性)。它还有一个插槽updateNumber 更新随机数.
"""
import sys
import time
import random
from PySide2.QtGui import QGuiApplication
from PySide2.QtQml import QQmlApplicationEngine
from PySide2.QtCore import QUrl, QObject, Signal, Slot, Property


class NumberGenerator(QObject):
    """数据生成"""
    # 声明 numberChanged 的信号
    numberChanged = Signal(int)
    # 声明 maxNumberChanged 的信号，也可以用@Signal槽方式
    maxNumberChanged = Signal()

    def __init__(self):
        QObject.__init__(self)
        self.__number = 42
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

    @Slot(int)
    def setMaxNumber(self, val):
        self.set_max_number(val)

    @Slot()
    def updateNumber(self):
        self.__set_number(random.randint(0, self.__max_number))

    # 定义一个 number 属性
    # 属性构造函数有三个参数在本例中:类型(int)、getter(get_number)和
    # 作为命名传递的通知信号参数(notify=numberChanged)。
    # 注意getter有一个Python名称，即使用下划线而不是camelCase，
    # 因为它用于从Python中读取值。对于QML，使用属性名称和编号
    number = Property(int, get_number, notify=numberChanged)

    # 提供了 setter 
    maxNumber = Property(int, get_max_number, set_max_number, notify=maxNumberChanged)


if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    number_generator = NumberGenerator()
    engine.rootContext().setContextProperty(
        "numberGenerator", number_generator
    )

    engine.load(QUrl("main.qml"))

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())
