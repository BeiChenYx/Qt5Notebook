# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'frmegaugemini.ui'
#
# Created by: PyQt5 UI code generator 5.13.2
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_frmGaugeMini(object):
    def setupUi(self, frmGaugeMini):
        frmGaugeMini.setObjectName("frmGaugeMini")
        frmGaugeMini.resize(500, 300)
        self.gridLayout_2 = QtWidgets.QGridLayout(frmGaugeMini)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.horizontalSlider = QtWidgets.QSlider(frmGaugeMini)
        self.horizontalSlider.setMinimum(-100)
        self.horizontalSlider.setMaximum(100)
        self.horizontalSlider.setSingleStep(1)
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.gridLayout_2.addWidget(self.horizontalSlider, 1, 2, 1, 1)
        self.ckDouble = QtWidgets.QCheckBox(frmGaugeMini)
        self.ckDouble.setObjectName("ckDouble")
        self.gridLayout_2.addWidget(self.ckDouble, 1, 0, 1, 1)
        self.widget = QtWidgets.QWidget(frmGaugeMini)
        self.widget.setObjectName("widget")
        self.gridLayout = QtWidgets.QGridLayout(self.widget)
        self.gridLayout.setContentsMargins(9, 9, 9, 9)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.labValue1 = QtWidgets.QLabel(self.widget)
        self.labValue1.setText("")
        self.labValue1.setAlignment(QtCore.Qt.AlignCenter)
        self.labValue1.setObjectName("labValue1")
        self.gridLayout.addWidget(self.labValue1, 1, 0, 1, 1)
        self.labValue3 = QtWidgets.QLabel(self.widget)
        self.labValue3.setText("")
        self.labValue3.setAlignment(QtCore.Qt.AlignCenter)
        self.labValue3.setObjectName("labValue3")
        self.gridLayout.addWidget(self.labValue3, 1, 2, 1, 1)
        self.labValue4 = QtWidgets.QLabel(self.widget)
        self.labValue4.setText("")
        self.labValue4.setAlignment(QtCore.Qt.AlignCenter)
        self.labValue4.setObjectName("labValue4")
        self.gridLayout.addWidget(self.labValue4, 4, 0, 1, 1)
        self.labValue6 = QtWidgets.QLabel(self.widget)
        self.labValue6.setText("")
        self.labValue6.setAlignment(QtCore.Qt.AlignCenter)
        self.labValue6.setObjectName("labValue6")
        self.gridLayout.addWidget(self.labValue6, 4, 2, 1, 1)
        self.labValue2 = QtWidgets.QLabel(self.widget)
        self.labValue2.setText("")
        self.labValue2.setAlignment(QtCore.Qt.AlignCenter)
        self.labValue2.setObjectName("labValue2")
        self.gridLayout.addWidget(self.labValue2, 1, 1, 1, 1)
        self.labValue5 = QtWidgets.QLabel(self.widget)
        self.labValue5.setText("")
        self.labValue5.setAlignment(QtCore.Qt.AlignCenter)
        self.labValue5.setObjectName("labValue5")
        self.gridLayout.addWidget(self.labValue5, 4, 1, 1, 1)
        self.gaugeMini3 = GaugeMini(self.widget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.gaugeMini3.sizePolicy().hasHeightForWidth())
        self.gaugeMini3.setSizePolicy(sizePolicy)
        self.gaugeMini3.setObjectName("gaugeMini3")
        self.gridLayout.addWidget(self.gaugeMini3, 0, 2, 1, 1)
        self.gaugeMini6 = GaugeMini(self.widget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.gaugeMini6.sizePolicy().hasHeightForWidth())
        self.gaugeMini6.setSizePolicy(sizePolicy)
        self.gaugeMini6.setObjectName("gaugeMini6")
        self.gridLayout.addWidget(self.gaugeMini6, 3, 2, 1, 1)
        self.gaugeMini1 = GaugeMini(self.widget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.gaugeMini1.sizePolicy().hasHeightForWidth())
        self.gaugeMini1.setSizePolicy(sizePolicy)
        self.gaugeMini1.setObjectName("gaugeMini1")
        self.gridLayout.addWidget(self.gaugeMini1, 0, 0, 1, 1)
        self.gaugeMini5 = GaugeMini(self.widget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.gaugeMini5.sizePolicy().hasHeightForWidth())
        self.gaugeMini5.setSizePolicy(sizePolicy)
        self.gaugeMini5.setObjectName("gaugeMini5")
        self.gridLayout.addWidget(self.gaugeMini5, 3, 1, 1, 1)
        self.gaugeMini4 = GaugeMini(self.widget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.gaugeMini4.sizePolicy().hasHeightForWidth())
        self.gaugeMini4.setSizePolicy(sizePolicy)
        self.gaugeMini4.setObjectName("gaugeMini4")
        self.gridLayout.addWidget(self.gaugeMini4, 3, 0, 1, 1)
        self.gaugeMini2 = GaugeMini(self.widget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.gaugeMini2.sizePolicy().hasHeightForWidth())
        self.gaugeMini2.setSizePolicy(sizePolicy)
        self.gaugeMini2.setObjectName("gaugeMini2")
        self.gridLayout.addWidget(self.gaugeMini2, 0, 1, 1, 1)
        self.label = QtWidgets.QLabel(self.widget)
        self.label.setText("")
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 2, 0, 1, 1)
        self.gridLayout_2.addWidget(self.widget, 0, 0, 1, 3)
        self.cboxPointerStyle = QtWidgets.QComboBox(frmGaugeMini)
        self.cboxPointerStyle.setMinimumSize(QtCore.QSize(100, 0))
        self.cboxPointerStyle.setObjectName("cboxPointerStyle")
        self.cboxPointerStyle.addItem("")
        self.cboxPointerStyle.addItem("")
        self.cboxPointerStyle.addItem("")
        self.cboxPointerStyle.addItem("")
        self.gridLayout_2.addWidget(self.cboxPointerStyle, 1, 1, 1, 1)

        self.retranslateUi(frmGaugeMini)
        QtCore.QMetaObject.connectSlotsByName(frmGaugeMini)

    def retranslateUi(self, frmGaugeMini):
        _translate = QtCore.QCoreApplication.translate
        frmGaugeMini.setWindowTitle(_translate("frmGaugeMini", "Form"))
        self.ckDouble.setText(_translate("frmGaugeMini", "左右等分"))
        self.cboxPointerStyle.setItemText(0, _translate("frmGaugeMini", "线条指示器"))
        self.cboxPointerStyle.setItemText(1, _translate("frmGaugeMini", "指针指示器"))
        self.cboxPointerStyle.setItemText(2, _translate("frmGaugeMini", "圆角指针指示器"))
        self.cboxPointerStyle.setItemText(3, _translate("frmGaugeMini", "三角指示器"))
from gaugemini import GaugeMini