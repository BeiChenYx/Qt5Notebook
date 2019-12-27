#include "frmother.h"
#include "ui_frmother.h"

#include "frmpanelmovewidget.h"
#include "frmpanelitem.h"
#include "frmpanelwidget.h"
#include "frmlcddatetime.h"
#include "frmlcdnumber.h"
#include "frmlednumber.h"
#include "frmledtext.h"
#include "frmdevicesizetable.h"
#include "frmcpumemorylabel.h"
#include "frmtextmovewidget.h"
#include "frmverifiedcode.h"
#include "frmiconeditor.h"
#include "frmipaddress.h"
#include "frmflatui.h"
#include "frmcombobox.h"

frmOther::frmOther(QWidget *parent) : QWidget(parent), ui(new Ui::frmOther)
{
    ui->setupUi(this);
    this->initForm();
}

frmOther::~frmOther()
{
    delete ui;
}

void frmOther::initForm()
{
    ui->stackedWidget->addWidget(new frmPanelMoveWidget);
    ui->stackedWidget->addWidget(new frmPanelItem);
    ui->stackedWidget->addWidget(new frmPanelWidget);
    ui->stackedWidget->addWidget(new frmLcdDateTime);
    ui->stackedWidget->addWidget(new frmLcdNumber);
    ui->stackedWidget->addWidget(new frmLedNumber);
    ui->stackedWidget->addWidget(new frmLedText);
    ui->stackedWidget->addWidget(new frmDeviceSizeTable);
    ui->stackedWidget->addWidget(new frmCpuMemoryLabel);
    ui->stackedWidget->addWidget(new frmTextMoveWidget);
    ui->stackedWidget->addWidget(new frmVerifiedCode);
    ui->stackedWidget->addWidget(new frmIconEditor);
    ui->stackedWidget->addWidget(new frmIPAddress);
    ui->stackedWidget->addWidget(new frmFlatUI);
    ui->stackedWidget->addWidget(new frmComboBox);
}

void frmOther::setIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
