import QtQuick 2.9
import QtQuick.Window 2.2

/***********************************************************************
* 扩展QML的方式：
* 1. 上下文属性 - setContextProperty()
* 2. 引擎注册类型 - 在 main.cpp 中调用 qmlRegisterType
* 3. QML 扩展插件
*
* 上下文属性:
* 上下文属性使用对于小型的应用程序使用非常方便。不需要做太多的事情就可以将
* 系统编程接口暴露为友好的全局对象。有助于确保不会出现命名冲突.
*
* 引擎注册类型:
* 注册 QML 类型允许用户从 QML 中控制一个 C++ 对象的生命周期。上下文属性无法完成这件事情。
* 也不会破坏全局命名空间。所有的类型都需要先注册，并且在程序启动时会链接所有库。
*
* QML 扩展插件:
* QML 扩展插件提供了最灵活的扩展系统。允许在插件中注册类型，在第一个 QML 文件调用导入
* 鉴定时会加载这个插件。由于使用了一个 QML 单例这也不会再破坏全局命名空间。插件允许跨项目
* 重用模块，这在使用 Qt 包含多个项目时非常方便.
*
* 插件是一个已定义接口的库，它只在需要时才被加载。这与一个库在程序启动时被链接和加载不同j，
* 在 QML 场景下， 这个接口叫做 QQmlExtensionPlugin . 有两个方法需要关注:
* initializeEngine() 和 registerTypes()
* 当插件被加载时，首先会调用 initializeEngine(), 它允许我们访问引擎将插件对象暴露j给
* 根上下文：
* 大多数时候只会使用到 registerTypes() 方法，它允许你注册自定义的 QML 类型到引擎提供
* 的地址上.
*
************************************************************************/


Window {
    visible: true
    width: 640
    height: 480
//    title: qsTr("Hello World")
    Text {
        id: text
        anchors.centerIn: parent
        text: qsTr("Hello World!")
    }
}
