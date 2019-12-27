#include "frmtiledbg.h"
#include "ui_frmtiledbg.h"

frmTiledBg::frmTiledBg(QWidget *parent) : QWidget(parent), ui(new Ui::frmTiledBg)
{
    ui->setupUi(this);
}

frmTiledBg::~frmTiledBg()
{
    delete ui;
}
