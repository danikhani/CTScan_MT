#include "sweidmt.h"
#include "ui_sweidmt.h"

SWEIDMT::SWEIDMT(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SWEIDMT)
{
    ui->setupUi(this);
    //this will make the widget object
    m_pWidget = new ImageLoader(this);
    // to point the widget zeiger to the object
    ui ->tabWidget->addTab(m_pWidget, "LoadImage");
    m_pImageLoader = new ApplicationData(this);
    m_pImageLoader->setData(&m_Data);


}

SWEIDMT::~SWEIDMT()
{
    delete ui;
    // this deletes the widget object
    delete m_pWidget;
}
