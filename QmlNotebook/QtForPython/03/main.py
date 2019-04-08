"""
将状态暴露给QML的常用方法是通过属性，而不是完全依赖于信号和插槽。一个
属性是setter、getter和通知信号的组合。setter是可选的，我们也可以有
只读属性

为了尝试这一点，我们将把NumberGenerator从最后一个示例更新为一个基于属性的版本。它
将有两个属性:number(保存最后一个随机数的只读属性)和maxNumber (a)
具有可返回的最大值的读写属性。它还有一个插槽updateNumber
更新随机数
"""
import sys
import time
import random
from PySide2.QtGui import QGuiApplication
from PySide2.QtQml import QQmlApplicationEngine
from PySide2.QtCore import QUrl, QObject, Signal, Slot


class NumberGenerator(QObject):
    """数据生成"""

    nextNumber = Signal(int)
    def __init__(self):
        QObject.__init__(self)

    @Slot()
    def giveNumber(self):
        self.nextNumber.emit(random.randint(0, 99))


if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    """
    有趣的行是我们首先安装一个数字生成器的行。然后将此对象公开给
    QML使用QML引擎的rootContext的setContextProperty方法。这暴露了
    对象，将QML作为名称numberGenerator下的全局变量。
    """
    number_generator = NumberGenerator()
    engine.rootContext().setContextProperty(
        "numberGenerator", number_generator
    )

    engine.load(QUrl("main.qml"))

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())
