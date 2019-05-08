import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Rectangle {
    color: "transparent";
    GridLayout {
        rows: 5;
        columns: 5;
        rowSpacing: 10;
        columnSpacing: 10;

        ULabel {
            text: "串口配置:";
            Layout.row: 0; Layout.column: 0;
            Layout.columnSpan: 2;
            Layout.topMargin: 30;
        }

        ULabel {
            Layout.row: 1; Layout.column: 0;
            text: "串口号";
        }
        UComboBox {
            Layout.row: 1; Layout.column: 1;
            id: com;
            implicitWidth: 100;
        }

        ULabel {
            Layout.row: 2; Layout.column: 0;
            text: "波特率";
        }
        UComboBox {
            Layout.row: 2; Layout.column: 1;
            id: baude;
            model: [1200, 2400, 4800, 9600, 19200];
            implicitWidth: 100;
        }
        UButton {
            Layout.row: 4; Layout.column: 1;
            id: openClose;
            implicitWidth: 100;
            text: "打开";
        }

        // 占坑
        ULabel {
            width: 100;
            Layout.row: 0; Layout.column: 2;
        }


        // 反馈的状态
        ULabel {
            text: "串口状态:";
            Layout.topMargin: 30;
            Layout.row: 0; Layout.column: 3;
            Layout.columnSpan: 2;
        }
        ULabel {
            Layout.row: 1; Layout.column: 3;
            text: "波特率:";
        }
        ULabel{
            Layout.row: 1; Layout.column: 4;
        }

        ULabel {
            Layout.row: 2; Layout.column: 3;
            text: "停止位:";
        }
        ULabel {
            Layout.row: 2; Layout.column: 4;
        }

        ULabel {
            Layout.row: 3; Layout.column: 3;
            text: "校验位:";
        }
        ULabel {
            Layout.row: 3; Layout.column: 4;
        }

        ULabel {
            Layout.row: 4; Layout.column: 3;
            text: "数据位:";
        }
        ULabel {
            Layout.row: 4; Layout.column: 4;
        }
    }
}
