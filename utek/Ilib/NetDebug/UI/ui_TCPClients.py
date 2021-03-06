# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'TCPClients.ui'
#
# Created by: PyQt5 UI code generator 5.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(554, 477)
        self.gridLayout = QtWidgets.QGridLayout(Form)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setHorizontalSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.horizontalLayout_4 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_4.setSpacing(6)
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.checkBox_Display_Time = QtWidgets.QCheckBox(Form)
        self.checkBox_Display_Time.setObjectName("checkBox_Display_Time")
        self.horizontalLayout_4.addWidget(self.checkBox_Display_Time)
        self.checkBox_Display_Hex = QtWidgets.QCheckBox(Form)
        self.checkBox_Display_Hex.setObjectName("checkBox_Display_Hex")
        self.horizontalLayout_4.addWidget(self.checkBox_Display_Hex)
        self.checkBox_Pause = QtWidgets.QCheckBox(Form)
        self.checkBox_Pause.setObjectName("checkBox_Pause")
        self.horizontalLayout_4.addWidget(self.checkBox_Pause)
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem)
        self.pushButton_Clear_Display = QtWidgets.QPushButton(Form)
        self.pushButton_Clear_Display.setObjectName("pushButton_Clear_Display")
        self.horizontalLayout_4.addWidget(self.pushButton_Clear_Display)
        self.pushButton_Clear_Count = QtWidgets.QPushButton(Form)
        self.pushButton_Clear_Count.setObjectName("pushButton_Clear_Count")
        self.horizontalLayout_4.addWidget(self.pushButton_Clear_Count)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.label_5 = QtWidgets.QLabel(Form)
        self.label_5.setMaximumSize(QtCore.QSize(20, 16777215))
        self.label_5.setObjectName("label_5")
        self.horizontalLayout_2.addWidget(self.label_5)
        self.label_TX = QtWidgets.QLabel(Form)
        self.label_TX.setMinimumSize(QtCore.QSize(40, 0))
        self.label_TX.setObjectName("label_TX")
        self.horizontalLayout_2.addWidget(self.label_TX)
        self.horizontalLayout.addLayout(self.horizontalLayout_2)
        self.label_3 = QtWidgets.QLabel(Form)
        self.label_3.setMaximumSize(QtCore.QSize(20, 16777215))
        self.label_3.setObjectName("label_3")
        self.horizontalLayout.addWidget(self.label_3)
        self.label_RX = QtWidgets.QLabel(Form)
        self.label_RX.setMinimumSize(QtCore.QSize(40, 0))
        self.label_RX.setObjectName("label_RX")
        self.horizontalLayout.addWidget(self.label_RX)
        self.horizontalLayout_4.addLayout(self.horizontalLayout)
        spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem1)
        self.gridLayout.addLayout(self.horizontalLayout_4, 1, 0, 1, 1)
        self.textEdit = QtWidgets.QTextEdit(Form)
        self.textEdit.setObjectName("textEdit")
        self.gridLayout.addWidget(self.textEdit, 0, 0, 1, 1)
        self.tabWidget = QtWidgets.QTabWidget(Form)
        self.tabWidget.setMaximumSize(QtCore.QSize(16777215, 200))
        self.tabWidget.setObjectName("tabWidget")
        self.tab_set = QtWidgets.QWidget()
        self.tab_set.setObjectName("tab_set")
        self.gridLayout_3 = QtWidgets.QGridLayout(self.tab_set)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.gridLayout_2 = QtWidgets.QGridLayout()
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.lineEdit_IP = QtWidgets.QLineEdit(self.tab_set)
        self.lineEdit_IP.setMaximumSize(QtCore.QSize(200, 16777215))
        self.lineEdit_IP.setObjectName("lineEdit_IP")
        self.gridLayout_2.addWidget(self.lineEdit_IP, 0, 0, 1, 1)
        self.lineEdit_Port = QtWidgets.QLineEdit(self.tab_set)
        self.lineEdit_Port.setMaximumSize(QtCore.QSize(80, 16777215))
        self.lineEdit_Port.setObjectName("lineEdit_Port")
        self.gridLayout_2.addWidget(self.lineEdit_Port, 0, 1, 1, 1)
        self.lineEdit_Clients_Count = QtWidgets.QLineEdit(self.tab_set)
        self.lineEdit_Clients_Count.setMaximumSize(QtCore.QSize(200, 16777215))
        self.lineEdit_Clients_Count.setObjectName("lineEdit_Clients_Count")
        self.gridLayout_2.addWidget(self.lineEdit_Clients_Count, 1, 0, 1, 1)
        self.pushButton_Connect = QtWidgets.QPushButton(self.tab_set)
        self.pushButton_Connect.setMaximumSize(QtCore.QSize(80, 16777215))
        self.pushButton_Connect.setObjectName("pushButton_Connect")
        self.gridLayout_2.addWidget(self.pushButton_Connect, 1, 1, 1, 1)
        self.gridLayout_3.addLayout(self.gridLayout_2, 0, 0, 1, 1)
        spacerItem2 = QtWidgets.QSpacerItem(38, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_3.addItem(spacerItem2, 0, 1, 1, 1)
        self.listWidget = QtWidgets.QListWidget(self.tab_set)
        self.listWidget.setObjectName("listWidget")
        self.gridLayout_3.addWidget(self.listWidget, 0, 2, 2, 1)
        spacerItem3 = QtWidgets.QSpacerItem(20, 123, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout_3.addItem(spacerItem3, 1, 0, 1, 1)
        self.tabWidget.addTab(self.tab_set, "")
        self.gridLayout.addWidget(self.tabWidget, 2, 0, 1, 1)

        self.retranslateUi(Form)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "TCP/IP_Clients"))
        self.checkBox_Display_Time.setText(_translate("Form", "显示时间"))
        self.checkBox_Display_Hex.setText(_translate("Form", "Hex"))
        self.checkBox_Pause.setText(_translate("Form", "暂停"))
        self.pushButton_Clear_Display.setText(_translate("Form", "清空显示"))
        self.pushButton_Clear_Count.setText(_translate("Form", "清空计数"))
        self.label_5.setText(_translate("Form", "TX"))
        self.label_TX.setText(_translate("Form", "0"))
        self.label_3.setText(_translate("Form", "RX"))
        self.label_RX.setText(_translate("Form", "0"))
        self.textEdit.setPlaceholderText(_translate("Form", "网络数据接收"))
        self.lineEdit_IP.setPlaceholderText(_translate("Form", "目标服务器地址"))
        self.lineEdit_Port.setPlaceholderText(_translate("Form", "端口号"))
        self.lineEdit_Clients_Count.setPlaceholderText(_translate("Form", "客户端个数"))
        self.pushButton_Connect.setText(_translate("Form", "连接"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_set), _translate("Form", "配置"))

import img_rc
