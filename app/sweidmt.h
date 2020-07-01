#ifndef SWEIDMT_H
#define SWEIDMT_H

#include <QWidget>
#include "imageloader.h"
#include "applicationdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class sweidmt; }
QT_END_NAMESPACE

class sweidmt : public QWidget
{
    Q_OBJECT

public:
    sweidmt(QWidget *parent = nullptr);
    ~sweidmt();


private:
    Ui::sweidmt *ui;
    //zeiger for the widget
    ImageLoader *m_pImageLoader;
    ApplicationData m_Data;
    //image3D imageData3D;

private slots:
    void LOG(QString str);

};
#endif // SWEIDMT_H
