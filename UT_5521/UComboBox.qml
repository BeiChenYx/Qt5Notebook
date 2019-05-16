import QtQuick 2.0
import QtQuick.Controls 2.0

ComboBox {
    id: addrComboBox
    implicitWidth: 80;
    background: Rectangle {
        color: "transparent";
        border.color: "#3270E1";
        radius: addrComboBox.width / 10;
    }
    delegate: ItemDelegate {
        width: addrComboBox.width;
        contentItem: Text {
            text: modelData;
            color: "#59BAF2";
            font: addrComboBox.font;
            elide: Text.ElideRight;
            verticalAlignment: Text.AlignVCenter;
        }
        highlighted: addrComboBox.highlightedIndex === index;
        background: Rectangle{
            color: hovered ? "#3270E1" : "transparent";
        }
    }
    contentItem: Text {
        text: addrComboBox.displayText;
        font: addrComboBox.font;
        color: addrComboBox.pressed ? "#17a81a" : "#21be2b";
        verticalAlignment: Text.AlignVCenter;
        horizontalAlignment: Text.AlignHCenter;
        elide: Text.ElideRight;
    }
    popup: Popup {
        y: addrComboBox.height - 1
        width: addrComboBox.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: addrComboBox.popup.visible ? addrComboBox.delegateModel : null
            currentIndex: addrComboBox.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#21be2b"
            radius: 2
            color: "transparent";
        }
    }
}
