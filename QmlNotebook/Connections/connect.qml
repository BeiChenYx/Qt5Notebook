import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
    id: forwarder;
    width: 400;
    height: 300;

    signal send();
    onSend: console.log("Send clicked");

    MouseArea {
        id: mousearea;
        anchors.fill: parent;
        onClicked: console.log("MouseArea clicked")
    }

    Component.onCompleted: {
        mousearea.clicked.connect(send);
    }
}
