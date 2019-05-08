import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4 as OldControl

Rectangle {
    implicitHeight: 30;
    implicitWidth: 100;
    color: "transparent";

    TextField {
        id: dateTimeBtn;
        anchors.centerIn: parent;
        placeholderText: qsTr("双击选择日期")
        focus: true;
        font.pixelSize: 12;
        color: "#59BAF2";

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 30
            color: "transparent";
            border.color: "#3270E1";
        }
    }

    MouseArea {
        anchors.fill: parent;
        // 弹出日历
        onClicked: calPopup.open();
    }

    Popup {
        id: calPopup;
        modal: true;
        focus: true;
        closePolicy: Popup.CloseOnEscape;
        x: dateTimeBtn.x;
        y: dateTimeBtn.y + dateTimeBtn.height;
        OldControl.Calendar {
            minimumDate: new Date(2015, 1, 1)
            maximumDate: new Date();

            onDoubleClicked: {
                dateTimeBtn.text = date.toLocaleDateString(Qt.locale(), "yyyy-MM-dd");
                calPopup.close();
            }
        }

    }

}
