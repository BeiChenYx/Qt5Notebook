import QtQuick 2.0
import QtQuick.Controls 2.0

Button {
    id: control;
//    text: "保存"
    contentItem: Text{
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#59BAF2";
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle{
        implicitHeight: 30;
        implicitWidth: 100;
        color: "transparent";
        border.color: "#3270E1";
        radius: control.width / 10;
    }
}
