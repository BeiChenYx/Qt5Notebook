import sys

from PyQt5 import QtWidgets, QtGui, QtCore

from gaugemini import GaugeMini
from ui_frame import Ui_frmGaugeMini


class FrameGaugeMini(QtWidgets.QWidget, Ui_frmGaugeMini):

    def __init__(self, parent=None):
        super(FrameGaugeMini, self).__init__(parent)
        self.setupUi(self)


def main():
    """ 显示UI界面，会阻塞线程，开启Qt事件循环 """
    app = QtWidgets.QApplication(sys.argv)
    frame_ui = FrameGaugeMini()
    frame_ui.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
