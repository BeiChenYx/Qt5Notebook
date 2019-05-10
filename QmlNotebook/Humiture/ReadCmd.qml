import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.4
import an.qt.HandleHumiture 1.0

Rectangle {
    color: "transparent";

    HandleHumiture {
        id: readCmdHanle;
    }

    GridLayout {
       rows: 5;
       columns: 6;
       rowSpacing: 10;
       columnSpacing: 10;

       ULabel {
           Layout.topMargin: 30;
           Layout.row: 0; Layout.column: 0;
           Layout.columnSpan: 6;
           text: "读取温湿度指令(03指令): 本产品只用03指令读取参数";
       }

       ULabel {
           Layout.row: 1; Layout.column: 0;
           text: "设备地址:";
       }
       UTextField {
           id: deviceAddr;
           Layout.row: 1; Layout.column: 1;
           placeholderText: "01";
       }

       ULabel {
           Layout.row: 1; Layout.column: 2;
           text: "寄存器起始地址:";
       }
       UTextField {
           id: registerAddr;
           Layout.row: 1; Layout.column: 3;
           placeholderText: "0000";
       }

       ULabel {
           Layout.row: 1; Layout.column: 4;
           text: "寄存器个数:";
       }
       UTextField {
           id: registerLen;
           Layout.row: 1; Layout.column: 5;
           placeholderText: "0001";
       }

       ULabel {
           Layout.row: 2; Layout.column: 0;
           text: "发送:"
       }
       UTextField {
           Layout.row: 2; Layout.column: 1;
           Layout.columnSpan: 5;
           implicitWidth: 500;
           placeholderText: "010300000001";
       }
       ULabel {
           Layout.row: 3; Layout.column: 0;
           text: "响应:";
       }
       UTextField {
           id: dataField;
           Layout.row: 3; Layout.column: 1;
           Layout.columnSpan: 5;
           implicitWidth: 500;
       }

       UButton {
           Layout.row: 4; Layout.column: 5;
           text: "读取";
           onClicked: {
               var devAddr = parseInt(deviceAddr.text);
               var regAddr = parseInt(registerAddr.text);
               var regLen = parseInt(registerLen.text);
               readCmdHanle.onReadCmd(devAddr, regAddr, regLen);
           }
       }
    }

    Connections {
        target: readCmdHanle;
        onReadCmd: {
            dataField.text = data;
        }
    }
}
