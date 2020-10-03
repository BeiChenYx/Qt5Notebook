#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QRegExpValidator>
#include "FrameLessWidget/framelesswidget.h"
#include "setfilter.h"
#include "ui_setfilter.h"

SetFilter::SetFilter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetFilter)
{
    ui->setupUi(this);
    ui->tableWidget_filter_com->setColumnWidth(3, 200);
    ui->tableWidget_filter_com->setColumnWidth(4, 200);
    QString regStr("[0-9a-fA-F ]{0,11}");
    QStringList typeStr({tr("标准帧单ID滤波"), tr("标准帧组ID滤波"), tr("扩展帧单ID滤波"), tr("扩展帧组ID滤波")});
    for (int i=0; i<10; ++i) {
        int rowCom = ui->tableWidget_filter_com->rowCount();
        ui->tableWidget_filter_com->insertRow(rowCom);
        // 序号
        QTableWidgetItem *itemIdCom = new QTableWidgetItem(QString("%1").arg(rowCom+1));
        itemIdCom->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_filter_com->setItem(rowCom, 0, itemIdCom);
        // 使能
        QCheckBox *check = new QCheckBox();
        ui->tableWidget_filter_com->setCellWidget(rowCom, 1, check);
        // 类型
        QComboBox *typeCom = new QComboBox(this);
        typeCom->addItems(typeStr);
        ui->tableWidget_filter_com->setCellWidget(rowCom, 2, typeCom);
        // 开始ID
        QLineEdit *startCom = new QLineEdit();
        startCom->setPlaceholderText(tr("FA FB FC FD"));
        startCom->setValidator(new QRegExpValidator(QRegExp(regStr), this));
        ui->tableWidget_filter_com->setCellWidget(rowCom, 3, startCom);
        // 起始ID
        QLineEdit *endCom = new QLineEdit();
        endCom->setPlaceholderText(tr("FA FB FC FD"));
        endCom->setValidator(new QRegExpValidator(QRegExp(regStr), this));
        endCom->setEnabled(false);
        ui->tableWidget_filter_com->setCellWidget(rowCom, 4, endCom);
        connect(typeCom, QOverload<int>::of(&QComboBox::currentIndexChanged), [startCom, endCom](int index){
            if(index == 0 || index == 2){
                endCom->setEnabled(false);
                endCom->setText(startCom->text());
            }else {
                endCom->setEnabled(true);
            }
        });
        connect(startCom, &QLineEdit::textEdited, this, [typeCom, endCom](QString text){
            if(typeCom->currentIndex() == 0 || typeCom->currentIndex() == 2){
                endCom->setText(text);
            }
        });
    }
    ui->tableWidget_filter_com->setColumnWidth(1, 50);
    ui->tableWidget_filter_com->setColumnWidth(2, 150);
}

SetFilter::~SetFilter()
{
    delete ui;
}

void SetFilter::onFilters(QList<QStringList> &filter)
{
    if(!filter.isEmpty()){
        for (int index=0; index<filter.length(); index++) {
            auto row = filter.at(index);
            QByteArray start, end;
            uint32_t num1 = row.at(1).toUInt(), num2 = row.at(2).toUInt();
            if(!num1 || !num2){continue;}

            QComboBox *typeItem = static_cast<QComboBox *>(ui->tableWidget_filter_com->cellWidget(index, 2));
            QLineEdit *startItem = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(index, 3));
            QLineEdit *endItem = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(index, 4));
            typeItem->setCurrentIndex(row.at(0).toInt());
            startItem->setText(uintToQString(num1));
            endItem->setText(uintToQString(num2));
        }
    }
}

void SetFilter::onFilters(QVector<FilterConfig> &filterData)
{
    if(!filterData.isEmpty()){
        for (int index=0; index<filterData.length(); index++) {
            auto row = filterData.at(index);
            if(row.startID == "00 00 00 00" || row.endID == "00 00 00 00"){return;}
            QComboBox *typeItem = static_cast<QComboBox *>(ui->tableWidget_filter_com->cellWidget(index, 2));
            QLineEdit *startItem = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(index, 3));
            QLineEdit *endItem = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(index, 4));
            typeItem->setCurrentText(row.idFilterType);
            startItem->setText(row.startID);
            endItem->setText(row.startID);
        }
    }
}

void SetFilter::onClearText()
{
    for (int i=0; i<10; ++i) {
        QCheckBox *check = static_cast<QCheckBox*>(ui->tableWidget_filter_com->cellWidget(i, 1));
        QComboBox *typeItem = static_cast<QComboBox *>(ui->tableWidget_filter_com->cellWidget(i, 2));
        QLineEdit *startItem = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(i, 3));
        QLineEdit *endItem = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(i, 4));
        check->setChecked(false);
        typeItem->setCurrentIndex(0);
        startItem->setText("");
        endItem->setText("");
    }
}

void SetFilter::on_pushButton_filter_clicked()
{
    // 设置过滤规则
    if(m_isSerial){
        this->setSerialFilter();
    }else if(m_isHid){
        this->setHidFilter();
    }else {
        CustomInfoBox::customInfoBox(tr("操作错误"), tr("请至少先在设备连接选择一种方式打开CAN设备!"));
    }
}

