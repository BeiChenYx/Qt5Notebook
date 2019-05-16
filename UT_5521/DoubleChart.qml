import QtQuick 2.12
import QtCharts 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.4

/*
 * 使用 StackView 的方式切换曲线配置和曲线展示的页面,
 * 曲线的配置主要有 设备选择 和 日期选择，日期如果为当前日期
 * 则显示当前实时数据，如果是历史日期，则从数据库中获取历史数据；
 */
Rectangle {
    id: spline;
    color: "transparent";
    implicitWidth: 500;
    implicitHeight: 400;
    property int currentIndex: 1;
    //    property var dynamicObjects: new Array();

    SwipeView{
        id: containerView;
        anchors.fill: parent;
        interactive: false;
        clip: true;
    }
    Component.onCompleted: {
        // 初始化 SwipeView 根据底层设备个数，默认16个
        // 直接在 SwipeView 中添加16个视图会导致缩放的过程很卡
        // 因此采用添加一个就删除一个的方式
        gerneratorChart(currentIndex);
    }
    function onPreView(index){
        if(index <= 1){
            return;
        }else{
            containerView.takeItem(0);
            currentIndex -= 1;
            gerneratorChart(currentIndex);
        }

    }
    function onNextView(index){
        if(index >= 16){
            return;
        }else{
            containerView.takeItem(0);
            currentIndex += 1;
            gerneratorChart(currentIndex);
        }
    }
    function gerneratorChart(index){
        var chartComponent = Qt.createComponent("qrc:/LineChart.qml");
        if(chartComponent.status === Component.Ready){
            var deviceChart = chartComponent.createObject(containerView)
            deviceChart.initChart("设备地址为" + index + "的温湿度曲线", index)
            deviceChart.preView.connect(spline.onPreView);
            deviceChart.nextView.connect(spline.onNextView);
            containerView.addItem(deviceChart);
            //                dynamicObjects[i] = deviceChart;
        }
        containerView.setCurrentIndex(0);
    }

}
