# This Python file uses the following encoding: utf-8
import sys
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQml import QQmlApplicationEngine
from PyQt5.QtCore import QUrl


if __name__ == "__main__":
    app = QApplication(sys.argv)
    engine = QQmlApplicationEngine()
    engine.load(QUrl("./main.qml")

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())
