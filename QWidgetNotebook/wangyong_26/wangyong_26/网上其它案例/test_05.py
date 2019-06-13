#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2017年12月19日
@author: Irony."[讽刺]
@site: http://alyl.vip, http://orzorz.vip, http://coding.net/u/892768447, http://github.com/892768447
@email: 892768447@qq.com
@file: LineChart自定义xy轴
@description:
'''
import random
import sys

from PyQt5.QtChart import QChartView, QLineSeries, QChart, QCategoryAxis
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout


__version__ = "0.0.1"

m_listCount = 3
m_valueMax = 10
m_valueCount = 7


def generateRandomData(listCount, valueMax, valueCount):
    random.seed()
    dataTable = []
    for i in range(listCount):
        dataList = []
        yValue = 0.0
        f_valueCount = float(valueCount)
        for j in range(valueCount):
            yValue += random.uniform(0, valueMax) / f_valueCount
            value = j + random.random() * m_valueMax / f_valueCount, yValue
            label = "Slice " + str(i) + ":" + str(j)
            dataList.append((value, label))
        dataTable.append(dataList)
    return dataTable


m_dataTable = generateRandomData(m_listCount, m_valueMax, m_valueCount)


def getChart(title):
    chart = QChart(title=title)
    for i, data_list in enumerate(m_dataTable):
        series = QLineSeries(chart)
        for value, _ in data_list:
            series.append(*value)
        series.setName("Series " + str(i))
        chart.addSeries(series)
    chart.createDefaultAxes()  # 创建默认的轴
    return chart


def customAxisX(chart):
    # 自定义x轴(均分)
    series = chart.series()
    if not series:
        return
    axisx = QCategoryAxis(
        chart, labelsPosition=QCategoryAxis.AxisLabelsPositionOnValue)
    minx = chart.axisX().min()
    maxx = chart.axisX().max()
    tickc = chart.axisX().tickCount()
    if tickc < 2:
        axisx.append("lable0", minx)
    else:
        step = (maxx - minx) / (tickc - 1)  # tickc>=2
        for i in range(0, tickc):
            axisx.append("lable%s" % i, minx + i * step)
    chart.setAxisX(axisx, series[-1])


def customTopAxisX(chart):
    # 自定义top x轴
    series = chart.series()
    if not series:
        return
    category = ["%d月" % i for i in range(1, 9)]  # 1-8月
    axisx = QCategoryAxis(
        chart, labelsPosition=QCategoryAxis.AxisLabelsPositionOnValue)
    axisx.setGridLineVisible(False)  # 隐藏网格线条
    axisx.setTickCount(len(category))  # 设置刻度个数
    chart.axisX().setTickCount(len(category))  # 强制修改x轴的刻度个数一致
    minx = chart.axisX().min()
    maxx = chart.axisX().max()
    tickc = chart.axisX().tickCount()
    step = (maxx - minx) / (tickc - 1)  # tickc>=2
    for i in range(0, tickc):
        axisx.append(category[i], minx + i * step)
    chart.addAxis(axisx, Qt.AlignTop)  # 添加到右侧
    series[-1].attachAxis(axisx)  # 附加到series上


def customAxisY(chart):
    # 自定义y轴(不等分)
    series = chart.series()
    if not series:
        return
    category = ["周一", "周二", "周三", "周四",
                "周五", "周六", "周日"]
    axisy = QCategoryAxis(
        chart, labelsPosition=QCategoryAxis.AxisLabelsPositionOnValue)
    axisy.setGridLineVisible(False)  # 隐藏网格线条
    axisy.setTickCount(len(category))  # 设置刻度个数
    miny = chart.axisY().min()
    maxy = chart.axisY().max()
    tickc = axisy.tickCount()
    if tickc < 2:
        axisy.append(category[0])
    else:
        step = (maxy - miny) / (tickc - 1)  # tickc>=2
        for i in range(0, tickc):
            axisy.append(category[i], miny + i * step)
    chart.addAxis(axisy, Qt.AlignRight)  # 添加到右侧
    series[-1].attachAxis(axisy)  # 附加到series上


class Window(QWidget):

    def __init__(self, *args, **kwargs):
        super(Window, self).__init__(*args, **kwargs)
        layout = QHBoxLayout(self)

        # 自定义x轴(和原来的x轴值对应等分)
        chart = getChart("自定义x轴(和原来的x轴值对应等分)")
        customAxisX(chart)
        layout.addWidget(QChartView(chart, self))
        # 自定义添加右侧y轴(等分,与左侧不对应)
        chart = getChart("自定义添加右侧y轴(等分,与左侧不对应)")
        customAxisY(chart)
        layout.addWidget(QChartView(chart, self))
        # 自定义top x轴(按现有新的x轴划分)
        chart = getChart("自定义top x轴(按现有新的x轴划分)")
        customTopAxisX(chart)
        layout.addWidget(QChartView(chart, self))


if __name__ == "__main__":
    app = QApplication(sys.argv)
    view = Window()
    view.resize(1000, 800)
    view.show()
    sys.exit(app.exec_())