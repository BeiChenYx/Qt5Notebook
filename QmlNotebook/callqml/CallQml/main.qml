import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    objectName: "rootObject";
    title: qsTr("Hello World")

    Text {
        objectName: "textLable"
        text: qsTr("Hello World")
        anchors.centerIn: parent
        font.pixelSize: 26;
    }

    Button {
        anchors.right: parent.right;
        anchors.rightMargin: 4;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 4;
        text: "quit"
        objectName: "quitButton"
    }
    function myQmlFunction(msg){
        console.log("Got message1: ", msg)
        return "some return value 1"
    }
}
