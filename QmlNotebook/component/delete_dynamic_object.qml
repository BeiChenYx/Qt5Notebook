import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    id: rootItem;
    width: 360;
    height: 300;
    property int count: 0;
    property Component component: null;

    Text {
        id: coloredText;
        text: "Hello World!";
        anchors.centerIn: parent;
        font.pixelSize: 24;
    }

    function changeTextColor(clr){
        coloredText.color = clr;
    }

    function createColorPicker(clr){
        if(rootItem.component == null){
            rootItem.component = Qt.createComponent("./ColorPicker.qml");
        }
        var colorPicker;
        if(rootItem.component.status == Component.Ready){
            colorPicker = rootItem.component.createObject(rootItem,
                 {"color": clr, "x": rootItem.count * 55, "y": 10});
            colorPicker.colorPicked.connect(rootItem.changeTextColor);
            if(rootItem.count % 2 == 1){
                colorPicker.destroy(1000);
            }
        }
        rootItem.count++;
    }

    Button {
        id: add;
        text: "add";
        anchors.left: parent.left;
        anchors.leftMargin: 4;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 4;
        onClicked: {
            createColorPicker(Qt.rgba(Math.random(),
                          Math.random(), Math.random(), 1));
        }
    }
}
