import QtQuick 2.0
import QtCharts 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.4
import an.qt.HandleHumiture 1.0

/*
 * 使用 StackView 的方式切换曲线配置和曲线展示的页面,
 * 曲线的配置主要有 设备选择 和 日期选择，日期如果为当前日期
 * 则显示当前实时数据，如果是历史日期，则从数据库中获取历史数据；
 */
Rectangle {
    id: spline;
    color: "transparent";

    HandleHumiture {
        id: chartHanle;
    }

    StackView {
        id: stackView;
        anchors.fill: parent;
        initialItem: singleChart;
        clip: true;
    }

    Component {
        id: configView;
        GridLayout {
            rows: 3;
            columns: 2;
            rowSpacing: 10;
            columnSpacing: 5;

            width: 100;
            height: 100;
            Label {
                width: 20;
                Layout.rowSpan: 1;
                Layout.columnSpan: 1;
                Layout.leftMargin: 40;
                Layout.topMargin: 25;
                color: "#59BAF2";
                text: "从机地址:";
            }
            UComboBox {
                id: addrComboBox
                Layout.topMargin: 25;
                Layout.rowSpan: 1;
                Layout.columnSpan: 1;
                implicitWidth: 80;
                model: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16];
            }

            Label {
                Layout.leftMargin: 40;
                width: 20;
                Layout.rowSpan: 1;
                Layout.columnSpan: 1;
                text: "图表日期:";
                color: "#59BAF2";
            }
            Loader {
                Layout.rowSpan: 1;
                Layout.columnSpan: 1;
                id: calendarSelect;
                source: "qrc:/DateTimeEdit.qml"
            }
            UButton {
                id: control;
                Layout.rowSpan: 1;
                Layout.columnSpan: 1;
                Layout.column: 1;
                Layout.row: 2;
                text: "保存"

                onClicked: {
                    var deviceAddr = parseInt(addrComboBox.currentText);
                    var date = calendarSelect.item.currentText;
                    chartHanle.onQueryRecord(deviceAddr, date);
                    if(stackView.depth >= 1){
                        stackView.pop();

                        var x = Qt.formatDateTime(new Date(), "hh:mm:ss");
                        temperatureSeries.append(x, 20);
                        temperatureSeries.append(x, 25);
                    }
                }
            }
        }
    }

    Component {
        id: singleChart;
        // 单点测试图表
        RowLayout {
            Button {
                id: leftBtn;
                implicitWidth: 20;
                implicitHeight: 20;
                background: Rectangle{
                    color: (leftBtn.hovered | leftBtn.pressed) ? "#42A1FA" : "transparent";
                    BorderImage {
                        source: "qrc:/image/left.png"
                        anchors.fill: parent;
                    }
                }
            }
            ChartView {
                implicitWidth: parent.width - leftBtn.width - rightBtn.width - 10;
                implicitHeight: parent.height;
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
                title: "设备地址为1的温湿度曲线"
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
                    id: temperatureSeries;
                    name: "温度";
                    axisX: axisXBottom;
                    axisY: axisYLeft;
                }
                LineSeries {
                    id: humiditySeries;
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
                        source: "qrc:/image/right.png"
                        anchors.fill: parent;
                    }
                }
                onClicked: {
                    stackView.push(configView);
                }
            }
        }
    }

    Connections {
        target: chartHanle;
        onHumitureData: {
            var x = Qt.formatDateTime(new Date(), "hh:mm:ss");
//            console.log("x: ", x, "temperature: ", temperature, "humidity: ", humidity);
//            console.log("x: ", x, "humidity: ", humidity);
//            temperatureSeries.append(x, temperature);
//            humiditySeries.append(x, humidity);
        }
    }
}
