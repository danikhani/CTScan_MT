#include "sweidmt.h"
#include "ui_sweidmt.h"

SWEIDMT::SWEIDMT(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SWEIDMT)
{
    ui->setupUi(this);
}

SWEIDMT::~SWEIDMT()
{
    delete ui;
}
