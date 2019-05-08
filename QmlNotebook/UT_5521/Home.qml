import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

// 主页显示区, 设置和曲线的显示
Rectangle {
    id: chartDisplay;
//    width: 600 * root.hRatio;
//    height: 420 * root.vRatio;
    color: "transparent";

    // 单点测试图表
    Rectangle {
        id: singleChart;
        anchors.top: parent.top;
        anchors.left: parent.left;
        width: parent.width;
        height: 200 * root.vRatio;
        color: "transparent";
        Image {
            source: "./image/single.png";
            fillMode: Image.Stretch;
            anchors.fill: parent;
            z: 0;
        }
        Loader {
            id: singleStackView;
            source: "./SingleChart.qml";
            anchors.fill: parent;
            anchors.topMargin: 25 * root.vRatio;
            anchors.leftMargin: 10 * root.vRatio;
            anchors.rightMargin: 10 * root.vRatio;
            z: 1;
        }
    }
    // 多点测试图表
    Rectangle {
        id: doubleChart;
        anchors.top: singleChart.bottom;
        anchors.topMargin: 20 * root.vRatio;
        anchors.left: parent.left;
        width: parent.width;
        height: 200 * root.vRatio;
        color: "transparent";
        Image {
            source: "./image/double.png";
            fillMode: Image.Stretch;
            anchors.fill: parent;
            z: 0;
        }
        Loader {
            id: doubleStackView;
            source: "./DoubleChart.qml";
            anchors.fill: parent;
            anchors.topMargin: 25 * root.vRatio;
            anchors.leftMargin: 10 * root.vRatio;
            anchors.rightMargin: 10 * root.vRatio;
            z: 1;
        }
    }
}
