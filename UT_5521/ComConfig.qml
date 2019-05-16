import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.4
import an.qt.HandleHumiture 1.0

Rectangle {
    color: "transparent";

    HandleHumiture {
        id: comHanle;
    }

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
            model: ["COM1", "COM2", "COM3",
                "COM4", "COM5", "COM6", "COM7",
                "COM8", "COM9", "COM10", "COM11"]
        }

        ULabel {
            Layout.row: 2; Layout.column: 0;
            text: "波特率"
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
            onClicked: {
                if(openClose.text == "打开"){
                    var currentCom = com.currentText;
                    var currentbaude = parseInt(baude.currentText);
                    comHanle.onOpenCom(currentCom, currentbaude);
                }else{
                    comHanle.onCloseCom();
                }

            }
        }

        // 占坑
        ULabel {
            width: 100;
            Layout.row: 0; Layout.column: 2;
        }


        // 反馈的状态
        ULabel {
            id: comStatus;
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
            id: baudrate;
            Layout.row: 1; Layout.column: 4;
        }

        ULabel {
            Layout.row: 2; Layout.column: 3;
            text: "停止位:";
        }
        ULabel {
            id: stop;
            Layout.row: 2; Layout.column: 4;
        }

        ULabel {
            Layout.row: 3; Layout.column: 3;
            text: "校验位:";
        }
        ULabel {
            id: paritybits;
            Layout.row: 3; Layout.column: 4;
        }

        ULabel {
            Layout.row: 4; Layout.column: 3;
            text: "数据位:";
        }
        ULabel {
            id: data;
            Layout.row: 4; Layout.column: 4;
        }
    }

    Component.onCompleted: {
        comHanle.comOpenResult.connect(updateOpenResult);
        comHanle.comCloseResult.connect(updateCloseResult);
    }
    function updateOpenResult(result, baud, stopbits, parity, databits){
        console.log(baud, stopbits, parity, databits);
        if(result){
            baudrate.text = baud.toString();
            stop.text = stopbits.toString();
            paritybits.text  = parity;
            data.text = databits.toString();
            openClose.text = "关闭";
            comStatus.text = "串口状态: 已打开";
        }else{
            comStatus.text = "串口状态: 打开失败";
        }
    }

    function updateCloseResult(){
        openClose.text = "打开";
        comStatus.text = "串口状态: ";
    }
}
