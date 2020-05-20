#ifndef SWEIDMT_H
#define SWEIDMT_H

#include <QWidget>
#include "imageloader.h"
#include "applicationdata.h"

namespace Ui {
class SWEIDMT;
}

class SWEIDMT : public QWidget
{
    Q_OBJECT

public:
    explicit SWEIDMT(QWidget *parent = nullptr);
    ~SWEIDMT();

private:
    Ui::SWEIDMT *ui;
    //zeiger for the widget
    ImageLoader *m_pWidget;
    ApplicationData m_data;
};

#endif // SWEIDMT_H
