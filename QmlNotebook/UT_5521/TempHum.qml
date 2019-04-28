// 温湿度监控平台的主页
import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: root;
    visible: true;
    // width / height  = 16 / 9, 因为图片都是这个缩放比例
    height: 425;
    width: 640;
    // 去掉标题栏，自定义标题栏
    flags: Qt.Window | Qt.FramelessWindowHint;
    visibility: Window.Maximized;

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
                    root.width = root.width - delta.x;
                    root.height = root.height - delta.y
                    root.setX(root.x + delta.x)
                    root.setY(root.y + delta.y)
                    break;
                case 2:
                    root.width = root.width - delta.x;
                    root.setX(root.x + delta.x)
                    break;
                case 3:
                    root.width = root.width - delta.x;
                    root.height = root.height + delta.y;
                    root.setX(root.x + delta.x)
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 4:
                    root.height = root.height - delta.y
                    root.setY(root.y + delta.y)
                    break;
                case 6:
                    root.height = root.height + delta.y;
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 7:
                    root.width = root.width + delta.x;
                    root.height = root.height - delta.y;
                    root.setY(root.y + delta.y)
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 8:
                    root.width = root.width + delta.x;
                    clickPos = Qt.point(mouse.x, mouse.y)
                    break;
                case 9:
                    root.width = root.width + delta.x;
                    root.height = root.height + delta.y;
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
                else if(mouseX > step && mouseX < root.width - step){
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
                else if(mouseX >= root.width - step && mouseX <= root.width){
                    // 九宫格中间第三列的三行, 第三列第三行的运行结果鼠标显示切换不明显，故范围扩大到 2 * step
                    if(mouseY <= step && mouseY > 0){
                        mouseState = 7;
                        rootMouseArea.cursorShape = Qt.SizeBDiagCursor;
                    }
                    else if(mouseY > step && mouseY < root.height - 2 * step){
                        mouseState = 8;
                        rootMouseArea.cursorShape = Qt.SizeHorCursor;
                    }
                    else if(mouseY >= root.height - 2 * step && mouseY <= root.height){
                        mouseState = 9;
                        rootMouseArea.cursorShape = Qt.SizeFDiagCursor;
                    }
                }
            }
            mouse.accepted = true;
        }
    }

    GridLayout {
        // 栅格布局管理器来管理标题栏 和 内容区域
        // 第一行为标题栏, 占两列
        // 第二行为 左导航栏 和 内容区域
        id: rootGL;
        rows: 2
        columns: 2;
        anchors.fill: parent;

        // 标题栏
        GridLayout {
            id: titleBarLY;
            rows: 1;
            columns: 1;
            Layout.columnSpan: 2;

            // 标题名称背景图, LOGO，最小化，最大化，关闭按钮
            Rectangle {
                id: titleBar;
                width: root.width;
                height: root.height * (3 / 26) + 20;
                color: "transparent";

                // 背景图
                Image {
                    id: titleImg;
                    width: root.width - 20;
                    height: titleImg.width * (106 / 1880);
                    source: "./image/title.png";
                    fillMode: Image.Stretch;
                    anchors.left: parent.left;
                    anchors.leftMargin: 10;
                    anchors.top: parent.top;
                    anchors.topMargin: 10;
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
                    anchors.leftMargin: 40 * (root.width / 1366);
                    anchors.top: parent.top;
                    anchors.topMargin: 75 * ( root.height / 768);
                    z: 1;
                }

                Rectangle {
                    id: titlebar;
                    width: 76;
                    height: 20;
                    anchors.right: parent.right;
                    anchors.top: parent.top;
                    anchors.topMargin: 35 * (root.height / 768);
                    color: "transparent";
                    property bool isMax: true;

                    // 最小化, 最大化, 关闭按钮
                    Button {
                        id: closeButton;
                        width: 20; height: 20;
                        anchors.right: parent.right;
                        anchors.rightMargin: 8;
                        anchors.top: parent.top;
                        anchors.topMargin: 2;
                        ToolTip.text: "关闭";
                        ToolTip.visible: hovered;

                        onClicked: {
                            // TODO: 增加相应的退出处理
                            Qt.quit();
                        }

                        background: Rectangle {
                            implicitWidth: 20; implicitHeight: 20;
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
                        width: 20; height: 20;
                        anchors.right: closeButton.left;
                        anchors.rightMargin: 8;
                        anchors.top: closeButton.top;
                        ToolTip.text: parent.isMax ? "还原" : "最大化";
                        ToolTip.visible: hovered;
                        Image {
                            id: btnImage;
                            fillMode: Image.Stretch;
                            width: parent.width; height: parent.height;
                            source: "./image/reset.png";
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
                        width: 20; height: 20;
                        anchors.right: maxButton.left;
                        anchors.rightMargin: 8;
                        anchors.top: closeButton.top;
                        ToolTip.text: "最小化";
                        ToolTip.visible: hovered;
                        background: Rectangle {
                            implicitWidth: 20; implicitHeight: 20;
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
        }
        // 内容区域
        RowLayout {
            id: containLY;
            Layout.columnSpan: 2;
            implicitWidth: root.width;
            implicitHeight: root.height * (23 / 26);

            // 按钮导航栏
            Rectangle {
                id: leftNav;
                width: 380 * (root.width / 1366);
                height: root.height - titleBar.height;
                anchors.bottom: root.bottom;
                anchors.left: root.left;
            }

            // 主页显示区, 设置和曲线的显示
            Rectangle {
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

    // 定义左导航栏的按钮
    Component {
        id: leftNavBtn;
        Rectangle{
            property string imgPath: "";
            Image {
                id: titleImg;
//                width: root.width - 20;
//                height: titleImg.width * (106 / 1880);
                source: imgPath;
                fillMode: Image.Stretch;
            }
        }
    }

}
