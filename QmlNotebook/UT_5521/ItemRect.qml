import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
 
Rectangle
{
    id : itemrect
    width: 100;
    height: 30
    color: "#000000"
    property var text: ""
    border.color: "gray"
    border.width: isclicked ? 0:1
    property int index: 0
 
    property bool isclicked: false
    signal mclicked(int index);  //自定义信号
 
    function clearColor()
    {
        color = "#000000"
        isclicked = false
    }
 
 
    Text {
        anchors.centerIn: parent
        text: itemrect.text
        color: "white"
    }
 
    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents:true
 
        onClicked: {
            parent.color = "#98A9EE"
            mclicked(index);
            isclicked = true
        }
 
        onEntered:
        {
            if (!isclicked)
                parent.color = "#767C7C"
        }
 
        onExited:
        {
            if (!isclicked)
                parent.color = "#000000"
        }
 
    }
}