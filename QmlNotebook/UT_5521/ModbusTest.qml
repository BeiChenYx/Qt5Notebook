import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Rectangle {
    color: "transparent";
    GridLayout {
        columns: 7;
        rows: 9;
        rowSpacing: 10;
        columnSpacing: 10;

        ULabel {
            Layout.topMargin: 30;
            Layout.row: 0;
            Layout.column: 0;
            Layout.columnSpan: 7;
            text: "读取温湿度指令(04命令): 用来读取温湿度"
        }

        // 读取的温湿度显示
        ULabel {
            Layout.row: 1; Layout.column: 0;
            text: "反馈温度:";
        }
        UTextField {
            Layout.row: 1; Layout.column: 1;
        }
        ULabel{
            Layout.row: 1; Layout.column: 2;
            text: "°C";
        }

        ULabel {
            Layout.row: 1; Layout.column: 3;
            text: "反馈湿度:";
        }
        UTextField{
            Layout.row: 1; Layout.column: 4;
        }
        ULabel {
            Layout.row: 1; Layout.column: 5;
            text: "%RH";
        }

        // 单次读取配置
        ULabel {
            Layout.row: 2; Layout.column: 0;
            text: "设备地址:";
        }
        UTextField {
            Layout.row: 2; Layout.column: 1;
            placeholderText: "01";
        }

        ULabel {
            Layout.row: 2; Layout.column: 3;
            text: "寄存器地址:";
        }
        UTextField{
            Layout.row: 2; Layout.column: 4;
            placeholderText: "0000";
        }

        ULabel {
            Layout.row: 3; Layout.column: 0;
            text: "寄存器长度:";
        }
        UTextField {
            Layout.row: 3; Layout.column: 1;
            placeholderText: "0001";
        }

        UButton {
            Layout.row: 3; Layout.column: 3;
            text: "读取";
        }

        // 循环读取
        ULabel {
            Layout.row: 4; Layout.column: 0;
            text: "循环读取:";
        }
        ULabel {
            Layout.row: 5; Layout.column: 0;
            text: "开始地址:";
        }
        UTextField {
            Layout.row: 5; Layout.column: 1;
            placeholderText: "01";
        }
        ULabel {
            Layout.row: 5; Layout.column: 3;
            text: "结束地址";
        }
        UTextField {
            Layout.row: 5; Layout.column: 4;
            placeholderText: "16";
        }
        ULabel {
            Layout.row: 6; Layout.column: 0;
            text: "发送:";
        }
        UTextField {
            Layout.row: 6;
            Layout.column: 1;
            Layout.columnSpan: 6;
            placeholderText: "010400000001";
            implicitWidth: 335;
        }

        ULabel {
            Layout.row: 7; Layout.column: 0;
            text: "响应:"
        }
        UTextField {
            Layout.row: 7;
            Layout.column: 1;
            Layout.columnSpan: 6;
            implicitWidth: 335;
        }
        ULabel {
            Layout.row: 8; Layout.column: 0;
            text: "循环间隔(ms):";
        }
        UTextField {
            Layout.row: 8; Layout.column: 1;
            text: "1000";
        }
        UButton {
            Layout.row: 8; Layout.column: 3;
            text: "连续读";
        }

    }
}
