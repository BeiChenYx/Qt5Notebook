import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: root;
    visible: true;
    width: 640;
    height: 480;
    title: "SwipeView";

    //导航栏start--------------------------------------
    //测试 默认在左侧
    Rectangle
    {
        id : leftNav;
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

    StackLayout {
        id: stackMain;
        currentIndex: 0;
        anchors.left: leftNav.right;

        LineChart {
            id: lineCh;
            implicitWidth: root.width - leftNav.width;
            implicitHeight: root.height;
        }
        PieChart {
            id: pieCh;
            implicitWidth: root.width - leftNav.width;
            implicitHeight: root.height;
        }
    }

    //添加项
    function addItem(text,index)
    {
        var component = Qt.createComponent("ItemRect.qml");
        if (component.status === Component.Ready)
        {
            var itemrect = component.createObject(lyout);
            itemrect.text = text;
            itemrect.index = index;
            itemrect.mclicked.connect(mmclicked); //信号的关联槽函数
        }
    }
    Component.onCompleted:
    {
        addItem("Line",0);
        addItem("Pie",1);
    }

    //事件处理
    function mmclicked(index)
    {
        stackMain.currentIndex = index;
        for (var i in lyout.children)
        {
            if (lyout.children[i].index != index)
                lyout.children[i].clearColor();
        }
    }
}
