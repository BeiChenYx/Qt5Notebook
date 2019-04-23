import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.1

Rectangle {
    width: 320;
    height: 240;
    color: "gray";

    QtObject {
        id: attrs;
        property int counter;
        Component.onCompleted: {
            attrs.counter = 10;
        }
    }

    Text {
        id: countShow;
        anchors.centerIn: parent;
        color: "blue";
        font.pixelSize: 40;
    }

    Timer {
        id: countDown;
        interval: 1000;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            countShow.text = attrs.counter;
            attrs.counter -= 1;
            if (attrs.counter < 0){
                countDown.stop();
                countShow.text = "Clap Now!";
            }
        }
    }

    Button {
        id: startButton;
        anchors.top: countShow.bottom;
        anchors.topMargin: 20;
        anchors.horizontalCenter: countShow.horizontalCenter;
        text: "Start";
        onClicked: {
            attrs.counter = 10;
            countDown.start();
        }
    }
}

//Window {
//    id: mainWindow
//    visible: true
//    width: 540
//    height: 330
//    title: qsTr("Hello World")
//    flags: Qt.Window | Qt.FramelessWindowHint
//    color:"#262626"
//    Rectangle {
//        id: title
//        anchors.top: parent.top
//        anchors.left: parent.left
//        anchors.right: parent.right
//        height: 30
//        color: "#979797"


//        MouseArea {
//            anchors.fill: parent
//            acceptedButtons: Qt.LeftButton
//            property point clickPos: "0,0"
//            onPressed: {
//                clickPos = Qt.point(mouse.x, mouse.y)
//            }


//            onPositionChanged: {
//                //鼠标偏移量
//                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)


//                //如果mainwindow继承自QWidget,用setPos
//                mainWindow.setX(mainWindow.x + delta.x)
//                mainWindow.setY(mainWindow.y + delta.y)
//            }
//        }
//    }
//}
