import QtQuick 2.0
import QtQuick.Controls 2.5

TextField {
//    id: dateTimeBtn;
    anchors.centerIn: parent;
//    placeholderText: qsTr("双击选择日期")
    focus: true;
    font.pixelSize: 12;
    color: "#59BAF2";

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 30
        color: "transparent";
        border.color: "#3270E1";
    }
}
