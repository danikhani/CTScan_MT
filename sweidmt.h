#ifndef SWEIDMT_H
#define SWEIDMT_H

#include <QWidget>

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
};

#endif // SWEIDMT_H
