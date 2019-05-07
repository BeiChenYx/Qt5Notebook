import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4 as OldControl

Rectangle {
   width: 100;
   height: 30;

   Rectangle {
       anchors.fill: parent;
       color: "gray";
       TextInput {
           id: dateTimeBtn;
           anchors.centerIn: parent;
           focus: true;
           font.pixelSize: 16;
           text: "双击选择日期";
       }
       MouseArea {
           anchors.fill: parent;
           onClicked: {
               // 弹出日历
               chartCalendar.sourceComponent = cal
               chartCalendar.x = parent.x;
               chartCalendar.y = parent.y + parent.height;
           }
       }
   }

   Component {
       id: cal;
       OldControl.Calendar {
           minimumDate: new Date(2015, 1, 1)
           maximumDate: new Date();
           signal endClicked(var date);
           onDoubleClicked: endClicked(date);
       }
   }

   Loader {
       id: chartCalendar;
   }
   Connections {
       target: chartCalendar.item;
       onEndClicked: {
           chartCalendar.sourceComponent = undefined;
           console.log("date: ", date);
           dateTimeBtn.text = date.toLocaleDateString(Qt.locale(), "yyyy-MM-dd")
       }
   }
}
