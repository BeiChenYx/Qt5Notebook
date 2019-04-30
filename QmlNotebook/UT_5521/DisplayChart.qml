import QtQuick 2.0
import QtCharts 2.3
import QtQuick.Controls 2.5

Rectangle {
    id: spline;
    width: 960;
    height: 540;
    color: "transparent";
    property int pData: 0;
    Rectangle {
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
    Rectangle {
        // 图形配置
    }

}
