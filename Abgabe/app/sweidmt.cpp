#include "sweidmt.h"
#include "ui_sweidmt.h"

sweidmt::sweidmt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sweidmt)
{
    ui->setupUi(this);
    //this will make the widget object
    m_pImageLoader = new ImageLoader(this);
    // to point the widget zeiger to the object
    ui ->tabWidget->addTab(m_pImageLoader, "Bore Viewer");
    m_pImageLoader->setData(&m_Data);

    connect(m_pImageLoader, SIGNAL(LOG_State(QString)), this, SLOT(LOG_State(QString)));
    connect(m_pImageLoader, SIGNAL(LOG_Instructions(QString)), this, SLOT(LOG_Instructions(QString)));


}

sweidmt::~sweidmt()
{
    delete ui;
    // this deletes the widget object
    delete m_pImageLoader;
}

void sweidmt::LOG_State(QString str) {
    ui->textEdit_Error->append(str);
}
void sweidmt::LOG_Instructions(QString str) {
    ui->textEdit_Steps->append(str);
}
