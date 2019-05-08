import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Rectangle {
    color: "transparent";
    GridLayout {
        rows: 3;
        columns: 2;
        rowSpacing: 10;

        ULabel {
            Layout.topMargin: 30;
            text: "串口号";
        }
        UComboBox {
            Layout.topMargin: 30;
            id: com;
        }

        ULabel {
            text: "波特率";
        }
        UComboBox {
            id: baude;
            model: [1200, 2400, 4800, 9600, 19200];
        }
        UButton {
            id: openClose;
            Layout.row: 2;
            Layout.column: 1;
            text: "打开";
        }
    }
}
