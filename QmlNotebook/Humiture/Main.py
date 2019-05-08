import sys
import time
import random
from PySide2.QtGui import QGuiApplication
from PySide2.QtQml import QQmlApplicationEngine
from PySide2.QtCore import QUrl, QObject, Signal, Slot, Property



if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()
    engine.load(QUrl("./Main.qml"))

    # if not engine.rootObjects():
    #     sys.exit(-1)

    sys.exit(app.exec_())