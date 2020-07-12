#include "sweidmt.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sweidmt w;
    w.show();
    return a.exec();
}
