import QtQuick 2.0
import QtCharts 2.3

Item {
    id: lineChart;
    ChartView {
         title: "Line"
         anchors.fill: parent
         antialiasing: true

         LineSeries {
             name: "LineSeries"
             XYPoint { x: 0; y: 0 }
             XYPoint { x: 1.1; y: 2.1 }
             XYPoint { x: 1.9; y: 3.3 }
             XYPoint { x: 2.1; y: 2.1 }
             XYPoint { x: 2.9; y: 4.9 }
             XYPoint { x: 3.4; y: 3.0 }
             XYPoint { x: 4.1; y: 3.3 }
         }
     }
}
