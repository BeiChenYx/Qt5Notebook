# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'calendar.ui'
#
# Created by: PyQt5 UI code generator 5.13.2
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_calendarWidget(object):
    def setupUi(self, calendarWidget):
        calendarWidget.setObjectName("calendarWidget")
        calendarWidget.resize(609, 490)
        calendarWidget.setStyleSheet("#widget_week{\n"
"    background-color: rgb(22, 160, 134);\n"
"}\n"
"#widget_week QLabel{\n"
"    color:  rgb(255, 255, 255);\n"
"}")
        self.gridLayout_7 = QtWidgets.QGridLayout(calendarWidget)
        self.gridLayout_7.setObjectName("gridLayout_7")
        self.widget_top = QtWidgets.QWidget(calendarWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.widget_top.sizePolicy().hasHeightForWidth())
        self.widget_top.setSizePolicy(sizePolicy)
        self.widget_top.setObjectName("widget_top")
        self.gridLayout = QtWidgets.QGridLayout(self.widget_top)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setSpacing(20)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.toolButton_py = QtWidgets.QToolButton(self.widget_top)
        self.toolButton_py.setObjectName("toolButton_py")
        self.horizontalLayout.addWidget(self.toolButton_py)
        self.dateEdit_y = QtWidgets.QDateEdit(self.widget_top)
        self.dateEdit_y.setCalendarPopup(False)
        self.dateEdit_y.setObjectName("dateEdit_y")
        self.horizontalLayout.addWidget(self.dateEdit_y)
        self.toolButton_ny = QtWidgets.QToolButton(self.widget_top)
        self.toolButton_ny.setObjectName("toolButton_ny")
        self.horizontalLayout.addWidget(self.toolButton_ny)
        self.horizontalLayout_3.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.toolButton_pm = QtWidgets.QToolButton(self.widget_top)
        self.toolButton_pm.setObjectName("toolButton_pm")
        self.horizontalLayout_2.addWidget(self.toolButton_pm)
        self.dateEdit_m = QtWidgets.QDateEdit(self.widget_top)
        self.dateEdit_m.setCalendarPopup(False)
        self.dateEdit_m.setObjectName("dateEdit_m")
        self.horizontalLayout_2.addWidget(self.dateEdit_m)
        self.toolButton_nm = QtWidgets.QToolButton(self.widget_top)
        self.toolButton_nm.setObjectName("toolButton_nm")
        self.horizontalLayout_2.addWidget(self.toolButton_nm)
        self.horizontalLayout_3.addLayout(self.horizontalLayout_2)
        self.pushButton_now = QtWidgets.QPushButton(self.widget_top)
        self.pushButton_now.setObjectName("pushButton_now")
        self.horizontalLayout_3.addWidget(self.pushButton_now)
        self.gridLayout.addLayout(self.horizontalLayout_3, 0, 0, 1, 1)
        self.gridLayout_7.addWidget(self.widget_top, 0, 0, 1, 1)
        self.widget_week = QtWidgets.QWidget(calendarWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.widget_week.sizePolicy().hasHeightForWidth())
        self.widget_week.setSizePolicy(sizePolicy)
        self.widget_week.setObjectName("widget_week")
        self.gridLayout_2 = QtWidgets.QGridLayout(self.widget_week)
        self.gridLayout_2.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.label_7 = QtWidgets.QLabel(self.widget_week)
        self.label_7.setAlignment(QtCore.Qt.AlignCenter)
        self.label_7.setObjectName("label_7")
        self.gridLayout_2.addWidget(self.label_7, 0, 0, 1, 1)
        self.label_1 = QtWidgets.QLabel(self.widget_week)
        self.label_1.setAlignment(QtCore.Qt.AlignCenter)
        self.label_1.setObjectName("label_1")
        self.gridLayout_2.addWidget(self.label_1, 0, 1, 1, 1)
        self.label_2 = QtWidgets.QLabel(self.widget_week)
        self.label_2.setAlignment(QtCore.Qt.AlignCenter)
        self.label_2.setObjectName("label_2")
        self.gridLayout_2.addWidget(self.label_2, 0, 2, 1, 1)
        self.label_3 = QtWidgets.QLabel(self.widget_week)
        self.label_3.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setObjectName("label_3")
        self.gridLayout_2.addWidget(self.label_3, 0, 3, 1, 1)
        self.label_4 = QtWidgets.QLabel(self.widget_week)
        self.label_4.setAlignment(QtCore.Qt.AlignCenter)
        self.label_4.setObjectName("label_4")
        self.gridLayout_2.addWidget(self.label_4, 0, 4, 1, 1)
        self.label_5 = QtWidgets.QLabel(self.widget_week)
        self.label_5.setAlignment(QtCore.Qt.AlignCenter)
        self.label_5.setObjectName("label_5")
        self.gridLayout_2.addWidget(self.label_5, 0, 5, 1, 1)
        self.label_6 = QtWidgets.QLabel(self.widget_week)
        self.label_6.setAlignment(QtCore.Qt.AlignCenter)
        self.label_6.setObjectName("label_6")
        self.gridLayout_2.addWidget(self.label_6, 0, 6, 1, 1)
        self.gridLayout_7.addWidget(self.widget_week, 1, 0, 1, 1)
        self.widget_calendar = QtWidgets.QWidget(calendarWidget)
        self.widget_calendar.setObjectName("widget_calendar")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.widget_calendar)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setSpacing(6)
        self.verticalLayout.setObjectName("verticalLayout")
        self.gridLayout_7.addWidget(self.widget_calendar, 2, 0, 1, 1)
        self.gridLayout_7.setRowStretch(0, 1)
        self.gridLayout_7.setRowStretch(1, 1)
        self.gridLayout_7.setRowStretch(2, 8)

        self.retranslateUi(calendarWidget)
        QtCore.QMetaObject.connectSlotsByName(calendarWidget)

    def retranslateUi(self, calendarWidget):
        _translate = QtCore.QCoreApplication.translate
        calendarWidget.setWindowTitle(_translate("calendarWidget", "自定义日历"))
        self.toolButton_py.setText(_translate("calendarWidget", "..."))
        self.dateEdit_y.setDisplayFormat(_translate("calendarWidget", "yyyy"))
        self.toolButton_ny.setText(_translate("calendarWidget", "..."))
        self.toolButton_pm.setText(_translate("calendarWidget", "..."))
        self.dateEdit_m.setDisplayFormat(_translate("calendarWidget", "MM"))
        self.toolButton_nm.setText(_translate("calendarWidget", "..."))
        self.pushButton_now.setText(_translate("calendarWidget", "转到今天"))
        self.label_7.setText(_translate("calendarWidget", "星期日"))
        self.label_1.setText(_translate("calendarWidget", "星期一"))
        self.label_2.setText(_translate("calendarWidget", "星期二"))
        self.label_3.setText(_translate("calendarWidget", "星期三"))
        self.label_4.setText(_translate("calendarWidget", "星期四"))
        self.label_5.setText(_translate("calendarWidget", "星期五"))
        self.label_6.setText(_translate("calendarWidget", "星期六"))
import font_rc
