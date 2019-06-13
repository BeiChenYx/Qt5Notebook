import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.4

Window {
    id: root;
    visible: true;
    width: 960; height: 540;

//    flags: Qt.Window | Qt.FramelessWindowHint;

    BorderImage {
        id: background;
        source: "qrc:/image/background.jpg";
        width: root.width; height: root.height;
        border.left: 0; border.top: 0;
        border.right: 0; border.bottom: 0;
    }

    GridLayout {
        id: mainLayout;
        anchors.fill: parent;
        rows: 10; columns: 1;
        rowSpacing: 0; columnSpacing: 0;

        // 标题栏
        Rectangle {
            Layout.rowSpan: 1;
            Layout.columnSpan: 1;
            Layout.row: 0; Layout.column: 0;
            Layout.fillHeight: true; Layout.fillWidth: true;
            Layout.minimumWidth: 960; Layout.maximumWidth: 1920;
            Layout.minimumHeight: 75; Layout.maximumHeight: 145;
            Layout.preferredWidth: 960; Layout.preferredHeight: 106;
            color: "transparent";
            BorderImage {
                id: titleImg;
                source: "qrc:/imge/title.png";
                width: root.width; height: root.height;
                border.left: 0; border.top: 0;
                border.right: 0; border.bottom: 0;
            }

        }
        GridLayout {
            rows: 1; columns: 7;
            rowSpacing: 0; columnSpacing: 0;
            // 左导航栏
            Rectangle {
                Layout.rowSpan: 1;
                Layout.columnSpan: 3;
                Layout.row: 0; Layout.column: 0;
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                Layout.preferredWidth: 100;
                color: "yellow";

            }

            // 内容区
            Rectangle {
                Layout.rowSpan: 1;
                Layout.columnSpan: 4;
                Layout.row: 0; Layout.column: 3;
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                Layout.preferredWidth: 600;
                color: "green";

            }
        }

    }
}
