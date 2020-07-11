#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H
#include <cmath>
#include <QString>
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "Eigen/Core"
#include "mylib.h"


class ApplicationData
{
public:
    ApplicationData();
    ~ApplicationData();
    bool uploadImage(QString path);
    const image3D getImage3D();

private:
    image3D imageData3D;
};

#endif // APPLICATIONDATA_H
