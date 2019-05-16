import QtQuick 2.0

Rectangle {
    id: colorPicker;
    width: 50;
    height: 30;
    signal colorPicked(color clr);

    function configureBorder(colorId) {
        colorPicker.border.width = colorPicker.focus ? 2 : 0;
        colorPicker.border.color = colorPicker.focus ? "#90D750" : "#808080";
        console.log("call configureBorder: ", colorId);
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: {
            colorPicker.colorPicked(colorPicker.color);
            mouse.accepted = true;
            colorPicker.focus = true;
        }
    }

    Keys.onReturnPressed: {
        colorPicker.colorPicked(colorPicker.color);
        event.accepted = true;
    }
    Keys.onSelectPressed: {
        colorPicker.colorPicked(colorPicker.color);
        event.accepted = true;
    }

    onFocusChanged: {
        configureBorder();
    }
    // 单独文件内定义组件不需要 Component 对象
    Component.onCompleted: {
        configureBorder();
    }
}
