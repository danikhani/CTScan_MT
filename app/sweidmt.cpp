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
    ui ->tabWidget->addTab(m_pImageLoader, "LoadImage");
    m_pImageLoader->setData(&m_Data);

    connect(m_pImageLoader, SIGNAL(LOG(QString)), this, SLOT(LOG(QString)));


}

sweidmt::~sweidmt()
{
    delete ui;
    // this deletes the widget object
    delete m_pImageLoader;
}

void sweidmt::LOG(QString str) {
    ui->textEdit->append(str);
}

