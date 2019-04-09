import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0

Window {
	id: root;

	width: 640;
	height: 480;
	visible: true;
	title: "Hello Python World";

	Flow {
		Button {
			id: btn1
			text: "Give me a number!"
			// TODO: 还不知道暴露的对象怎么传参数进去，信号怎么传参数出来
			onClicked: numberGenerator.giveNumber();
		}
		Label {
			id: numberLabel
			text: "no number"
		}
	}
	// 信号参数名不会从Python传播到QML，因此我们需要这样做回送信号
	signal reNextNumber(int number)
	// Component 具体见 Qt 文档， 直接索引 Component.
	Component.onCompleted: numberGenerator.nextNumber.connect(reNextNumber)
	// Connections 具体见 Qt 文档, 索引 Connections
	Connections {
		target: root
		onReNextNumber: numberLabel.text = number
	}
}