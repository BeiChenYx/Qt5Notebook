import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
    width: 320;
    height: 480;
    color: gray;

    focus: true;
    Keys.enabled: true;
    Keys.onEscapePressed: {
        Qt.quit();
    }
    // 表示传递按键事件给列表内的对象，如果某个对象处理了按键则不会继续传递
    Keys.forwardTo: [moveText, likeQt];

    Text {
        id: moveText;
        x: 20;
        y: 20;
        width: 200;
        height: 30;
        text: qsTr("Moving Text");
        color: "blue";
        font {
            bold:	true;
            pixelSize: 24;
        }
        // 如果想让某个元素处理按键，则需要把焦点给它
        Keys.enabled: true;
        Keys.onPressed: {
            switch(event.key){
            case Qt.Key_Left:
                x -= 10;
                break;
            case Qt.Key_Right:
                x += 10;
                break;
            case Qt.Key_Down:
                y += 10;
                break
            case Qt.Key_Up:
                y -= 10;
                break;
            default:
                return;
            }
            // 接收处理按键，则事件不会继续下传
            event.accepted = true;
        }
    }

    CheckBox {
        // CheckBox 控件本身已经处理了空格键
        id: likeQt;
        text: "Like Qt Quick";
        anchors.left: parent.left;
        anchors.leftMargin: 10;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 10;
        z: 1;
    }
}
