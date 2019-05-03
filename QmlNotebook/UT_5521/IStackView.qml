import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
      title: qsTr("Hello World")
      width: 640
      height: 480
      visible: true

      StackView {
          id: stack
          initialItem: pieCh;
          anchors.fill: parent
      }

      MouseArea {
          anchors.fill: parent;
          acceptedButtons: Qt.LeftButton | Qt.RightButton;
          onClicked: {
              if(mouse.button === Qt.LeftButton){
                  stack.clear();
                  stack.push(pieCh);
              }else if (mouse.button === Qt.RightButton){
                  stack.clear();
                  stack.push(lineCh);
              }
          }
      }

      PieChart {
          id: pieCh;
      }
      LineChart {
          id: lineCh;
      }

//      Component {
//          id: mainView

//          Row {
//              spacing: 10

//              Button {
//                  text: "Push"
//                  onClicked: stack.push(mainView)
//              }
//              Button {
//                  text: "Pop"
//                  enabled: stack.depth > 1
//                  onClicked: stack.pop()

//              }
//              Text {
//                  text: stack.depth
//              }
//          }
//      }
  }
