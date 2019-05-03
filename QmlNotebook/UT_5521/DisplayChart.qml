import QtQuick 2.0
import QtCharts 2.3
import QtQuick.Controls 2.5

/*
 * 使用 StackView 的方式切换曲线配置和曲线展示的页面,
 * 曲线的配置主要有 设备选择 和 日期选择，日期如果为当前日期
 * 则显示当前实时数据，如果是历史日期，则从数据库中获取历史数据；
 */
Rectangle {
    id: spline;
    width: 960;
    height: 540;
    color: "transparent";

    // 单点测试图表
    Rectangle {
        id: singleChart;
        width: 800;
        height: parent.height;
        anchors.left: parent.left;
        anchors.top: parent.top;
        color: "transparent";
        Image {
            source: "./image/single.png";
            fillMode: Image.Stretch;
            anchors.fill: parent;
            z: 0;
        }
        ChartView {
            title: "温湿度";
            anchors.fill: parent;
            antialiasing: true;
            SplineSeries {
                id: splinePoint;
                name: "温度";
                axisX: ValueAxis{
                   min: 0;
                   max: 20;
                }
                axisY: ValueAxis{
                    min: 0;
                    max: 20;
                }
            }
        }
    }
    // 多点测试图表
//    Rectangle {
//        id: doubleChart;
//        anchors.top: singleChart.bottom;
//        anchors.topMargin: 20 * root.vRatio;
//        anchors.left: parent.left;
//        width: parent.width;
//        height: 200 * root.vRatio;
//        color: "transparent";
//        Image {
//            source: "./image/double.png";
//            fillMode: Image.Stretch;
//            anchors.fill: parent;
//            z: 0;
//        }
//    }
//    Rectangle {
//        width: 800;
//        height: parent.height;
//        anchors.left: parent.left;
//        anchors.top: parent.top;
//        ChartView {
//            title: "温湿度";
//            anchors.fill: parent;
//            antialiasing: true;
//            SplineSeries {
//                id: splinePoint;
//                name: "温度";
//                axisX: ValueAxis{
//                   min: 0;
//                   max: 20;
//                }
//                axisY: ValueAxis{
//                    min: 0;
//                    max: 20;
//                }
//            }
//        }
//    }
//    Rectangle {
//        // 图形配置
//    }

}
