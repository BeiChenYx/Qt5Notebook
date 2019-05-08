// 温湿度监控平台的主页
import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: root;
    visible: true;
    height: rootGL.implicitHeight;
    width: rootGL.implicitWidth;
    property int _width: 960;
    property int _height: 540;
    property double hRatio: width / _width;
    property double vRatio: height / _height;
    // 去掉标题栏，自定义标题栏
    flags: Qt.Window | Qt.FramelessWindowHint;

    BorderImage {
        id: name
        source: "./image/background.jpg";
        width: root.width; height: root.height;
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
    }

    // 处理 root 的鼠标动作
    MouseArea {
        id: rootMouseArea;
        anchors.fill: parent;
        acceptedButtons: Qt.LeftButton;
        hoverEnabled: true;
        /*
        * 将界面分成一个九宫格分别为: LeftTop Top RightTop Left Center Right
        * LeftBottom Bottom RightBottom, 鼠标在这九个区域内需要做特殊处理.
        * 边界缩放范围 step = 5, 也就是说鼠标在四边 step 范围内且没有按住左键
        * 的情况下，只改变鼠标形状，按住了左键然后移动鼠标，就需要处理缩放界面.
        * 如果在边界缩放范围外，且在界面内任何主界面(非子控件),都正常显示鼠标
        * 形状，如果按住左键并移动鼠标，则移动主界面，放开鼠标左键，则恢复鼠标形状
        */
        property point clickPos: "0,0"

        // 边框缩放的范围
        property int step: 10;
        // 是否点击
        property bool isClicked: false;
        // 鼠标的状态
        property int mouseState: 0
        // 最小宽高
        property int minWidthHeight:30;

        onDoubleClicked: {
            handleMaxButton();
            isClicked = false;
            mouse.accepted = true;
        }

        onPressed: {
            focus = true;
            isClicked = true;
            clickPos = Qt.point(mouse.x, mouse.y)
            mouse.accepted = true;
        }
        onReleased: {
            isClicked = false;
            mouse.accepted = true;
        }

        onPositionChanged: {
            if(isClicked)
            {
                //鼠标偏移量
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                switch(mouseState){
                case 0:
                case 5:
                    root.setX(root.x + delta.x)
                    root.setY(root.y + delta.y)
                    break;
                case 1:
                    // TODO: 缩放功能目前有bug,先不上线
//                    root.width = root.width - delta.x;
//                    root.height = root.height - delta.y
                    root.setX(root.x + delta.x)
                    root.setY(root.y + delta.y)
                    break;
                case 2:
//                    root.width = root.width - delta.x;
                    root.setX(root.x + delta.x)
                    break;
                case 3:
//                    root.width = root.width - delta.x;
//                    root.height = root.height + delta.y;
                    root.setX(root.x + delta.x)
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 4:
//                    root.height = root.height - delta.y
                    root.setY(root.y + delta.y)
                    break;
                case 6:
//                    root.height = root.height + delta.y;
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 7:
//                    root.width = root.width + delta.x;
//                    root.height = root.height - delta.y;
                    root.setY(root.y + delta.y)
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 8:
//                    root.width = root.width + delta.x;
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 9:
//                    root.width = root.width + delta.x;
//                    root.height = root.height + delta.y;
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                default:
                    break;
                }
                // 防止页面消失
                if(root.width < minWidthHeight){
                    root.width = minWidthHeight;
                }
                if(root.height < minWidthHeight){
                    root.height = minWidthHeight;
                }

            }
        /* TODO: 鼠标改变窗口大小的功能实现有bug, 在右下角经常无法触发鼠标，因此屏蔽这个功能
            else
            {
                // 没有按左键，只需要改变鼠标形状
                if(mouseX <= step && mouseX > 0){
                    // 九宫格左侧三个格子,宽度 step, 分别处理三个格子区域的鼠标形状
                    if(mouseY < step && mouseY > 0){
                        // 第一行
                        mouseState = 1;
                        rootMouseArea.cursorShape = Qt.SizeFDiagCursor;
                    }
                    else if(mouseY >= step && mouseY < root.height - step){
                        // 第二行
                        mouseState = 2;
                        rootMouseArea.cursorShape = Qt.SizeHorCursor;
                    }
                    else if(mouseY >= root.height - step && mouseY <= root.height){
                        // 第三行
                        mouseState = 3;
                        rootMouseArea.cursorShape = Qt.SizeBDiagCursor;
                    }
                }
                else if(mouseX > step && mouseX <= root.width - step){
                    // 九宫格中间第二列的三行
                    if(mouseY < step && mouseY > 0){
                        mouseState = 4;
                        rootMouseArea.cursorShape = Qt.SizeVerCursor;
                    }
                    else if(mouseY >= step && mouseY < root.height - step){
                        mouseState = 5;
                        rootMouseArea.cursorShape = Qt.ArrowCursor;
                    }
                    else if(mouseY >= root.height - step && mouseY <= root.height){
                        mouseState = 6;
                        rootMouseArea.cursorShape = Qt.SizeVerCursor;
                    }
                }
                else if(mouseX > root.width - step && mouseX <= root.width){
                    // 九宫格中间第三列的三行
                    if(mouseY <= step && mouseY > 0){
                        mouseState = 7;
                        rootMouseArea.cursorShape = Qt.SizeBDiagCursor;
                    }
                    else if(mouseY > step && mouseY < root.height - step){
                        mouseState = 8;
                        rootMouseArea.cursorShape = Qt.SizeHorCursor;
                    }
                    else if((mouseY >= (root.height - step)) && mouseY <= root.height){
                        mouseState = 9;
                        rootMouseArea.cursorShape = Qt.SizeFDiagCursor;
                    }
                }
            }
        */
            mouse.accepted = true;
        }
    }

    GridLayout {
        id: rootGL;
        anchors.fill: parent;
        rows: 2; columns: 2;
        rowSpacing: 0; columnSpacing: 0;

        // 标题名称背景图, LOGO，最小化，最大化，关闭按钮
        Rectangle {
            id: titleBar;
            Layout.rowSpan: 1; Layout.columnSpan: 2;
            Layout.fillHeight: true; Layout.fillWidth: true;
            Layout.minimumWidth: 960; Layout.maximumWidth: 1920;
            Layout.minimumHeight: 75; Layout.maximumHeight: 145;
            Layout.preferredWidth: 960; Layout.preferredHeight: 75;
            Layout.topMargin: 20;
            Layout.leftMargin: 15;
            Layout.rightMargin: 15;
            color: "transparent";

            // 背景图
            Image {
                id: titleImg;
                // 30 是量 root._width,root._height 的图片中背景图的
                // 左边加右边的距离,得出宽度，同理得出相对高度，以下都是
                // 类似计算方法;
                width: root.width - 30 * root.hRatio;
                height: 55 * root.vRatio;
                source: "./image/title.png";
                fillMode: Image.Stretch;
                anchors.left: parent.left;
                anchors.top: parent.top;
                z: 0;
            }
            // LOGO
            Image {
                id: logo;
                width: root.width * (568 / 1920);
                height: logo.width * (48 / 568);
                source: "./image/logo.png";
                fillMode: Image.Stretch;
                anchors.left: parent.left;
                anchors.leftMargin: 27 * root.hRatio;
                anchors.top: titleImg.top;
                anchors.topMargin: 32 * ( root.height / root._height);
                z: 1;
            }

            Rectangle {
                id: titlebar;
                width: 76;
                height: 20;
                anchors.right: parent.right;
                anchors.top: titleImg.top;
                anchors.topMargin: 20 * root.vRatio;
                color: "transparent";
                property bool isMax: false;

                // 最小化, 最大化, 关闭按钮
                Button {
                    id: closeButton;
                    width: 22 * root.hRatio; height:22 * root.vRatio;
                    anchors.right: parent.right;
                    anchors.rightMargin: 23 * root.hRatio;
                    anchors.top: parent.top;
                    ToolTip.text: "关闭";
                    ToolTip.visible: hovered;

                    onClicked: {
                        // TODO: 增加相应的退出处理
                        Qt.quit();
                    }

                    background: Rectangle {
                        color: (closeButton.hovered | closeButton.pressed) ? "red" : "transparent";
                        radius: width / 2;
                        BorderImage {
                            anchors.fill: parent;
                            source: "./image/close.png";
                        }
                    }
                }
                Button {
                    id: maxButton;
                    width: 22 * root.hRatio; height: 22 * root.vRatio;
                    anchors.right: closeButton.left;
                    anchors.rightMargin: 8 * root.hRatio;
                    anchors.top: closeButton.top;
                    ToolTip.text: parent.isMax ? "还原" : "最大化";
                    ToolTip.visible: hovered;
                    Image {
                        id: btnImage;
                        fillMode: Image.Stretch;
                        width: parent.width; height: parent.height;
                        source: "./image/max.png";
                    }
                    background: Rectangle{
                        color: "transparent";
                    }

                    onClicked: {
                        handleMaxButton();
                    }
                }
                Button {
                    id: minButton;
                    width: 22 * root.hRatio; height: 22 * root.vRatio;
                    anchors.right: maxButton.left;
                    anchors.rightMargin: 8 * root.hRatio;
                    anchors.top: closeButton.top;
                    ToolTip.text: "最小化";
                    ToolTip.visible: hovered;
                    background: Rectangle {
                        color: "transparent";
                        BorderImage {
                            anchors.fill: parent;
                            source: "./image/min.png";
                        }
                    }
                    onClicked: root.visibility =  Window.Minimized;
                }
            }
        }

        Rectangle {
            Layout.rowSpan: 1; Layout.columnSpan: 2;
            Layout.fillHeight: true; Layout.fillWidth: true;
            Layout.minimumWidth: 960; Layout.maximumWidth: 1920;
            Layout.minimumHeight: 465; Layout.maximumHeight: 935;
            Layout.preferredWidth: 960; Layout.preferredHeight: 465;
            color: "transparent";
            // 按钮导航栏
            Rectangle {
                id: leftNav;
                width: 300 * root.hRatio;
                height: 360 * root.vRatio;
                color: "transparent";

                Button {
                    id: displaybtn;
                    // 原图是 1920 * 1080 的图，Button 是 383 * 80的大小
                    width: 383 * (root.width / 1920);
                    height: 80 * (root.height / 1080);
                    anchors.right: parent.right;
                    anchors.rightMargin: 58 * root.hRatio;
                    anchors.top: parent.top;
                    anchors.topMargin: 30 * root.vRatio;
                    background: Rectangle {
                        color: (displaybtn.hovered | displaybtn.pressed) ? "#42A1FA" : "transparent";
                        radius: width / 2;
                        BorderImage {
                            anchors.fill: parent;
                            source: "./image/display.png";
                        }
                    }
                    onClicked: {
                        mainView.currentIndex = 0;
                    }
                }
                Button {
                    id: combtn;
                    // 原图是 1920 * 1080 的图，Button 是 383 * 80的大小
                    width: 383 * (root.width / 1920);
                    height: 80 * (root.height / 1080);
                    anchors.right: parent.right;
                    anchors.rightMargin: 58 * root.hRatio;
                    anchors.top: displaybtn.bottom;
                    anchors.topMargin: 40 * root.vRatio;
                    background: Rectangle {
                        color: (combtn.hovered | combtn.pressed) ? "#42A1FA" : "transparent";
                        radius: width / 2;
                        BorderImage {
                            anchors.fill: parent;
                            source: "./image/com.png";
                        }
                    }
                    onClicked: {
                        mainView.currentIndex = 1;
                    }
                }
                Button {
                    id: modbusbtn;
                    // 原图是 1920 * 1080 的图，Button 是 383 * 80的大小
                    width: 383 * (root.width / 1920);
                    height: 80 * (root.height / 1080);
                    anchors.right: parent.right;
                    anchors.rightMargin: 58 * root.hRatio;
                    anchors.top: combtn.bottom;
                    anchors.topMargin: 40 * root.vRatio;
                    background: Rectangle {
                        color: (modbusbtn.hovered | modbusbtn.pressed) ? "#42A1FA" : "transparent";
                        radius: width / 2;
                        BorderImage {
                            anchors.fill: parent;
                            source: "./image/modbus.png";
                        }
                    }
                    onClicked: {
                        mainView.currentIndex = 2;
                    }
                }
                Button {
                    id: readbtn;
                    // 原图是 1920 * 1080 的图，Button 是 383 * 80的大小
                    width: 383 * (root.width / 1920);
                    height: 80 * (root.height / 1080);
                    anchors.right: parent.right;
                    anchors.rightMargin: 58 * root.hRatio;
                    anchors.top: modbusbtn.bottom;
                    anchors.topMargin: 40 * root.vRatio;
                    background: Rectangle {
                        color: (readbtn.hovered | readbtn.pressed) ? "#42A1FA" : "transparent";
                        radius: width / 2;
                        BorderImage {
                            anchors.fill: parent;
                            source: "./image/read.png";
                        }
                    }
                    onClicked: {
                        mainView.currentIndex = 3;
                    }
                }
                Button {
                    id: writebtn;
                    // 原图是 1920 * 1080 的图，Button 是 383 * 80的大小
                    width: 383 * (root.width / 1920);
                    height: 80 * (root.height / 1080);
                    anchors.right: parent.right;
                    anchors.rightMargin: 58 * root.hRatio;
                    anchors.top: readbtn.bottom;
                    anchors.topMargin: 40 * root.vRatio;
                    background: Rectangle {
                        color: (writebtn.hovered | writebtn.pressed) ? "#42A1FA" : "transparent";
                        radius: width / 2;
                        BorderImage {
                            anchors.fill: parent;
                            source: "./image/write.png";
                        }
                    }
                    onClicked: {
                        mainView.currentIndex = 4;
                    }
                }
            }

            StackLayout {
                id: mainView;
                width: 600 * root.hRatio;
                height: 420 * root.vRatio;
                anchors.left: leftNav.right;

                Loader {
                    id: home;
                    source: "./Home.qml";
                }
                Loader {
                    id: comConfig;
                    source: "./ComConfig.qml";
                }
                Loader {
                    id: modbusTest;
                    source: "./ModbusTest.qml";
                }
            }

        }
    }

    // 主页的 js 全局函数
    function handleMaxButton(){
        if(titlebar.isMax){
            root.visibility =  Window.Windowed;
            titlebar.isMax = false;
            btnImage.source = "./image/max.png";
        }else{
            root.visibility =  Window.Maximized;
            titlebar.isMax = true;
            btnImage.source = "./image/reset.png";
        }
    }

    Component.onCompleted: {
        mainView.currentIndex = 0
    }

}
