import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
 
ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")
 
    Rectangle
    {
        anchors.fill: parent
        color: "#000000"
 
        SwipeView {
            id: swipeView
            anchors.left: rootitem.right
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            currentIndex: 0
            Rectangle{
                color: "#649090"
                //anchors.fill: parent
            }
            Rectangle{
                color: "green"
                //anchors.fill: parent
            }
            Rectangle{
                color: "#525AC1"
                //anchors.fill: parent
            }
 
        }
 
        //导航栏start--------------------------------------
        //测试 默认在左侧
        Rectangle
        {
            id : rootitem
            anchors.left: parent.left
            //anchors.leftMargin: 0
            width: 100;
            height: parent.height;
            color: "#1B96D1"
 
            ColumnLayout{
                id : lyout
                width: parent.width
                //height: parent.height
                Layout.fillWidth: true
                spacing: 1
            }
        }
        //导航栏end----------------------------------------
 
        //添加项
        function addItem(text,index)
        {
            var component = Qt.createComponent("ItemRect.qml");
            if (component.status == Component.Ready)
            {
              var itemrect = component.createObject(lyout);
              itemrect.text = text;
              itemrect.index = index;
              itemrect.mclicked.connect(mmclicked); //信号的关联槽函数
            }
        }
        Component.onCompleted:
        {
            addItem("测试1",0)
            addItem("测试2",1)
            addItem("测试3",2)
        }
 
        //事件处理
        function mmclicked(index)
        {
            swipeView.currentIndex = index;
            for (var i in lyout.children)
            {
                if (lyout.children[i].index != index)
                    lyout.children[i].clearColor();
            }
        }
 
        /////////////////////////////////////////////
    }
 
}