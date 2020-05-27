#ifndef SWEIDMT_H
#define SWEIDMT_H

#include <QWidget>
#include "imageloader.h"

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
    ImageLoader *m_pWidget;
    //ApplicationData m_data;
};
#endif // SWEIDMT_H