void SetFilter::on_pushButton_clicked()
{
    // 读取过滤规则
    if(m_isSerial){
        m_pHandleSerial->readMcuConfig();
    }else if(m_isHid){
        emit readFilter();
    }else {
        CustomInfoBox::customInfoBox(tr("操作错误"), tr("请至少先在设备连接选择一种方式打开CAN设备!"));
    }
}

void SetFilter::on_pushButton_clear_filter_clicked()
{
    if(m_isSerial){
        m_pHandleSerial->clearfilter();
    }else if(m_isHid){
        this->clearHidFilter();
    }else {
        CustomInfoBox::customInfoBox(tr("操作错误"), tr("请至少先在设备连接选择一种方式打开CAN设备!"));
    }
    this->onClearText();
}

void SetFilter::setHidFilter()
{
    HidTaskPar par;
    int row = ui->tableWidget_filter_com->rowCount();
    for (int i=0; i<row; ++i) {
        QCheckBox * checkBox = static_cast<QCheckBox*>(ui->tableWidget_filter_com->cellWidget(i, 1));
        if(checkBox->isChecked()){
            QComboBox *type = static_cast<QComboBox *>(ui->tableWidget_filter_com->cellWidget(i, 2));
            QLineEdit *start = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(i, 3));
            QLineEdit *end= static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(i, 4));
            QStringList config;
            config.append(type->currentText());
            QString startStr = handleId(start->text().trimmed());
            QString endStr = handleId(end->text().trimmed());
            if(startStr.isEmpty() || endStr.isEmpty()){
                CustomInfoBox::customInfoBox(tr("帧ID不合法"), tr("帧ID错误, 正确格式: 0A FA 0B 12"));
                return;
            }

            bool ok;
            QString max = type->currentIndex() > 1 ? "1F FF FF FF" : "00 00 07 FF";
            QString errMsg = type->currentIndex() > 1 ? tr("扩展帧ID最大0x1FFFFFFF"): tr("标准帧ID最大0x07FF");
            if(startStr > max || endStr > max){
                CustomInfoBox::customInfoBox(tr("帧ID不合法"), errMsg);
                return;
            }
            auto startNum = startStr.replace(" ", "").toUInt(&ok, 16);
            auto endNum = endStr.replace(" ", "").toUInt(&ok, 16);
            if(startNum > endNum){
                CustomInfoBox::customInfoBox(tr("帧ID不合法"), tr("起始ID不能大于结束ID"));
                return;
            }
            config.append(startStr);
            config.append(endStr);
            par.filterData.append(config);
        }
    }
    if(par.filterData.size() == 0){
        CustomInfoBox::customInfoBox(tr("滤波操作"), tr("请至少勾选一个，或者使用清除滤波按钮!"));
        return;
    }
    m_pHidWorkThread->pushToTask(HidTask::FILTER_ID, par);
}

void SetFilter::setSerialFilter()
{
    // 设置滤波规则
    SerialTaskPar par;
    int row = ui->tableWidget_filter_com->rowCount();
    for (int i=0; i<row; ++i) {
        QCheckBox * checkBox = static_cast<QCheckBox*>(ui->tableWidget_filter_com->cellWidget(i, 1));
        if(checkBox->isChecked()){
            QComboBox *type = static_cast<QComboBox *>(ui->tableWidget_filter_com->cellWidget(i, 2));
            QLineEdit *start = static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(i, 3));
            QLineEdit *end= static_cast<QLineEdit *>(ui->tableWidget_filter_com->cellWidget(i, 4));
            FilterConfig config;
            config.idFilterType = type->currentText();
            QString startStr = handleId(start->text().trimmed());
            QString endStr = handleId(end->text().trimmed());
            if(startStr.isEmpty() || endStr.isEmpty()){
                CustomInfoBox::customInfoBox(tr("帧ID不合法"), tr("帧ID错误, 正确格式: 0A FA 0B 12"));
                return;
            }
            bool ok;
            QString max = type->currentIndex() > 1 ? "1F FF FF FF" : "00 00 07 FF";
            QString errMsg = type->currentIndex() > 1 ? tr("扩展帧ID最大0x1FFFFFFF"): tr("标准帧ID最大0x07FF");
            if(startStr > max || endStr > max){
                CustomInfoBox::customInfoBox(tr("帧ID不合法"), errMsg);
                return;
            }
            auto startNum = startStr.replace(" ", "").toUInt(&ok, 16);
            auto endNum = endStr.replace(" ", "").toUInt(&ok, 16);
            if(startNum > endNum){
                CustomInfoBox::customInfoBox(tr("帧ID不合法"), tr("起始ID不能大于结束ID"));
                return;
            }
            config.startID = startStr;
            config.endID = endStr;
            par.filterData.append(config);

        }
    }
    if(par.filterData.size() == 0){
        CustomInfoBox::customInfoBox(tr("滤波操作"), tr("请至少勾选一个，或者使用清除滤波按钮!"));
        return;
    }
    m_pHandleSerial->setFilter(par);
}


void SetFilter::clearHidFilter()
{
    m_pHidWorkThread->pushToTask(HidTask::CLEAR_FILTER_ID);
}

void SetFilter::onSerialStatus(bool status)
{
    m_isSerial = status;
}

void SetFilter::onHidStatus(bool status)
{
    m_isHid = status;
}
