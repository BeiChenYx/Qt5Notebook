from PyQt5 import QtCore, QtGui, QtWidgets
from yewuwenjian import My_Main_window
import sys

if __name__ == "__main__":
        app = QtWidgets.QApplication(sys.argv)  # 实例化QApplication类，作为GUI主程序入口，需要import sys，可紧放在这句之前，也可放在代码最前面部分
        widget = QtWidgets.QMainWindow()  	# 因为Qt Designer默认继承的object类，不提供show()显示方法，所以我们生成一个QWidget（窗口）对象来重载我们设计的Ui_Dialog类，达到显示效果。
        mywindow = My_Main_window()  	# 实例化之前定义的窗口类
        mywindow.setupUi(widget)  # 设置布局Ui，布局的对象是刚才生成的widget（窗口）
        mywindow.setupBusi(widget)	 # 设置业务
        widget.show()  	# 使用 show() 方法
        sys.exit(app.exec_())