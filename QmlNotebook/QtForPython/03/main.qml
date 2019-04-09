/*
 * 在QML中，我们可以绑定到numberGenerator对象的number和maxNumber属性。
 * 在按钮的onClicked处理程序中，我们调用updateNumber方法来生成一个新的
 * 随机数，在滑块的onValueChanged处理程序中，我们使用setMaxNumber方法.
 * 这是因为直接通过Javascript修改属性会破坏绑定到属性。
 * 通过显式地使用setter方法，可以避免这种情况
*/
import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0

Window {
	id: root;

	width: 640;
	height: 480;
	visible: true;
	title: "Hello Python World";

	Column {
		Flow {
			Button {
				text: "Give me a number!"
				onClicked: numberGenerator.updateNumber();
			}
			Label {
				id: numberLabel
				text: numberGenerator.number
			}
		}
		Flow {
			Slider {
				from: 0
				to: 99
				value: numberGenerator.maxNumber
				onValueChanged: numberGenerator.setMaxNumber(value)
			}
		}
	}
}