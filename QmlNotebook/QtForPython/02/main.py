"""
Exposing Python Objects to QML
暴露 Python 对象到 QML 中

在Python和QML之间共享信息的最简单方法是将Python对象公开给QML。
这样做是通过QQmlApplicationEngine注册上下文属性。在我们能做
到这一点之前，我们需要这样做定义一个类，以便公开一个对象。

Qt类附带了许多我们希望能够使用的特性。这些是:信号、槽和属性。
在第一个示例中，我们将自己限制为一对基本信号和槽。其余的将在
进一步的例子。
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
