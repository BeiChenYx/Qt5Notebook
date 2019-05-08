import QtQuick 2.0
import QtCharts 2.3
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

Rectangle {
    id: deviceChart;
    implicitWidth: 400;
    implicitHeight: 300;
    color: "transparent";
    property int index: 0
    function initChart(title, index)
    {
        chartView.title = title;
        deviceChart.index = index;
    }
    signal preView(int index);
    signal nextView(int index);

    RowLayout {
        Button {
            id: leftBtn;
                implicitWidth: 20;
                implicitHeight: 20;
                background: Rectangle{
                    color: (leftBtn.hovered | leftBtn.pressed) ? "#42A1FA" : "transparent";
                    BorderImage {
                        source: "./image/left.png"
                        anchors.fill: parent;
                    }
                }
                onClicked: deviceChart.preView(deviceChart.index);
            }
        ChartView {
            id: chartView;
            implicitWidth: deviceChart.width - leftBtn.width - rightBtn.width - 10;
            implicitHeight: deviceChart.height;
                antialiasing: true;
                backgroundColor: "transparent";
                legend.alignment: Qt.AlignRight;
                legend.labelColor: "#59BAF2";
                legend.font.pixelSize: 11;
                legend.markerShape: Legend.MarkerShapeFromSeries;
                margins.top: 0;
                margins.bottom: 0;
                margins.left: 0;
                margins.right: 0;
    //            title: "设备地址为1的温湿度曲线"
                titleColor: "#59BAF2";

                DateTimeAxis{
                    id: axisXBottom;
                    format: "hh:mm:ss";
                    min: Date.fromLocaleString(Qt.locale(), "00:00:00", "hh:mm:ss")
                    max: Date.fromLocaleString(Qt.locale(), "23:59:59", "hh:mm:ss")
                    tickCount: 7;
                    labelsColor: "#59BAF2";
                    gridLineColor: "#4A5877";
                }
                ValueAxis{
                    id: axisYLeft;
                    min: -25;
                    max: 45;
                    tickCount: 5;
                    labelsColor: "#59BAF2";
                    gridLineColor: "#4A5877";
                }
                ValueAxis{
                    id: axisYRight;
                    min: 0;
                    max: 100;
                    tickCount: 5;
                    labelsColor: "#59BAF2";
                    gridVisible: false;
                }
                LineSeries {
                    name: "温度";
                    axisX: axisXBottom;
                    axisY: axisYLeft;
                }
                LineSeries {
                    name: "湿度";
                    axisX: axisXBottom;
                    axisYRight: axisYRight;
                }
            }
        Button {
            id: rightBtn;
            implicitWidth: 20;
                implicitHeight: 20;
                background: Rectangle{
                    color: (rightBtn.hovered | rightBtn.pressed) ? "#42A1FA" : "transparent";
                    BorderImage {
                        source: "./image/right.png"
                        anchors.fill: parent;
                    }
                }
                onClicked: deviceChart.nextView(deviceChart.index);
            }
    }
}
